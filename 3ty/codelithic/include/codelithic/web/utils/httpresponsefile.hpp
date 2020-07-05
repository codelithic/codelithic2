#ifndef BEEBLEBROX_COMMON_PATH_Path_HPP
#define BEEBLEBROX_COMMON_PATH_Path_HPP

#include "httpresponse.hpp"

#include <ostream>
#include <string>

namespace Beeblebrox {

    class HttpResponseFile : public HttpResponse {
    public:
        HttpResponseFile() = default;
        explicit HttpResponseFile(std::string path) { (*this) = path; }

        int init() override { return 0; }
        int start() override {
            this->mFileStream = fopen(this->mPath.c_str(), "wb");

            return (this->mFileStream ? 0 : 1);
        }
        size_t header_data(void *ptr, size_t size, size_t nmemb) override {
            mHeaders.append((char *) ptr, size * nmemb);

            return size * nmemb;
        }
        size_t write_data(void *ptr, size_t size, size_t nmemb) override {
            return fwrite(ptr, size, nmemb, this->mFileStream);
        }

        void end() override {
            if (this->mFileStream) {
                fflush(this->mFileStream);
                fclose(this->mFileStream);

                this->mFileStream = nullptr;
            }
        }

        const std::string &GetPath() const { return mPath; }


        const std::string &getHeaders() const { return mHeaders; }

        HttpResponseFile &operator=(std::string path) noexcept(false) {
            reset();

            mPath = std::move(path);

            return *this;
        }


        ~HttpResponseFile() { end(); }

    private:
        std::string mPath{""};
        FILE *mFileStream{nullptr};
        std::string mHeaders{""};

        void reset() {
            mPath = "";
        }

    public:
        HttpResponseFile(const HttpResponseFile &other) {
            if (mFileStream != nullptr) {
                start();
            }
        }
        HttpResponseFile(HttpResponseFile &&other) noexcept(false) {
            mFileStream = other.mFileStream;
            other.mFileStream = nullptr;
        }
        HttpResponseFile &operator=(const HttpResponseFile &other) {
            if (this == &other) return *this;

            this->mPath = other.mPath;
            this->mHeaders = other.mHeaders;
            if (mFileStream != nullptr) {
                start();
            }

            return *this;
        }
        HttpResponseFile &operator=(HttpResponseFile &&other) noexcept(false) {
            if (this == &other) return *this;

            this->mPath = std::move(other.mPath);
            this->mHeaders = std::move(other.mHeaders);

            mFileStream = other.mFileStream;
            other.mFileStream = nullptr;

            return *this;
        }

        friend std::ostream &operator<<(std::ostream &os,
                                        const HttpResponseFile &type) {
            os << type.mPath << " "
               << " >"
               << "< ";

            return os;
        }
    };

}// namespace Beeblebrox

#endif// BEEBLEBROX_COMMON_PATH_Path_HPP
