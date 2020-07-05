
#include "httpcontentstring.hpp"
#include <arpa/inet.h>
#include <string>

namespace Beeblebrox {

    class HttpContentDockerStdin : public HttpContentString {
    public:
        explicit HttpContentDockerStdin(std::string &buffer) {
            prepareContent(buffer);
        }

        explicit HttpContentDockerStdin(const std::string &buffer) {
            prepareContent(buffer);
        }

        // explicit HttpContentDockerStdin(unsigned char* u, size_t t) {
        // 	mSize = t;
        // 	mPosition = 0;
        // 	mBufferStream.reset(u);
        // }

    private:
        uint32_t deserialize_uint32(unsigned char *buffer) {
            uint32_t res = *((uint32_t *) buffer);
            return ntohl(res);
        }

        unsigned char *serialize_uint32(unsigned char *buffer, uint32_t *value) {
            *((uint32_t *) buffer) = htonl(*value);
            return buffer;
        }

    protected:
        void _prepareContent(const std::string &buffer) override {
            mBufferStream.release();
            fflush(stdout);

            if (!buffer.empty()) {
                mSize = buffer.size() + 8 /*docker header*/ + 1 /*0x0a*/;

                auto _m = new unsigned char[mSize];
                std::memset(_m, 0x00, mSize);
                _m[0] = 0x00;
                _m[1] = 0x00;
                _m[2] = 0x00;
                _m[3] = 0x00;
                uint32_t value = buffer.size();
                serialize_uint32(&_m[4], &value);
                std::memcpy(&_m[8], buffer.data(), buffer.size());
                _m[mSize - 1] = 0x0a;
                mPosition = 0;
                mBufferStream.reset(_m);
                // mBufferStream

                if (std::FILE *f1 = std::fopen("file.bin", "wb")) {
                    std::fwrite(_m, sizeof _m[0], mSize, f1);
                    std::fclose(f1);
                }

            } else {
                mSize = 0;
                mPosition = 0;
            }
        }

    private:
    };

}// namespace Beeblebrox