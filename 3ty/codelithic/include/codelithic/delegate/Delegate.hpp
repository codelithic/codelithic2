
#ifndef CODELITHIC_DELEGATE_H
#define CODELITHIC_DELEGATE_H

#include "DelegateBase.hpp"

//------------------------------------------------------------------------------------------------------------
// link
//------------------------------------------------------------------------------------------------------------
// https://en.wikipedia.org/wiki/Partial_template_specialization#Partial_specialization
// https://en.wikipedia.org/wiki/Variadic_template#C.2B.2B
//------------------------------------------------------------------------------------------------------------
namespace codelithic {

    template<typename T>
    class delegate;
    template<typename T>
    class multicast_delegate;

    template<typename RET, typename... PARAMS>
    class delegate<RET(PARAMS...)> final : private delegate_base<RET(PARAMS...)> {
    public:
        delegate() = default;

        bool isNull() const { return invocation.stub == nullptr; }

        bool operator==(void *ptr) const {
            return (ptr == nullptr) && this->isNull();
        }// operator ==

        bool operator!=(void *ptr) const {
            return (ptr != nullptr) || (!this->isNull());
        }// operator !=

        delegate(const delegate &another) { another.invocation.Clone(invocation); }

        template<typename LAMBDA>
        delegate(const LAMBDA &lambda) {
            assign((void *) (&lambda), lambda_stub<LAMBDA>);
        }// delegate

        delegate &operator=(const delegate &another) {
            another.invocation.Clone(invocation);
            return *this;
        }// operator =

        template<typename LAMBDA>
        delegate &operator=(const LAMBDA &instance) {
            assign((void *) (&instance), lambda_stub<LAMBDA>);
            return *this;
        }// operator =

        bool operator==(const delegate &another) const {
            return invocation == another.invocation;
        }

        bool operator!=(const delegate &another) const {
            return invocation != another.invocation;
        }

        bool operator==(const multicast_delegate<RET(PARAMS...)> &another) const {
            return another == (*this);
        }

        bool operator!=(const multicast_delegate<RET(PARAMS...)> &another) const {
            return another != (*this);
        }

        template<class T, RET (T::*TMethod)(PARAMS...)>
        static delegate create(T *instance) {
            return delegate(instance, method_stub<T, TMethod>);
        }

        template<class T, RET (T::*TMethod)(PARAMS...) const>
        static delegate create(T const *instance) {
            return delegate(const_cast<T *>(instance), const_method_stub<T, TMethod>);
        }

        template<RET (*TMethod)(PARAMS...)>
        static delegate create() {
            return delegate(nullptr, function_stub<TMethod>);
        }

        template<typename LAMBDA>
        static delegate create(const LAMBDA &instance) {
            return delegate((void *) (&instance), lambda_stub<LAMBDA>);
        }

        RET operator()(PARAMS... arg) const {
            return (*invocation.stub)(invocation.object, arg...);
        }

    private:
        delegate(void *anObject,
                 typename delegate_base<RET(PARAMS...)>::stub_type aStub) {
            invocation.object = anObject;
            invocation.stub = aStub;
        }

        void assign(void *anObject,
                    typename delegate_base<RET(PARAMS...)>::stub_type aStub) {
            this->invocation.object = anObject;
            this->invocation.stub = aStub;
        }

        template<class T, RET (T::*TMethod)(PARAMS...)>
        static RET method_stub(void *this_ptr, PARAMS... params) {
            T *p = static_cast<T *>(this_ptr);
            return (p->*TMethod)(params...);
        }

        template<class T, RET (T::*TMethod)(PARAMS...) const>
        static RET const_method_stub(void *this_ptr, PARAMS... params) {
            T *const p = static_cast<T *>(this_ptr);
            return (p->*TMethod)(params...);
        }

        template<RET (*TMethod)(PARAMS...)>
        static RET function_stub(void *this_ptr, PARAMS... params) {
            return (TMethod)(params...);
        }

        template<typename LAMBDA>
        static RET lambda_stub(void *this_ptr, PARAMS... arg) {
            LAMBDA *p = static_cast<LAMBDA *>(this_ptr);
            return (p->operator())(arg...);
        }

        friend class multicast_delegate<RET(PARAMS...)>;
        typename delegate_base<RET(PARAMS...)>::InvocationElement invocation;

    };// class delegate

}// namespace codelithic

#endif
