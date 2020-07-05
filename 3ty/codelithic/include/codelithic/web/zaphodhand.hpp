#ifndef BEEBLEBROX_ZephodHand_ZephodHand_HPP
#define BEEBLEBROX_ZephodHand_ZephodHand_HPP

#include <codelithic/delegate/Delegate.hpp>
#include <codelithic/thread/InterlockedProperty.hpp>
#include <codelithic/thread/ThreadWrapper.hpp>

#include "utils/authuserpassword.hpp"
#include "utils/httpresponse.hpp"
#include "utils/identifier.hpp"
#include "utils/uri.hpp"

#include "utils/httpcontent.hpp"
#include "utils/httpresponsefile.hpp"

#include <curl/curl.h>

#include <memory>
#include <string>


namespace ZaphodHands {

#define SKIP_PEER_VERIFICATION
#define SKIP_HOSTNAME_VERIFICATION

#define ZAPHODHAND_USER_AGENT ((const char *) "ZaphodHand/client")

    // https://gist.github.com/whoshuu/2dc858b8730079602044
    class ZaphodHand : private codelithic::ThreadWrapper {
    public:
        ZaphodHand() : mIsRunning(mutex, false), mBlock(mutex, false) {}
        ~ZaphodHand() { mIsRunning = false; }

    public:
        void AbortHand() { mBlock = true; }
        void Run() {
            this->Body();
            // this->Async()->WaitingForDownload();
        }
        ZaphodHand *Async() {
            this->Start();
            return this;
        }
        void WaitingForDownload() { this->Join(); }

        ZaphodHand &operator<<(Beeblebrox::Uri *uri) noexcept(false) {
            mUri = uri;
            return *this;
        }
        ZaphodHand &operator<<(const Beeblebrox::Identifier &identifier) noexcept(
                false) {
            this->mIdentifier = identifier;
            return *this;
        }
        ZaphodHand &operator<<(Beeblebrox::HttpResponse *getter) noexcept(false) {
            mHttpReturContent = getter;
        }
        ZaphodHand &operator<<(Beeblebrox::AuthUserPassword *authentication) noexcept(
                false) {
            mAuthentication = authentication;

            return *this;
        }
        ZaphodHand &operator<<(Beeblebrox::HttpContent *httpContent) noexcept(false) {
            this->mHttpContent = httpContent;
            return *this;
        }

    protected:
        void Body() override {
            {
                mElapsed = 0;
                resetError();
                mBlock = false;
                mIsRunning = true;

                if (this->mUri == nullptr) {
                    throw std::runtime_error("Object Uri not defined");
                }

                if (this->mUri->getUri().empty()) {
                    throw std::runtime_error("Getter not defined");
                }

                if (this->mHttpReturContent == nullptr) {
                    throw std::runtime_error("Getter not defined");
                }

                int iRes = this->mHttpReturContent->init();
                if (iRes) {
                    throw std::runtime_error("HttpReturContent init failed");
                }

                iRes = this->mHttpReturContent->start();
                if (iRes) {
                    closeAll();
                    throw std::runtime_error("HttpReturContent start failed");
                }

                resCurl = curl_global_init(CURL_GLOBAL_ALL);
                if (resCurl != CURLE_OK) {
                    closeAll();
                    throw std::runtime_error(curl_easy_strerror(resCurl));
                }

                curl_handle = curl_easy_init();
                if (curl_handle) {
                    //    /* pass in that last of FTP commands to run after the transfer */
                    //     curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);
                    __setCustomRequest();

                    __setUri();

                    __setFollowUri();

                    __setTrasfertByteForSecond();

                    __setVerificationCertificates();

                    __setWriteDataFunction();

                    __setAuthentication();

                    __setUserAgent();

                    __setHttpContent();

                    __setConnectionTimeOut();

                    __setTimeoutTransfert();

                    __setFailOnHttpError();

                    __setHeaders();

                    if (addCurlOptions()) {
                        closeAll();
                        this->mError = curl_easy_strerror(resCurl);
                        throw std::runtime_error(this->mError);
                    }

                    __setHttpContentHeaders();

                    curl_easy_setopt(curl_handle, CURLOPT_TCP_KEEPALIVE, 1L);

                    /* keep-alive idle time to 120 seconds */
                    curl_easy_setopt(curl_handle, CURLOPT_TCP_KEEPIDLE, 120L);

                    /* interval time between keep-alive probes: 60 seconds */
                    curl_easy_setopt(curl_handle, CURLOPT_TCP_KEEPINTVL, 60L);

                    if (verbose) curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
                    //		curl_easy_setopt(curl_handle,
                    //CURLOPT_EXPECT_100_TIMEOUT_MS, 3000L);

                    __setPerform();

                    __setGetHttpCode();

                    __setElapsedTime();

                    closeAll();

                } else {
                    throw std::runtime_error("Init lib error!");
                }
            }
        }
        void Aborted() override {
            mError = "Aborted!!";
            mBlock = true;
        }
        void Terminated() override {
            mIsRunning = false;
            // std::cout << "terminate \n";
        }
        void ExceptionCaught(std::exception &exception) override {
            mError = exception.what();
        }
        void UnknownExceptionCaught() override {
            mError = "Unknown Exception Caught!!";
        }

