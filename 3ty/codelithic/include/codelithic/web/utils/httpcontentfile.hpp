#ifndef BEEBLEBROX__COMMON_GETTER_HttpContentFile_HPP
#define BEEBLEBROX__COMMON_GETTER_HttpContentFile_HPP

#include "httpcontent.hpp"

namespace Beeblebrox {

    class HttpContentFile : public HttpContent {
    public:
        HttpContentFile() = default;
        HttpContentFile(std::string &file) { openFile(file); }
        ~HttpContentFile() = default;

        virtual int init() { return (mFileStream != nullptr && mFsize != 0) ? 0 : 1; }

        virtual int start() {
            return (mFileStream != nullptr && mFsize != 0) ? 0 : 1;
        }

        HttpContentFile &operator<<(const std::string &buffer) {
            openFile(buffer);

            return *this;
        }

        // return size * nmemb
        virtual size_t read_data(void *ptr, size_t size, size_t nitems) {
            // curl_off_t nread;
            size_t retcode = fread(ptr, size, nitems, this->mFileStream);
            // nread = (curl_off_t)retcode;
            return retcode;
        }

        virtual size_t size() { return mFsize; }

        virtual void end() {}

    private:
        FILE *mFileStream{nullptr};
        size_t mFsize{0l};

        void openFile(const std::string &file) {
            if (mFileStream != nullptr) {
                fclose(mFileStream);
                mFsize = 0l;
            }

            struct stat file_info;

            if (!stat(file.c_str(), &file_info)) {
                mFsize = (long) file_info.st_size;

                mFileStream = fopen(file.c_str(), "rb");
            } else {
                mFsize = 0l;
                mFileStream = nullptr;
            }
        }

        // can be passed only for pointer or reference
        HttpContentFile(const HttpContentFile &other) = default;
        HttpContentFile(HttpContentFile &&other) = default;
        HttpContentFile &operator=(const HttpContentFile &other) = default;
        HttpContentFile &operator=(HttpContentFile &&other) = default;
    };

}// namespace Beeblebrox
#endif