
#ifndef BEEBLEBROX__COMMON__COOKIE_HPP
#define BEEBLEBROX__COMMON__COOKIE_HPP

#include <string>

namespace Beeblebrox {

    class Cookie {
    public:
        Cookie() {}

        Cookie(const std::string &name, const std::string &value)
            : name(name), value(value) {}

        Cookie(const Cookie &other) : name(other.name), value(other.value) {}

        Cookie(Cookie &&other) noexcept(false)
            : name(std::move(other.name)), value(std::move(other.value)) {}

        Cookie &operator=(const Cookie &other) {}

        Cookie &operator=(Cookie &&other) noexcept(false) {}

    private:
        std::string name{""};
        std::string value{""};
    };

    class CookieInLine {};

}// namespace Beeblebrox

#endif// BEEBLEBROX__COMMON__COOKIE_HPP
