
#ifndef BEEBLEBROX__COMMON_GETTER_HttpContentString_HPP
#define BEEBLEBROX__COMMON_GETTER_HttpContentString_HPP

#include "httpcontent.hpp"
#include <codelithic/thread/InterlockedProperty.hpp>

namespace Beeblebrox {

    class HttpContentString : public HttpContent {
    public:
        explicit HttpContentString(std::string &buffer) { prepareContent(buffer); }

        explicit HttpContentString(const std::string &buffer) {
            prepareContent(buffer);
        }

        // explicit HttpContentString(unsigned char* buffer, size_t size) {
        // 	_prepareContent(buffer, size);
        // }

        HttpContentString() = default;
        ~HttpContentString() = default;

        int init() override { return 0; }

        int start() override { return 0; }

        // return size * nmemb
        size_t read_data(void *ptr, size_t size, size_t nmemb) override {
            auto buffer = mBufferStream.get();

            // fprintf(stdout, "%d,%d\n", size, nmemb);
            // fflush(stdout);

            if (buffer != nullptr) {
                auto lastPosition = this->mPosition;
                if (lastPosition >= mSize) {
                    return 0;
                }

                auto val = size * nmemb;
                auto future = val + lastPosition;

                if (future > this->mSize) {
                    val = this->mSize - lastPosition;
                }
                // 	mBufferStream->read((char*)ptr, val);
                memcpy(ptr, (void *) (buffer + lastPosition), val);
                this->mPosition += val;

                //			std::clog << this->mPosition << " : " << mSize
                //<< "\n";
                if (this->mPosition >= mSize) {
                    isDone = true;
                }

                return (size_t) val;
            }

            isDone = true;
            return (size_t) 0;
        }

        bool empty() { return mSize == 0; }

        size_t size() override { return mSize; };

        void end() override {}

        HttpContentString &operator<<(std::string &buffer) {
            prepareContent(buffer);

            return *this;
        }

        bool HasSendAll() { return isDone; }

    protected:
        std::unique_ptr<unsigned char[]> mBufferStream{nullptr};
        size_t mPosition{0};
        size_t mSize{0};

        // virtual void _prepareContent(unsigned char* buffer, size_t size) {
        // 	mBufferStream.release();

        // 	if (buffer != nullptr) {
        // 		mSize = size;
        // 		auto _m = new unsigned char[mSize];
        // 		std::memset(_m, '\0', mSize);
        // 		std::memcpy(_m, buffer, mSize);
        // 		mPosition = 0;
        // 		mBufferStream.reset(_m);
        // 		// mBufferStream

        // 	} else {
        // 		mSize = 0;
        // 		mPosition = 0;
        // 	}
        // }

        virtual void _prepareContent(const std::string &buffer) {
            mBufferStream.release();

            //		std::clog << "IL TRENO\n";

            if (!buffer.empty()) {
                mSize = (buffer.size()) /*+ 1*/;
                auto _m = new unsigned char[mSize];
                std::memset(_m, '\0', mSize);
                std::memcpy(_m, buffer.data(), mSize /*- 1*/);
                mPosition = 0;

                // auto closer_lambda = [](unsigned char* f) { delete[] f; };
                // std::unique_ptr<FILE, decltype(closer_lambda)>
                // log_lambda(fopen("file.log", "r"), closer_lambda);
                mBufferStream.reset(_m);
                // mBufferStream
            } else {
                mSize = 0;
                mPosition = 0;
            }
        }

        void prepareContent(const std::string &buffer) { _prepareContent(buffer); }

        void prepareContent(std::string &buffer) { _prepareContent(buffer); }

    private:
        codelithic::InterlockedProperty<bool> isDone{false};

        // can be passed only for pointer or reference
        // HttpContentString(const HttpContentString& other) = default;
        // HttpContentString(HttpContentString&& other) = default;
        // HttpContentString& operator=(const HttpContentString& other) = default;
        // HttpContentString& operator=(HttpContentString&& other) = default;
    };

}// namespace Beeblebrox

#endif