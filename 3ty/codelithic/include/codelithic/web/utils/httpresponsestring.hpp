#ifndef BEEBLEBROX__COMMON_GETTER_HttpResponseString_HPP
#define BEEBLEBROX__COMMON_GETTER_HttpResponseString_HPP

#include "httpresponse.hpp"

#include <iostream>
#include <string>

namespace Beeblebrox {

    class HttpResponseString : public HttpResponse {
    public:
        HttpResponseString() = default;
        ~HttpResponseString() = default;

        int init() override { return 0; }
        int start() override { return 0; }

        // return size * nmemb
        size_t write_data(void *ptr, size_t size, size_t nmemb) override {
            mContent.append((char *) ptr, size * nmemb);
            return size * nmemb;
        }

        void end() override {}

        const std::string &getContent() const { return mContent; }

        const std::string &getHeaders() const { return mHeaders; }

        size_t header_data(void *ptr, size_t size, size_t nmemb) override {
            mHeaders.append((char *) ptr, size * nmemb);

            return size * nmemb;
        }

        HttpResponseString(const HttpResponseString &other)
            : mContent{other.mContent}, mHeaders{other.mHeaders} {}
        HttpResponseString(HttpResponseString &&other) noexcept(false)
            : mContent{std::move(other.mContent)},
              mHeaders{std::move(other.mHeaders)} {}
        HttpResponseString &operator=(const HttpResponseString &other) {
            if (this == &other) return *this;

            this->mContent = other.mContent;
            this->mHeaders = other.mHeaders;
            return *this;
        }
        HttpResponseString &operator=(HttpResponseString &&other) noexcept(false) {
            if (this == &other) return *this;

            this->mContent = std::move(other.mContent);
            this->mHeaders = std::move(other.mHeaders);
            return *this;
        }

    private:
        std::string mContent{""};
        std::string mHeaders{""};
    };// namespace Beeblebrox

}// namespace Beeblebrox
#endif// BEEBLEBROX__COMMON_GETTER_HttpResponseString_HPP
