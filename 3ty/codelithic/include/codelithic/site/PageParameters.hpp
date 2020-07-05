#ifndef PAGEPARAMETERS_H
#define PAGEPARAMETERS_H

#include <iostream>
#include <string>

#include "Session.hpp"

#include <cgicc/Cgicc.h>
#include <cgicc/FormEntry.h>
#include <cgicc/HTTPHeader.h>

#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTTPRedirectHeader.h>
#include <cgicc/HTTPResponseHeader.h>
#include <cgicc/HTTPStatusHeader.h>

#include "FCgiIO.h"

#define PERFEC_FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

namespace codelithic {
    namespace site {
        class PageParameters {
        public:
            explicit PageParameters(cgicc::Cgicc &cgicc, std::ostream *pOutput,
                                    std::ostream *error);

            //@deprecated
            //	explicit PageParameters(cgicc::Cgicc& CGI);

            ~PageParameters();

            Session &getSession() { return session; }

            cgicc::Cgicc *getCgicc() { return cgicc; }


            inline std::ostream *getOutput() const { return pOutput; }

            inline std::ostream *getError() const { return pError; }

            void setOutput(std::ostream *Output) { pOutput = Output; }

            void setThreadName(std::string &name) { mThreadName = name; }

            const std::string &getThreadName() const { return mThreadName; }

            inline void *getApplication() { return application; };

            void setApplication(void *application);

            void writeCookies(cgicc::HTTPHeader &header);

        public:
            // PageParameters& operator<<(cgicc::HTTPHeader& header){
            // 	if (pOutput != nullptr) {
            // 			for (cgicc::HTTPCookie &coockie : session.getCookieJar())
            // { 				header.setCookie(coockie);
            // 			}
            // 			header.render(*pOutput);
            // 	}
            // 	return *this;
            // }

            // template<>
            // PageParameters& operator<<(cgicc::HTTPHeader& header);

            template<class T>
            PageParameters &operator<<(const T &t) {
                *pOutput << t;
                return *this;
            }

            template<class T>
            PageParameters &operator<<(T &&t) {
                *pOutput << t;
                return *this;
            }

            template<class T>
            PageParameters &operator<<(T &t) {
                *pOutput << t;
                return *this;
            }

            PageParameters &operator<<(cgicc::HTTPContentHeader &t);
            PageParameters &operator<<(cgicc::HTTPContentHeader &&t);

            PageParameters &operator<<(cgicc::HTTPRedirectHeader &t);
            PageParameters &operator<<(cgicc::HTTPRedirectHeader &&t);

            //
            //  PageParameters& operator<< (cgicc::HTTPContentHeader&& header);
            //  PageParameters& operator<< (cgicc::HTTPContentHeader& header);
            ////  PageParameters& operator<< (const cgicc::HTTPContentHeader& header);


            void setFCgiIO(cgicc::FCgiIO *f) {
                if (f != nullptr) {
                    fcgiIO = f;
                }
            }

            std::string getFCgiIOEnv(const char *varName) {
                if (fcgiIO == nullptr) {
                    return "";
                }

                return fcgiIO->getenv(varName);
            }

            std::string getMethod() {
                return getCgicc()->getEnvironment().getRequestMethod();
            }

            // explicit operator std::ostream*(){
            // 	return pOutput;
            // }

            // explicit operator std::ostream&(){
            // 	return *pOutput;
            // }

            // void render(cgicc::HTTPHeader* header);
            // void render(const std::string& buffer);

            // /***
            //  * it do not render the headers
            //  *
            //  */
            // void render(ViewAbstract* view);

            // /***
            //  * it renders the headers
            //  *
            //  */
            // void renderView(ViewAbstract* view);

            void setPathInfo(const std::string &path) { PageParameters::pathInfo = path; }

            const std::string &getPathInfo() const { return pathInfo; }

        private:
            Session session;
            cgicc::Cgicc *cgicc;
            std::ostream *pOutput{nullptr};
            std::ostream *pError{nullptr};
            std::string mThreadName{""};
            void *perisistence{nullptr};
            void *application{nullptr};
            std::string pathInfo{""};

            cgicc::FCgiIO *fcgiIO{nullptr};
        };
    }// namespace site
}// namespace codelithic
#endif /* PAGEPARAMETERS_H */