    public:
    protected:
        CURL *curl_handle{nullptr};
        CURLcode resCurl{CURLE_OK};
        struct curl_slist *slist{nullptr};
        static int xferinfo(void *p, curl_off_t dltotal, curl_off_t dlnow,
                            curl_off_t ultotal, curl_off_t ulnow) {
            // std::cerr << " >>"
            // 		  << (dlnow == 0 || dltotal == 0
            // 				  ? 1
            // 				  : static_cast<int>((static_cast<float>(dlnow) /
            // static_cast<float>(dltotal)) * 									 static_cast<float>(100)))
            // 		  << "%< " << ulnow << " " << ultotal << " " << dlnow << " " << dltotal
            // << "\n";

            // std::cout << "\r >>"
            // 		  << (dlnow == 0 || dltotal == 0
            // 				  ? 1
            // 				  : static_cast<int>((static_cast<float>(dlnow) /
            // static_cast<float>(dltotal)) * 									 static_cast<float>(100)))
            // 		  << "%"
            // 		  << "";

            ZaphodHand *zh = static_cast<ZaphodHand *>(p);
            zh->SyncPoint();

            // if (dlnow > STOP_DOWNLOAD_AFTER_THIS_MANY_BYTES)
            // 	return 1;

            // std::cout << dltotal << " " << dlnow << " " << ultotal << " " << ulnow <<
            // "\n"; fflush(stdout);

            if (zh->mBlock) {
                return 1;
            }

            return 0;
        }
        double mElapsed{0};
#if LIBCURL_VERSION_NUM < 0x072000
        static int older_progress(void *p, double dltotal, double dlnow,
                                  double ultotal, double ulnow) {
            return xferinfo(p, (curl_off_t) dltotal, (curl_off_t) dlnow,
                            (curl_off_t) ultotal, (curl_off_t) ulnow);
        }
#endif
        static size_t header_data(void *ptr, size_t size, size_t nmemb,
                                  void *stream) {
            ZaphodHand *zh = static_cast<ZaphodHand *>(stream);
            if (zh->mHttpReturContent != nullptr) {
                return zh->mHttpReturContent->header_data(ptr, size, nmemb);
            }

            return size * nmemb;
        }
        static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
            ZaphodHand *zh = static_cast<ZaphodHand *>(stream);
            if (zh->mHttpReturContent != nullptr) {
                // std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 5));
                return zh->mHttpReturContent->write_data(ptr, size, nmemb);
            }
            return size * nmemb;
        }
        static size_t read_data(void *ptr, size_t size, size_t nmemb, void *stream) {
            ZaphodHand *zh = static_cast<ZaphodHand *>(stream);
            if (zh->mHttpContent != nullptr) {
                return zh->mHttpContent->read_data(ptr, size, nmemb);
            }
            return 0;
        }

        Beeblebrox::Identifier mIdentifier{};
        Beeblebrox::AuthUserPassword *mAuthentication{nullptr};
        Beeblebrox::HttpResponse *mHttpReturContent{nullptr};
        Beeblebrox::HttpContent *mHttpContent{nullptr};
        Beeblebrox::Uri *mUri{nullptr};

        void closeAll() {
            if (slist != nullptr) curl_slist_free_all(slist);

            if (curl_handle) {
                curl_easy_cleanup(curl_handle);
                curl_global_cleanup();

                curl_handle = nullptr;
            }

            if (this->mHttpReturContent) {
                this->mHttpReturContent->end();
            }
        }
        std::string mError{""};
        bool verbose{false};

        codelithic::InterlockedProperty<bool> mBlock;
        codelithic::InterlockedProperty<bool> mIsRunning;
        std::mutex mutex;

        long mConnectionTimeout{-1};
        long mTimeoutTrasfertMs{-1};

        virtual bool addCurlOptions() { return false; }

    public:
        void setVerbose(bool val) { this->verbose = val; }

        double getElapsetTime() { return mElapsed; }

        void setConnectionTimeout(long val) { this->mConnectionTimeout = val; }

        void setTimeoutTrasfert(long val) { this->mTimeoutTrasfertMs = val; }

