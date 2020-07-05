
#ifndef CODELITHIC_DELEGATE_BASE_H
#define CODELITHIC_DELEGATE_BASE_H

//------------------------------------------------------------------------------------------------------------
// link
//------------------------------------------------------------------------------------------------------------
// https://en.wikipedia.org/wiki/Partial_template_specialization#Partial_specialization
// https://en.wikipedia.org/wiki/Variadic_template#C.2B.2B
//------------------------------------------------------------------------------------------------------------
namespace codelithic {

    template<typename T>
    class delegate_base;

    template<typename RET, typename... PARAMS>
    class delegate_base<RET(PARAMS...)> {
    protected:
        using stub_type = RET (*)(void *this_ptr, PARAMS...);

        struct InvocationElement {
            InvocationElement() = default;

            InvocationElement(void *this_ptr, stub_type aStub)
                : object(this_ptr), stub(aStub) {}

            void Clone(InvocationElement &target) const {
                target.stub = stub;
                target.object = object;
            }

            bool operator==(const InvocationElement &another) const {
                return another.stub == stub && another.object == object;
            }

            bool operator!=(const InvocationElement &another) const {
                return another.stub != stub || another.object != object;
            }

            void *object = nullptr;

            stub_type stub = nullptr;
        };
    };

}// namespace codelithic
#endif
