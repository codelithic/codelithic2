
#ifndef PAGENOTFOUND_HPP
#define PAGENOTFOUND_HPP

#include <exception>
#include <string>

namespace codelithic {

    namespace site {

        /*
struct PageNotFound : public std::exception
{
        const char * what () const throw ()
    {
        return "C++ Exception";
    }
};
*/

        class PageNotFound : public std::exception {
        public:
            PageNotFound();

            PageNotFound(const PageNotFound &other) { this->msg_ = other.msg_; };

            virtual ~PageNotFound();

            explicit PageNotFound(const char *message);

            explicit PageNotFound(const std::string &message);

            const char *what() const throw() override { return msg_.c_str(); }

        private:
            std::string msg_;
        };

    }// namespace site
}// namespace codelithic
#endif /* PAGENOTFOUND_HPP */