        int getErrorCode() { return static_cast<int>(resCurl); }

        const std::string &getError() const { return this->mError; }

        const Beeblebrox::Identifier &getName() const { return this->mIdentifier; }

        Beeblebrox::Uri *getUri() { return this->mUri; }

        bool isRunning() { return mIsRunning; }

    public:
        // create var costructor

        // *************************************************
        // CAN BE PASS ONLY FOR REFERENCE OR POINTER
    private:
        ZaphodHand(const ZaphodHand &other) = default;
        ZaphodHand(ZaphodHand &&other) noexcept(false) = default;
        // ZaphodHand& operator=(const ZaphodHand& other) = default;
        // ZaphodHand& operator=(ZaphodHand&& other) noexcept(false) = default;
        // *************************************************
    protected:
        virtual void __setCustomRequest() {
            if (this->mUri != nullptr) {
                curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST,
                                 this->mUri->getMethod());
            }
        }
        virtual void __setUri() {
            if (this->mUri != nullptr) {
                resCurl = curl_easy_setopt(curl_handle, CURLOPT_URL,
                                           this->mUri->getUri().c_str());
                if (resCurl != CURLE_OK) {
                    closeAll();
                    this->mError = curl_easy_strerror(resCurl);
                    throw std::runtime_error(this->mError);
                }
            }
        }
        virtual void __setFollowUri() {
            if (this->mUri != nullptr && this->mUri->followingUri()) {
                resCurl = curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
                if (resCurl != CURLE_OK) {
                    closeAll();
                    this->mError = curl_easy_strerror(resCurl);
                    throw std::runtime_error(this->mError);
                }
            }
        }
        virtual void __setVerificationCertificates() {
#ifdef SKIP_PEER_VERIFICATION
            resCurl = curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
            if (resCurl != CURLE_OK) {
                closeAll();
                this->mError = curl_easy_strerror(resCurl);
                throw std::runtime_error(this->mError);
            }
#endif
#ifdef SKIP_HOSTNAME_VERIFICATION
            resCurl = curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
            if (resCurl != CURLE_OK) {
                closeAll();
                this->mError = curl_easy_strerror(resCurl);
                throw std::runtime_error(this->mError);
            }
#endif
        }
        virtual void __setWriteDataFunction() {
            resCurl = curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0L);
            if (resCurl != CURLE_OK) {
                closeAll();
                this->mError = curl_easy_strerror(resCurl);
                throw std::runtime_error(this->mError);
            }

            resCurl = curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION,
                                       ZaphodHand::write_data);
            if (resCurl != CURLE_OK) {
                closeAll();
                this->mError = curl_easy_strerror(resCurl);
                throw std::runtime_error(this->mError);
            }

            resCurl = curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA,
                                       this);// puo' essere la classe thread
            if (resCurl != CURLE_OK) {
                closeAll();
                this->mError = curl_easy_strerror(resCurl);
                throw std::runtime_error(this->mError);
            }
#if LIBCURL_VERSION_NUM >= 0x072000
            resCurl = curl_easy_setopt(curl_handle, CURLOPT_XFERINFOFUNCTION,
                                       ZaphodHand::xferinfo);
            if (resCurl != CURLE_OK) {
                closeAll();
                this->mError = curl_easy_strerror(resCurl);
                throw std::runtime_error(this->mError);
            }
            resCurl = curl_easy_setopt(curl_handle, CURLOPT_XFERINFODATA, this);
            if (resCurl != CURLE_OK) {
                closeAll();
                this->mError = curl_easy_strerror(resCurl);
                throw std::runtime_error(this->mError);
            }
#else
            resCurl =
                    curl_easy_setopt(curl_handle, CURLOPT_PROGRESSFUNCTION, older_progress);
            if (resCurl != CURLE_OK) {
                closeAll();
                throw std::runtime_error(curl_easy_strerror(resCurl));
            }
            resCurl = curl_easy_setopt(curl_handle, CURLOPT_PROGRESSDATA, this);
            if (resCurl != CURLE_OK) {
                closeAll();
                throw std::runtime_error(curl_easy_strerror(resCurl));
            }
