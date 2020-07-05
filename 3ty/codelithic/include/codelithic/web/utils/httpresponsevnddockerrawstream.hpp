#ifndef BEEBLEBROX__COMMON_GETTER_HttpResponseVndDockerRawStream_HPP
#define BEEBLEBROX__COMMON_GETTER_HttpResponseVndDockerRawStream_HPP

#include "httpresponse.hpp"
#include <codelithic/thread/InterlockedProperty.hpp>

#include <cstdio>
#include <cstring>

namespace Beeblebrox {

    // TODO: va creato un sistema per definire stderr strdout stream
    class HttpResponseVndDockerRawStream : public HttpResponse {
    public:
        HttpResponseVndDockerRawStream() = default;
        ~HttpResponseVndDockerRawStream() = default;

        int init() override { return 0; }
        int start() override { return 0; }

        size_t header_data(void *ptr, size_t size, size_t nmemb) override {
            auto res = size * nmemb;
            if (res == 2) {
                printf("header>>> %.*s", (int) (size * nmemb), (char *) ptr);
                this->isDone = true;
            }
            //		std::clog<< "RES:" << res  << " isDone:" << isDone <<"\n";
            return res;
        }
        // return size * nmemb
        //// https://en.cppreference.com/w/cpp/io/c/fprintf
        size_t write_data(void *ptr, size_t size, size_t nmemb) override {
            // TODO: va creato un sistema per definire stderr strdout stream

            auto etherhead = static_cast<char *>(ptr);
            //		// etherhead = etherhead + (sizeof(char) * 8);
            //		// fprintf(stdout, "%.*s", (int)((size * nmemb) - 8),
            //etherhead); 		std::cout << "FRAME:--------INI\n"; 		for (int i = 0; i < ((size
            //* nmemb) - 0); ++i) { 			std::cout << etherhead[i];
            //		}
            //		std::cout << "FRAME:--------END\n";
            //		// auto Y = fopen("cc", "wb");
            //		// fwrite(&((char*)(ptr))[8], size, nmemb - 8, Y);
            //		// fflush(Y);
            //		// fclose(Y);

            fflush(stdout);
            return size * nmemb;
        }

        void end() override {}

        HttpResponseVndDockerRawStream(const HttpResponseVndDockerRawStream &other) {}
        HttpResponseVndDockerRawStream(
                HttpResponseVndDockerRawStream &&other) noexcept(false) {}
        HttpResponseVndDockerRawStream &operator=(
                const HttpResponseVndDockerRawStream &other) {
            if (this == &other) return *this;

            return *this;
        }
        HttpResponseVndDockerRawStream &operator=(
                HttpResponseVndDockerRawStream &&other) noexcept(false) {
            if (this == &other) return *this;

            return *this;
        }

        bool HasHeaderFinished() { return isDone; }
        codelithic::InterlockedProperty<bool> isDone{false};

    private:
    };// namespace Beeblebrox

}// namespace Beeblebrox
#endif