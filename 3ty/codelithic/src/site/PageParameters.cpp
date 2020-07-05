
#include "codelithic/site/PageParameters.hpp"

#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTTPRedirectHeader.h>
#include <cgicc/HTTPResponseHeader.h>
#include <cgicc/HTTPStatusHeader.h>

namespace codelithic {
    namespace site {

        PageParameters::PageParameters(cgicc::Cgicc &cgicc, std::ostream *pOutput,
                                       std::ostream *error)
            : session(cgicc),
              cgicc(&cgicc),
              pOutput{pOutput},
              pError{error},
              perisistence{nullptr},
              application{nullptr} {}

        //@deprecated
        //        PageParameters::PageParameters(cgicc::Cgicc &CGI) :
        //                session(CGI), cgicc(&CGI),
        //                pOutput{nullptr},perisistence{nullptr},tag{nullptr} {
        //        }

        // void PageParameters::render(const std::string& buffer){
        // 	if (pOutput != nullptr) {
        // 		*pOutput<< buffer;
        // 	}
        // }

        // void PageParameters::render(cgicc::HTTPHeader *header) {
        //       if (pOutput != nullptr) {
        //           for (cgicc::HTTPCookie &coockie : session.getCookieJar()) {
        //               header->setCookie(coockie);
        //           }
        //           header->render(*pOutput);
        //       }
        //   }

        //   void PageParameters::render(ViewAbstract *view) {
        //       if (pOutput != nullptr) {
        //           view->render(*pOutput);
        //       }
        //   }

        //   void PageParameters::renderView(ViewAbstract *view) {
        //       render(view->getContentHeader());
        //       render(view);
        //   }

        PageParameters::~PageParameters() {
            cgicc = nullptr;
            pOutput = nullptr;
        }

        //   void *PageParameters::getApplication(){
        //       return application;
        //   }

        void PageParameters::setApplication(void *application) {
            PageParameters::application = application;
        }

        void PageParameters::writeCookies(cgicc::HTTPHeader &header) {
            for (cgicc::HTTPCookie &coockie : session.getCookieJar()) {
                header.setCookie(coockie);
            }
            header.render(*pOutput);
        }

        PageParameters &PageParameters::operator<<(cgicc::HTTPContentHeader &header) {
            writeCookies(header);
            return *this;
        }

        PageParameters &PageParameters::operator<<(cgicc::HTTPContentHeader &&header) {
            writeCookies(header);
            return *this;
        }

        PageParameters &PageParameters::operator<<(cgicc::HTTPRedirectHeader &header) {
            writeCookies(header);
            return *this;
        }

        PageParameters &PageParameters::operator<<(cgicc::HTTPRedirectHeader &&header) {
            writeCookies(header);
            return *this;
        }


    }// namespace site
}// namespace codelithic
