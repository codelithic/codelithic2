
#pragma once
#include <mutex>

namespace codelithic {

    template<typename T>
    class InterlockedProperty {
    public:
        InterlockedProperty() : InterlockedProperty(nullptr, nullptr) {}

        InterlockedProperty(const T &value) : InterlockedProperty(nullptr, &value) {}

        InterlockedProperty(std::mutex &sharedMutex)
            : InterlockedProperty(&sharedMutex, nullptr) {}

        InterlockedProperty(std::mutex &sharedMutex, const T &value)
            : InterlockedProperty(&sharedMutex, &value) {}

        InterlockedProperty &operator=(InterlockedProperty &other) {
            this->mutex = other.mutex;
            this->value = other.value;
            return *this;
        }// operator=

        void UseSharedMutex(std::mutex &mutex) { this->mutex = mutex; }

        operator T() const {
            std::lock_guard<std::mutex> lock(*mutex);
            return value;
        }

        T operator=(const T &value) {
            std::lock_guard<std::mutex> lock(*mutex);
            return this->value = value;
        }

    private:
        InterlockedProperty(std::mutex *sharedMutex, const T *value) {
            if (sharedMutex == nullptr)
                mutex = &uniqueMutex;
            else
                mutex = sharedMutex;
            if (value != nullptr)// don't use mutex to interlock value here: it could
                                 // be not yet fully constructed
                this->value = *value;
        }// InterlockedProperty

        std::mutex uniqueMutex{};
        std::mutex *mutex{nullptr};
        T value;
    };
}// namespace codelithic