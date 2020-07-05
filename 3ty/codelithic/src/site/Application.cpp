#include <string>
#include <unistd.h>

#include "codelithic/site/Application.hpp"
#include "codelithic/site/FCgiIO.h"
#include "codelithic/site/PageNotFound.hpp"
#include "codelithic/site/PageParameters.hpp"
#include "codelithic/site/Session.hpp"
#include <cgicc/HTMLClasses.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTTPRedirectHeader.h>
#include <cgicc/HTTPStatusHeader.h>
//#include "codelithic/site/define.hpp"

#ifndef lint
static const char rcsid[] =
        "$Id: threaded.c,v 1.9 2001/11/20 03:23:21 robs Exp $";
#endif /* not lint */

#include <fcgi_config.h>
#include <sys/types.h>

#ifdef HAVE_UNISTD_H

#include <unistd.h>

#endif

#include <fcgiapp.h>

using namespace std;
using namespace cgicc;

namespace codelithic {
    namespace site {

        struct ThreadParameters {
            void *mApplication{nullptr};

            // TODO: change in char* with
            std::string mName{""};

            void setName(std::string name, int val) {
                mName = name + "_" + std::to_string(val);
            }
        };

        void *threadloop(void *tParams) {
            int rc;
            auto theTRparams = static_cast<ThreadParameters *>(tParams);
            void *elements{theTRparams->mApplication};
            FCGX_Request request;
            FCGX_InitRequest(&request, 0, 0);
            // void* V{nullptr};
            //    long count = 50000;

            for (;;) {
                static pthread_mutex_t accept_mutex = PTHREAD_MUTEX_INITIALIZER;
                pthread_mutex_lock(&accept_mutex);
                rc = FCGX_Accept_r(&request);

                // https://gist.github.com/shuge/cc38cdf6fa9de849f634

                if (rc >= 0) {
                    FCgiIO IO(request);
                    try {
                        //   FCgiIO IO(request);
                        Cgicc CGI(&IO);
                        pthread_mutex_unlock(&accept_mutex);
                        PageParameters parameters(CGI, dynamic_cast<std::ostream *>(&IO),
                                                  &(IO.err()));
                        parameters.setThreadName(theTRparams->mName);
                        parameters.setApplication(elements);
                        parameters.setFCgiIO(&IO);
                        ((Application *) elements)
                                ->getController()
                                ->parseRequest(request, parameters);

                        // V = ((Application*)elements)->getController()->parseRequest(request,
                        // parameters); if (V != nullptr) {
                        //     // HTTPHeader* header = V->getContentHeader();
                        //     // for (cgicc::HTTPCookie &coockie :
                        //     parameters.getSession().getCookieJar()) {
                        //     //     header->setCookie(coockie);
                        //     // }
                        //     // header->render(IO);
                        //     // V->render(IO);
                        // } else {
                        // }
                    } catch (PageNotFound &c) {
                        //                        IO << HTTPHTMLHeader();
                        IO << cgicc::HTTPStatusHeader(404, "");
                        IO << h1() << "404 uu" << getpid() << " "
                           << " call before next reset reset " << h1() << "\n";
                    } catch (const exception &c) {
                        std::cerr << c.what() << std::endl;
                    }
                    // if (V != nullptr) {
                    //     delete V;
                    //     V = nullptr;
                    // }
                    FCGX_Finish_r(&request);
                } else {
                    pthread_mutex_unlock(&accept_mutex);
                    break;
                }
            }
            return nullptr;
        }

        int Application::starThread(int argc, const char **argv, char **envp,
                                    void *applicationParams, bool keepAlive) {
            int i = 0;
            // std::function<void*(void* a)> p=std::bind(&Application::threadloop, this,
            // std::placeholders::_1);
            FCGX_Init();

            if (this->nThread > 0) {
                id = new /*(nothrow)*/ pthread_t[this->nThread];
                for (i = 1; i < nThread; i++) {
                    auto p = new ThreadParameters();
                    p->mApplication = (void *) this;
                    p->setName("Thread_", i);
                    pthread_create(&id[i], nullptr, threadloop, (void *) p /*this*/);
                }
            }
            if (keepAlive) {
                auto p = new ThreadParameters();
                p->mApplication = (void *) this;
                p->setName("MainThread_", i);
                threadloop(p);
            }
            return 0;
        }

        int Application::starThread(int argc, const char **argv, char **envp,
                                    bool keepAlive) {
            return starThread(argc, argv, envp, nullptr, keepAlive);
        }

        void Application::dispose() {}

        Application::~Application() { this->dispose(); }

        Controller *Application::getController() { return &controller; }

        Application::Application(int nThread) {
            if (nThread < 0) {
                nThread = 0;
            }

            this->nThread = nThread;
        }
    }// namespace site
}// namespace codelithic