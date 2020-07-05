
#include "codelithic/site/Routes.hpp"

#include <algorithm>

namespace codelithic {
    namespace site {

        void eraseSubStr(std::string &mainStr, const std::string &toErase) {
            size_t pos = mainStr.find(toErase);
            if (pos != std::string::npos) {
                mainStr.erase(pos, toErase.length());
            }
        }

        Routes::Routes() {}

        Route *Routes::parse(const std::string &s, char delim,
                             std::vector<std::string> *params) {
            //    std::string p{s};
            //    std::size_t found = std::string::npos;
            //    do {
            ////        std::unordered_map<std::string, Route*>::iterator y =
            ///routes.find(p);
            //        auto y = routes.find(p);
            //        if (y != routes.end()) {
            //            if (params != nullptr) {
            //                if (found != std::string::npos) {
            //                    if (s.length() - found > 1) {
            //                        split(s.substr(found + 1, s.length()), delim,
            //                        *params);
            //                    }
            //                }
            //            }
            //            return y->second;
            //        }
            //        found = p.find_last_of("/");
            //        p = p.substr(0, found);
            //    } while (found != std::string::npos);

            for (auto &route : routes) {
                //        found = (route->lRoute ? route->lRoute(s) : route->getRoute()(s));

                if (route->dRoute(s, route->myUri)) {
                    if (s.length()) {
                        // TODO: CHANHE WITH POINTER AND NOT USE STRINGS
                        std::string newPath{s};
                        eraseSubStr(newPath, route->myUri);

                        split(newPath, delim, *params);
                        // std::reverse((*params).begin(),(*params).end());
                    }
                    return route;
                }
            }

            return nullptr;
        }

        void Routes::split(const std::string &s, char delim,
                           std::vector<std::string> &elems) {
            std::stringstream ss{};
            ss.str(s);
            std::string item;
            while (std::getline(ss, item, delim)) {
                if (!item.empty()) elems.push_back(item);
            }
        }

        std::vector<std::string> Routes::split(const std::string &s, char delim) {
            std::vector<std::string> elems{};
            split(s, delim, elems);
            return elems;
        }

        Routes::~Routes() { routes.clear(); }
    }// namespace site
}// namespace codelithic