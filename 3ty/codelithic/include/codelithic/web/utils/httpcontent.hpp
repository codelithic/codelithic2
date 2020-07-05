#ifndef BEEBLEBROX__COMMON_GETTER_httpcontent_HPP
#define BEEBLEBROX__COMMON_GETTER_httpcontent_HPP

#include <cstdio>
#include <cstring>
#include <memory>
#include <sstream>
#include <string>

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

namespace Beeblebrox {
    class HttpContent {
    public:
        HttpContent() = default;
        virtual ~HttpContent() = default;

        virtual int init() = 0;
        virtual int start() = 0;

        // return size * nmemb
        virtual size_t read_data(void *buffer, size_t size, size_t nitems) = 0;
        virtual size_t size() = 0;

        virtual void end() = 0;
    };

}// namespace Beeblebrox
#endif

// https://curl.haxx.se/libcurl/c/CURLOPT_UPLOAD.html
// https://curl.haxx.se/libcurl/c/CURLOPT_READFUNCTION.html
