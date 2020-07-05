
#ifndef LSD_SESSION_H
#define LSD_SESSION_H

#define SESSION_SID "lsd_session"
#define USER_SIZE 50

#include <cgicc/Cgicc.h>

#include <map>
#include <sstream>


namespace codelithic {

    namespace site {

        class CookieJar {
        public:
            std::vector<cgicc::HTTPCookie> getJar() { return cookieJar; };

            void addCookie(cgicc::HTTPCookie ck) { cookieJar.push_back(ck); }

            void findValue(std::string name, std::string &buffer);

        private:
            std::vector<cgicc::HTTPCookie> cookieJar;
        };

        //----------------------------------------------------

        class SessionPersistance {
            SessionPersistance() {}

        protected:
            std::string m_id;

        public:
            SessionPersistance(std::string id) : m_id{id} {}

            virtual void write(std::map<std::string, std::string> &map) = 0;

            virtual void read(std::map<std::string, std::string> &mapBuffer) = 0;

            virtual void remove() = 0;
        };
        //----------------------------------------------------

        //----------------------------------------------------

        class SessionFilePersistance : SessionPersistance {
            std::string basePath;

        protected:
            //        virtual void write(std::ostream &out, std::map<std::string,
            //        std::string >& map); virtual void read(std::istream &in,
            //        std::map<std::string, std::string >& mapBuffer);
        public:
            SessionFilePersistance(std::string id) : SessionPersistance(id) {
                basePath = "/tmp/";
            }

            virtual void write(std::map<std::string, std::string> &map) override;

            virtual void read(std::map<std::string, std::string> &mapBuffer) override;

            virtual void remove() override;
        };

        //----------------------------------------------------

        class Session {
        public:
            Session(){

            };

            Session(cgicc::Cgicc &CGI) {
                isStarted = false;
                getNUMCookie(CGI.getEnvironment().getCookieList());
            }

            std::vector<cgicc::HTTPCookie> getCookieJar() { return cookieJar.getJar(); }

            void deleteSession(cgicc::HTTPCookie *_cookie);

            void deleteSession();

            void startSession();

            void writeSession(void);

            std::string operator()(const std::string &val) { return session[val]; }

            template<typename T>
            void operator()(const std::string &key, const T &value) {
                std::stringbuf strBuffer;
                std::ostream os(&strBuffer);
                os << value;
                //    session.insert(std::pair<std::string, std::string>(key, strBuffer.str()));
                session[key] = std::string(strBuffer.str());
                // // session[key]=value;
                // std::istringstream iss (value);
                // session[key]=value;
            }

            // std::string &operator[](std::string);

            void dump(std::ostream &o) {
                for (auto &T : session) {
                    o << T.first << ":" << T.second << std::endl;
                }
            }

            virtual ~Session();

            std::map<std::string, std::string> &share() { return session; }

        private:
            unsigned long setMaxAge();

            Session(const Session &other)
                : session(other.session),
                  cookieJar(other.cookieJar),
                  isStarted(other.isStarted) {}

            std::map<std::string, std::string> session;

            CookieJar cookieJar;

            bool isCookieValid(cgicc::HTTPCookie *);

            void prepareSessionCookie(cgicc::HTTPCookie *);

            void getNUMCookie(std::vector<cgicc::HTTPCookie> pCookieVector);

            std::string generateUnicCookie();

            bool isStarted = false;
        };
    }// namespace site
}// namespace codelithic
#endif// LSD_SESSION_H
