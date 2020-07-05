#ifndef BEEBLEBROX_ZephodHand_ZUriDocker_HPP
#define BEEBLEBROX_ZephodHand_ZUriDocker_HPP

#include <algorithm>

#include "zaphodhand.hpp"

namespace ZaphodHands {

    class DockerUri : public Beeblebrox::Uri {
    public:
        DockerUri() : Beeblebrox::Uri() {}
        DockerUri(std::string uri) : Beeblebrox::Uri(std::move(uri)) {}

        DockerUri(const DockerUri &other)
            : Uri{other}, mSockertFile{other.mSockertFile} {}
        DockerUri(DockerUri &&other) noexcept(false)
            : Uri{std::move(other)}, mSockertFile{std::move(other.mSockertFile)} {}
        DockerUri &operator=(const DockerUri &other) {
            if (this == &other) return *this;

            Uri::operator=(other);
            mSockertFile = other.mSockertFile;
        }
        DockerUri &operator=(DockerUri &&other) noexcept(false) {
            if (this == &other) return *this;

            Uri::operator=(std::move(other));
            mSockertFile = std::move(other.mSockertFile);
        }

        void setSocketFile(std::string &file) { this->mSockertFile = file; }

        const std::string &getSocketFile() const { return mSockertFile; }

        void dump(std::ostream &os) override {
            Beeblebrox::Uri::dump(os);
            os << "\nSockertFile: " << mSockertFile;
        }

        virtual ~DockerUri() = default;

    protected:
        std::string mSockertFile{""};

        void parse(const std::string &url_s) override {
            mUri = url_s;
            const std::string prot_end(":/");
            std::string::const_iterator prot_i = std::search(
                    url_s.begin(), url_s.end(), prot_end.begin(), prot_end.end());
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
    };

}// namespace ZaphodHands

#endif