#endif
        }

        virtual void __setAuthentication() {
            if (mAuthentication != nullptr) {
                resCurl = curl_easy_setopt(curl_handle, CURLOPT_HTTPAUTH,
                                           mAuthentication->getAuthenticationMethods());
                if (resCurl != CURLE_OK) {
                    closeAll();
                    this->mError = curl_easy_strerror(resCurl);
                    throw std::runtime_error(this->mError);
                }
                resCurl = curl_easy_setopt(curl_handle, CURLOPT_USERPWD,
                                           mAuthentication->getValue().c_str());
                if (resCurl != CURLE_OK) {
                    closeAll();
                    this->mError = curl_easy_strerror(resCurl);
                    throw std::runtime_error(this->mError);
                }
            }
        }
        virtual void __setUserAgent() {
            // curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, ZAPHODHAND_USER_AGENT);
        }
        virtual void __setHttpContent() {
            if (this->mHttpContent != nullptr) {
                curl_easy_setopt(curl_handle, CURLOPT_UPLOAD, 1L);
                curl_easy_setopt(curl_handle, CURLOPT_READFUNCTION, read_data);
                curl_easy_setopt(curl_handle, CURLOPT_READDATA, this);
                curl_easy_setopt(curl_handle, CURLOPT_INFILESIZE_LARGE,
                                 (curl_off_t) mHttpContent->size());

                // curl_easy_setopt(curl_handle, CURLOPT_POST, 1L);

                // curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, 0L);
                // curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS,NULL);

                // curl_easy_setopt(curl_handle, CURLOPT_HTTP_VERSION,
                // CURL_HTTP_VERSION_1_1);
                // curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS,
                // "token=scasazzalaprovincia&SCM_PATH=https://github.com/t2rdirienzo/myjenkinstest.git&SCM_BRANCH=master&MAVEN_BUILD_OPTION=-Pthetest&PORTAL_USER=gallo");
                // curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, (long)42);
            }
        }
        virtual void __setConnectionTimeOut() {
            if (this->mConnectionTimeout != -1) {
                resCurl = curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT,
                                           this->mConnectionTimeout);
                if (resCurl != CURLE_OK) {
                    closeAll();
                    this->mError = curl_easy_strerror(resCurl);
                    throw std::runtime_error(this->mError);
                }
            }
        }
        virtual void __setTimeoutTransfert() {
            if (this->mTimeoutTrasfertMs != -1) {
                resCurl = curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT_MS,
                                           this->mTimeoutTrasfertMs);
                if (resCurl != CURLE_OK) {
                    closeAll();
                    this->mError = curl_easy_strerror(resCurl);
                    throw std::runtime_error(this->mError);
                }
            }
        }
        virtual void __setHttpContentHeaders() {
            curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, this);
            curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, header_data);
        }
        virtual void __setFailOnHttpError() {
            resCurl = curl_easy_setopt(curl_handle, CURLOPT_FAILONERROR, 0L);
            if (resCurl != CURLE_OK) {
                closeAll();
                this->mError = curl_easy_strerror(resCurl);
                throw std::runtime_error(this->mError);
            }
        }
        virtual void __setHeaders() {
            if (slist != nullptr) curl_slist_free_all(slist);

            if (this->mUri != nullptr) {
                for (auto &U : this->mUri->getHeaders()) {
                    slist = curl_slist_append(slist, U.c_str());
                }

                if (slist != nullptr) {
                    resCurl = curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, slist);
                    if (resCurl != CURLE_OK) {
                        closeAll();
                        this->mError = curl_easy_strerror(resCurl);
                        throw std::runtime_error(this->mError);
                    }
                }
            }
        }
        virtual void __setPerform() {
            resCurl = curl_easy_perform(curl_handle);

            if (resCurl == CURLE_ABORTED_BY_CALLBACK) {
            } else if (resCurl != CURLE_OK && resCurl != CURLE_HTTP_RETURNED_ERROR) {
                closeAll();
                this->mError = curl_easy_strerror(resCurl);
                throw std::runtime_error(this->mError);
            }
        }
        virtual void __setGetHttpCode() {
            if (this->mUri != nullptr) {
                long httpCode{-1};
                resCurl =
                        curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &httpCode);
                if (resCurl != CURLE_OK) {
                    closeAll();
                    this->mError = curl_easy_strerror(resCurl);
                    throw std::runtime_error(this->mError);
                }
                *(this->mUri) << httpCode;
            }
        }
        virtual void __setElapsedTime() {
            curl_easy_getinfo(curl_handle, CURLINFO_TOTAL_TIME, &mElapsed);
        }
        virtual void __setTrasfertByteForSecond() {
            //		curl_easy_setopt(curl_handle, CURLOPT_LOW_SPEED_TIME, 6000L);
            //		curl_easy_setopt(curl_handle, CURLOPT_LOW_SPEED_LIMIT, 3000L);
            // OR
            // 		curl_easy_setopt(curl_handle, CURLOPT_MAX_RECV_SPEED_LARGE,
            // (curl_off_t)1000);
        }

    protected:
        void resetError() { this->mError.clear(); }
    };

}// namespace ZaphodHands

#endif// BEEBLEBROX_ZephodHand_ZephodHand_HPP
