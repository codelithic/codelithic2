#ifndef BEEBLEBROX__COMMON_GETTER_HttpResponse_HPP
#define BEEBLEBROX__COMMON_GETTER_HttpResponse_HPP

//#include <iostream>
#include <string>

namespace Beeblebrox {

    class HttpResponse {
    public:
        virtual int init() = 0;
        virtual int start() = 0;

        // return size * nmemb
        virtual size_t write_data(void *ptr, size_t size, size_t nmemb) = 0;
        virtual size_t header_data(void *ptr, size_t size, size_t nmemb) {
            return size * nmemb;
        }
        virtual void end() = 0;
    };

}// namespace Beeblebrox
#endif// BEEBLEBROX__COMMON_GETTER_HttpResponse_HPP
