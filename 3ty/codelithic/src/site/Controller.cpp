
#include "codelithic/site/Controller.hpp"
#include "codelithic/delegate/Delegate.hpp"
#include "codelithic/site/PageNotFound.hpp"

// #include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTTPRedirectHeader.h>

#include <codelithic/site/Controller.hpp>
#include <memory>

namespace codelithic {

    namespace site {

        Controller::Controller() { routes = new Routes{}; }

#ifndef NO_FCGI

        void Controller::setRequest(FCGX_Request &request, cgicc::Cgicc &CGI) {}

#else

#endif

        void *Controller::parseRequest(FCGX_Request &request,
                                       PageParameters &pageParameter) {
            // DO NOT REMOVE    char* xxx = FCGX_GetParam(TEMPLATE_PATH,request.envp);

            const cgicc::CgiEnvironment &environment =
                    pageParameter.getCgicc()->getEnvironment();
            std::vector<std::string> V;
            pageParameter.setPathInfo(environment.getPathInfo());
            Route *R = routes->parse(pageParameter.getPathInfo(), '/', &V);

            if (R != nullptr) {
                if (R->shouldBeLogged) {
                    auto &sess = pageParameter.getSession();
                    sess.startSession();
                    if (sess("_id").empty()) {
                        std::string rTo{};
                        if (R->redirectTo != nullptr) {
                            rTo = R->redirectTo;
                        }
                        cgicc::HTTPRedirectHeader r{rTo};
                        r.render(*pageParameter.getOutput());
                        return nullptr;
                    }
                }
                return R->dContent(&V, pageParameter);
            }
            throw PageNotFound();
        }

        //         void Controller::registerPage(Page *page) {
        //             //do not remove    pages->emplace(key,page);
        //             std::vector<Route *> pageRoutes = page->getRoutes();
        //             for (Route *elem : pageRoutes) {
        // //        routes->routes.emplace(elem->routePath, elem);
        //                 routes->routes.push_back(elem);
        //             }
        //         }

        void Controller::registerRoute(Route *route) {
            routes->routes.push_back(route);
        }

        Controller::~Controller() {
            if (routes != nullptr) {
                delete routes;
                routes = nullptr;
            }
        }

        unsigned long Controller::getRoutesSize() { return routes->routes.size(); }

    }// namespace site
}// namespace codelithic