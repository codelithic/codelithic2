#ifndef BEEBLEBROX__COMMON_AuthUserPassword_HPP
#define BEEBLEBROX__COMMON_AuthUserPassword_HPP

#include "auth.hpp"
#include <curl/curl.h>

#include <string>

namespace Beeblebrox {

    class AuthUserPassword : public Authentication {
    public:
        AuthUserPassword() = default;
        AuthUserPassword(std::string user, std::string password)
            : mUser(std::move(user)), mPassword(std::move(password)) {}
        ~AuthUserPassword() = default;

        const std::string getType() const override { return "STRING"; }
        const std::string getValue() const override {
            return (mUser + ":" + mPassword);
        }
        explicit operator std::string() const override {
            return (mUser + ":" + mPassword);
        }
        bool verify() override { return !mUser.empty() && !mPassword.empty(); }

        long getAuthenticationMethods() override { return CURLAUTH_BASIC; }

    private:
        std::string mUser{};
        std::string mPassword{};

    public:
        AuthUserPassword(const AuthUserPassword &other)
            : mUser{other.mUser}, mPassword{other.mPassword} {}
        AuthUserPassword(AuthUserPassword &&other) noexcept(false)
            : mUser{std::move(other.mUser)}, mPassword{std::move(other.mPassword)} {}
        AuthUserPassword &operator=(const AuthUserPassword &other) {
            if (this == &other) return *this;

            this->mUser = other.mUser;
            this->mPassword = other.mPassword;

            return *this;
        }
        AuthUserPassword &operator=(AuthUserPassword &&other) noexcept(false) {
            if (this == &other) return *this;

            this->mUser = std::move(other.mUser);
            this->mPassword = std::move(other.mPassword);

            return *this;
        }
    };

}// namespace Beeblebrox

#endif// BEEBLEBROX__COMMON_AuthUserPassword_HPP
