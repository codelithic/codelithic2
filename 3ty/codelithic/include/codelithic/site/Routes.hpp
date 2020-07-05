
#ifndef LSD_DSADFD_HGFGHF_sdfsdROUTES_H
#define LSD_DSADFD_HGFGHF_sdfsdROUTES_H

#include "PageParameters.hpp"
#include "codelithic/delegate/Delegate.hpp"

#include <functional>
#include <iostream>

#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>


#define fT                                                     \
    std::function<void *(std::vector<std::string> * pathParam, \
                         Codelithic::PageParameters & params)>
#define rT std::function<bool(const std::string &path)>
typedef bool (*LRoute)(const std::string &);
#define _CODELITHIC_DELEGATE_ROUTE \
<bool(const std::string& path,const std::string& routePath)>
#define _CODELITHIC_DELEGATE_ROUTE_REGEX \
<bool(const std::string& path,const std::string& routePath,std::vector<std::string> *params)>
#define CODELITHIC_DELEGATE_CONTENT \
<void*(std::vector<std::string>*,codelithic::site::PageParameters&)>

namespace codelithic {

    namespace site {
        struct Route {
            bool shouldBeLogged{false};
            const char *redirectTo{nullptr};
            std::string myUri{""};
            delegate _CODELITHIC_DELEGATE_ROUTE dRoute;
            delegate CODELITHIC_DELEGATE_CONTENT dContent;

        private:
        };

        class Routes {
        public:
            Routes();

            Route *parse(const std::string &s, char delim,
                         std::vector<std::string> *params);

            std::vector<Route *> routes{};

            virtual ~Routes();

        private:
            void split(const std::string &s, char delim, std::vector<std::string> &elems);

            std::vector<std::string> split(const std::string &s, char delim);
        };
    }// namespace site
}// namespace codelithic
#endif /* ROUTES_H */
