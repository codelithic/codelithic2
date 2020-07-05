#ifndef BEEBLEBROX__COMMON_Auth_Auth_HPP
#define BEEBLEBROX__COMMON_Auth_Auth_HPP


#include <string>

namespace Beeblebrox {

    class Authentication {
    public:
        virtual const std::string getType() const = 0;
        virtual const std::string getValue() const = 0;
        virtual operator std::string() const = 0;
        virtual bool verify() = 0;

        virtual long getAuthenticationMethods() = 0;
    };

}// namespace Beeblebrox
#endif