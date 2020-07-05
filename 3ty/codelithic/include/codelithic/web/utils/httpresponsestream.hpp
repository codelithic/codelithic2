#ifndef BEEBLEBROX__COMMON_GETTER_HttpResponseStream_HPP
#define BEEBLEBROX__COMMON_GETTER_HttpResponseStream_HPP

#include "httpresponse.hpp"
#include <cstdio>

namespace Beeblebrox {

    // TODO: to complete
    class HttpResponseStream : public HttpResponse {
    public:
        HttpResponseStream() = default;
        ~HttpResponseStream() = default;

        explicit HttpResponseStream(std::ostream *ostream) : mStream{ostream} {}
        int init() override { return 0; }
        int start() override { return 0; }

        // return size * nmemb
        //// https://en.cppreference.com/w/cpp/io/c/fprintf
        size_t write_data(void *ptr, size_t size, size_t nmemb) override {
            // printf("%.*s", (int)(size * nmemb), (char*)ptr);
            if (mStream != nullptr) {
                size_t cursor = 0;
                while (cursor < (size * nmemb)) {
                    (*mStream) << (static_cast<unsigned char *>(ptr))[cursor++];
                }
            }
            return size * nmemb;
        }

        void end() override {}

        HttpResponseStream(const HttpResponseStream &other) {}
        HttpResponseStream(HttpResponseStream &&other) noexcept(false) {}
        HttpResponseStream &operator=(const HttpResponseStream &other) {
            if (this == &other) return *this;

            this->mStream = other.mStream;

            return *this;
        }
        HttpResponseStream &operator=(HttpResponseStream &&other) noexcept(false) {
            if (this == &other) return *this;

            return *this;
        }

    private:
        std::ostream *mStream{nullptr};
    };// namespace Beeblebrox

}// namespace Beeblebrox
#endif