#ifndef BEEBLEBROX_Identifier_Identifier_HPP
#define BEEBLEBROX_Identifier_Identifier_HPP

#include <ostream>
#include <string>

namespace Beeblebrox {

    class Identifier {
    public:
        Identifier() {
            // this->mUuid = Uuid::getTimeSafe();
        }

        explicit Identifier(std::string uuid) : mUuid{std::move(uuid)} {}

        void setName(std::string name) { this->mName = std::move(name); }

        const std::string &getName() const { return this->mName; }

        const std::string &getNUuid() const { return this->mUuid; }

        ~Identifier() = default;

    private:
        std::string mName{""};
        std::string mUuid{""};

    public:
        Identifier(const Identifier &other)
            : mName{other.mName}, mUuid{other.mUuid} {}

        Identifier(Identifier &&other) noexcept(false)
            : mName{std::move(other.mName)}, mUuid{std::move(other.mUuid)} {}
        Identifier &operator=(const Identifier &other) {
            if (this == &other) return *this;

            this->mName = other.mName;
            this->mUuid = other.mUuid;

            return *this;
        }
        Identifier &operator=(Identifier &&other) noexcept(false) {
            if (this == &other) return *this;

            this->mName = std::move(other.mName);
            this->mUuid = std::move(other.mUuid);

            return *this;
        }

        friend std::ostream &operator<<(std::ostream &os, const Identifier &type) {
            os << type.mName << " " << type.mUuid;
            return os;
        }
    };

}// namespace Beeblebrox

#endif// BEEBLEBROX_Identifier_Identifier_HPP
