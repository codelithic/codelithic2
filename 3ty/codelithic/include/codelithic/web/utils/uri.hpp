#ifndef BEEBLEBROX__COMMON_URI_Uri_HPP
#define BEEBLEBROX__COMMON_URI_Uri_HPP

#include <curl/curl.h>

#include <algorithm>
#include <ostream>
#include <string>
#include <vector>

namespace Beeblebrox {

    // class Content {
    // public:
    // };

    class Uri {
    public:
        enum class Method {
            GET,
            POST,
            PUT,
            DELETE,
            HEADER,
            UNKNOWN,
            NOT_MANAGED,
            PATCH
        };

        Uri() = default;
        explicit Uri(std::string);

        const std::vector<std::string> getHeaders() const { return this->nHeaders; }

        const std::string &getUri() const { return mUri; }

        const std::string &getProtocol() const { return mProtocol; }
        const std::string &getHost() const { return mHost; }
        const std::string &getPath() const { return mPath; }
        const std::string &getQuery() const { return mQuery; }

        long getHttpReturnCode() { return this->mHttpReturnCode; }

        void setMethod(Method method) { mMethod = method; }

        void followUri(bool val) { mFollowUri = val; }

        bool followingUri() { return mFollowUri; }

        const char *getMethod() {
            switch (mMethod) {
                //...........................
                case Method::GET: {
                    return mGET;
                }
                case Method::POST: {
                    return mPOST;
                }
                case Method::PUT: {
                    return mPUT;
                }
                case Method::DELETE: {
                    return mDELETE;
                }
                case Method::HEADER: {
                    return mHEADER;
                }
                case Method::PATCH: {
                    return mPATCH;
                }
                case Method::UNKNOWN:
                case Method::NOT_MANAGED: {
                }

            }// switch (mMethod)

            return nullptr;
        }

        virtual void dump(std::ostream &os) {
            os << "\nmUri: " << mUri << "\nmProtocol: " << mProtocol
               << "\nmHost: " << mHost << "\nmPath: " << mPath
               << "\nmQuery: " << mQuery;
        }

        virtual ~Uri();

    private:
        constexpr static char *mGET{(char *) "GET"};
        constexpr static char *mPOST{(char *) "POST"};
        constexpr static char *mPUT{(char *) "PUT"};
        constexpr static char *mDELETE{(char *) "DELETE"};
        constexpr static char *mHEADER{(char *) "HEADER"};
        constexpr static char *mPATCH{(char *) "PATCH"};

        long mHttpReturnCode{0};
        Method mMethod{Method::GET};
        std::vector<std::string> nHeaders{};

        void reset();
        bool mFollowUri{true};

    protected:
        std::string mUri{""};
        std::string mProtocol{""};
        std::string mHost{""};
        std::string mPath{""};
        std::string mQuery{""};
        virtual void parse(const std::string &url_s);
    public:
        Uri(const Uri &other);
        Uri(Uri &&other) noexcept(false);
        Uri &operator=(const Uri &other);
        Uri &operator=(Uri &&other) noexcept(false);

        Uri &operator<<(std::string);
        Uri &operator<<(const Method &);
        Uri &operator<<(long);

        Uri &operator+(std::string);
        Uri &operator+=(std::string);

        explicit operator std::string() const;

        friend std::ostream &operator<<(std::ostream &os, const Uri &type) {
            os << type.mUri;
            return os;
        }
    };

}// namespace Beeblebrox

#endif// BEEBLEBROX__COMMON_URI_Uri_HPP
