
#include <algorithm>
#include <functional>

#include <codelithic/web/utils/uri.hpp>

namespace Beeblebrox {

    Uri::Uri(std::string host) : mUri{std::move(host)} { parse(mUri); }

    void Uri::parse(const std::string &url_s) {
        mUri = url_s;
        const std::string prot_end("://");
        std::string::const_iterator prot_i =
                std::search(url_s.begin(), url_s.end(), prot_end.begin(), prot_end.end());
        mProtocol.reserve(std::distance(url_s.begin(), prot_i));
        transform(url_s.begin(), prot_i, back_inserter(mProtocol),
                  std::ptr_fun<int, int>(tolower));// protocol is icase
        if (prot_i == url_s.end()) return;
        std::advance(prot_i, prot_end.length());
        std::string::const_iterator path_i = std::find(prot_i, url_s.end(), '/');
        mHost.reserve(std::distance(prot_i, path_i));
        transform(prot_i, path_i, back_inserter(mHost),
                  std::ptr_fun<int, int>(tolower));// host is icase
        std::string::const_iterator query_i = std::find(path_i, url_s.end(), '?');
        mPath.assign(path_i, query_i);
        if (query_i != url_s.end()) ++query_i;
        mQuery.assign(query_i, url_s.end());
    }

    void Uri::reset() {
        mUri = "";
        mProtocol = "";
        mHost = "";
        mPath = "";
        mQuery = "";
    }

    Uri &Uri::operator<<(long httpReturnCode) {
        this->mHttpReturnCode = httpReturnCode;
        return *this;
    }

    Uri &Uri::operator<<(const Method &method) {
        mMethod = method;
        return *this;
    }

    Uri &Uri::operator<<(std::string val) {
        reset();
        parse(val);
        return *this;
    }

    Uri::~Uri() {}

    Uri::Uri(const Uri &other)
        : mHttpReturnCode{other.mHttpReturnCode},
          mMethod{other.mMethod},
          nHeaders{other.nHeaders},
          mUri{other.mUri},
          mProtocol{other.mProtocol},
          mHost{other.mHost},
          mPath{other.mPath},
          mQuery{other.mQuery}
    {}

    Uri::Uri(Uri &&other)
        : mHttpReturnCode{other.mHttpReturnCode},
          mMethod{other.mMethod},
          nHeaders{std::move(other.nHeaders)},
          mUri{std::move(other.mUri)},
          mProtocol{std::move(other.mProtocol)},
          mHost{std::move(other.mHost)},
          mPath{std::move(other.mPath)},
          mQuery{std::move(other.mQuery)} {}

    Uri &Uri::operator=(const Uri &other) {
        if (this == &other) return *this;

        this->mUri = other.mUri;
        this->mProtocol = other.mProtocol;
        this->mHost = other.mHost;
        this->mPath = other.mPath;
        this->mQuery = other.mQuery;
        this->mHttpReturnCode = other.mHttpReturnCode;
        this->mMethod = other.mMethod;
        this->nHeaders = other.nHeaders;

        return *this;
    }

    Uri &Uri::operator=(Uri &&other) noexcept(false) {
        if (this == &other) return *this;

        this->mUri = std::move(other.mUri);
        this->mProtocol = std::move(other.mProtocol);
        this->mHost = std::move(other.mHost);
        this->mPath = std::move(other.mPath);
        this->mQuery = std::move(other.mQuery);
        this->mHttpReturnCode = other.mHttpReturnCode;
        this->mMethod = other.mMethod;
        this->nHeaders = std::move(other.nHeaders);

        return *this;
    }

    Uri &Uri::operator+(std::string header) {
        this->nHeaders.push_back(std::move(header));
        return *this;
    }

    Uri &Uri::operator+=(std::string header) {
        this->nHeaders.push_back(std::move(header));
        return *this;
    }

    Uri::operator std::string() const { return this->mUri; }

}// namespace Beeblebrox