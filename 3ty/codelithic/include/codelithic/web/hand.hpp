#ifndef BEEBLEBROX__HAND__Hand_HPP
#define BEEBLEBROX__HAND__Hand_HPP

#include "utils/httpresponsefile.hpp"
#include "utils/identifier.hpp"
#include "utils/uri.hpp"

class Hand {
public:
    Hand() = default;

    virtual void StartDownload() = 0;
    virtual void WaitingForDownload() = 0;
    virtual void AbortDownload() = 0;

    ~Hand() = default;

    virtual Hand &operator<<(const Beeblebrox::Uri &uri) noexcept(false) = 0;

    virtual Hand &operator<<(const Beeblebrox::Identifier &identifier) noexcept(
            false) = 0;

private:
public:
    Hand(const Hand &other) = default;
    Hand(Hand &&other) noexcept(false) = default;

    Hand &operator=(const Hand &other) {
        if (this == &other) return *this;

        return *this;
    }
    Hand &operator=(Hand &&other) noexcept(false) {
        if (this == &other) return *this;

        return *this;
    }
};

#endif// BEEBLEBROX__HAND__Hand_HPP
