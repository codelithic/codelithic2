
#ifndef CODELITHIC_DELEGATEMULTICAST_BASE_H
#define CODELITHIC_DELEGATEMULTICAST_BASE_H

#include <codelithic/delegate/Delegate.hpp>
#include <functional>
#include <list>

//------------------------------------------------------------------------------------------------------------
// link
//------------------------------------------------------------------------------------------------------------
// https://en.wikipedia.org/wiki/Partial_template_specialization#Partial_specialization
// https://en.wikipedia.org/wiki/Variadic_template#C.2B.2B
//------------------------------------------------------------------------------------------------------------
namespace codelithic {

    template<typename RET, typename... PARAMS>
    class multicast_delegate<RET(PARAMS...)> final
        : private delegate_base<RET(PARAMS...)> {
    public:
        multicast_delegate() = default;

        ~multicast_delegate() {
            for (auto &element : invocationList) delete element;
            invocationList.clear();
        }

        bool isNull() const { return invocationList.size() < 1; }

        bool operator==(void *ptr) const {
            return (ptr == nullptr) && this->isNull();
        }

        bool operator!=(void *ptr) const {
            return (ptr != nullptr) || (!this->isNull());
        }

        size_t size() const { return invocationList.size(); }

        multicast_delegate &operator=(const multicast_delegate &) = delete;
        multicast_delegate(const multicast_delegate &) = delete;

        bool operator==(const multicast_delegate &another) const {
            if (invocationList.size() != another.invocationList.size()) return false;
            auto anotherIt = another.invocationList.begin();
            for (auto it = invocationList.begin(); it != invocationList.end(); ++it)
                if (**it != **anotherIt) return false;
            return true;
        }

        bool operator!=(const multicast_delegate &another) const {
            return !(*this == another);
        }

        bool operator==(const delegate<RET(PARAMS...)> &another) const {
            if (isNull() && another.isNull()) return true;
            if (another.isNull() || (size() != 1)) return false;
            return (another.invocation == **invocationList.begin());
        }

        bool operator!=(const delegate<RET(PARAMS...)> &another) const {
            return !(*this == another);
        }

        multicast_delegate &operator+=(const multicast_delegate &another) {
            for (auto &item :
                 another.invocationList)// clone, not copy; flattens hierarchy:
                this->invocationList.push_back(
                        new typename delegate_base<RET(PARAMS...)>::InvocationElement(
                                item->object, item->stub));
            return *this;
        }

        template<typename LAMBDA>// template instantiation is not neededm, will be
        // deduced/inferred:
        multicast_delegate &operator+=(
                const LAMBDA &lambda) {
            delegate<RET(PARAMS...)> d =
                    delegate<RET(PARAMS...)>::template create<LAMBDA>(lambda);
            return *this += d;
        }

        multicast_delegate &operator+=(const delegate<RET(PARAMS...)> &another) {
            if (another.isNull()) return *this;
            this->invocationList.push_back(
                    new typename delegate_base<RET(PARAMS...)>::InvocationElement(
                            another.invocation.object, another.invocation.stub));
            return *this;
        }

        void operator()(PARAMS... arg) const {
            for (auto &item : invocationList) (*(item->stub))(item->object, arg...);
        }

        template<typename HANDLER>
        void operator()(PARAMS... arg, HANDLER handler) const {
            size_t index = 0;
            for (auto &item : invocationList) {
                RET value = (*(item->stub))(item->object, arg...);
                handler(index, &value);
                ++index;
            }
        }

        void operator()(PARAMS... arg, delegate<void(size_t, RET *)> handler) const {
            operator()<decltype(handler)>(arg..., handler);
        }

        void operator()(PARAMS... arg,
                        std::function<void(size_t, RET *)> handler) const {
            operator()<decltype(handler)>(arg..., handler);
        }

    private:
        std::list<typename delegate_base<RET(PARAMS...)>::InvocationElement *>
                invocationList;
    };

}// namespace codelithic

#endif