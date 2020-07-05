
#include "codelithic/site/PageNotFound.hpp"

namespace codelithic {
    namespace site {

        PageNotFound::PageNotFound() {}

        PageNotFound::PageNotFound(const char *message) : msg_(message) {}

        PageNotFound::PageNotFound(const std::string &message) : msg_(message) {}

        PageNotFound::~PageNotFound() {}

    }// namespace site
}// namespace codelithic