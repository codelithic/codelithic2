
#include "codelithic/site/Session.hpp"
#include "codelithic/utils/FileUtil.hpp"
#include "codelithic/utils/StringUtil.hpp"
#include <cstring>
#include <ctime>
#include <sstream>

// Session* Session::m_pInstance = nullptr;
//
// Session* Session::Instance() {
//    if (m_pInstance!=nullptr) // Only allow one instance of class to be
//    generated.
//        m_pInstance = new Session;
//
//    return m_pInstance;
//}

#define SESSION_TIMEOUT_MIN 30 * 60
#define SESSION_TIMEOUT_MS (uint64_t)(SESSION_TIMEOUT_MIN)
#define COOKIES_SECURE true
#define COOKIES_HTTPONLY true

// void SessionFilePersistance::write(std::ostream &out, std::map<std::string,
// std::string >& map) {
//    FileUtil::serialize(out, map);
//}
//
// void SessionFilePersistance::read(std::istream &in, std::map<std::string,
// std::string >& mapBuffer) {
//    FileUtil::deserialize(in,mapBuffer);
//}

namespace codelithic {
    namespace site {

        void SessionFilePersistance::remove() {
            std::string fileName = basePath + m_id;
            if (FileUtil::is_file_exist(fileName.c_str())) {
                FileUtil::remove(fileName.c_str());
            }
        }

        void SessionFilePersistance::write(std::map<std::string, std::string> &map) {
            std::string fileName = basePath + m_id;
            std::ofstream file(fileName, std::ios::binary);
            try {
                if (file && file.good()) {
                    FileUtil::serialize(file, map);
                    file.flush();
                    file.close();
                } else {
                    std::string err;
                    err.append("Impossible to write the file: ");
                    err.append(fileName);

                    // throw std::ofstream::failure("ss");
                }

            } catch (std::ofstream::failure &e) {
                std::cerr << e.what() << std::endl;

            } catch (...) {
                std::cerr << "..." << std::endl;
            }
        }

        void SessionFilePersistance::read(
                std::map<std::string, std::string> &mapBuffer) {
            std::string fileName = basePath + m_id;
            std::ifstream file(fileName);
            try {
                if (file && file.good()) {
                    FileUtil::deserialize(file, mapBuffer);
                    //					file.flush();
                    file.close();
                } else {
                    ////			std::string err;
                    ////			err.append("Impossible to write the file: ");
                    ////			err.append(fileName);
                    // throw std::ifstream::failure("ss")
                }

            } catch (std::ifstream::failure &e) {
            } catch (...) {
            }
        }

        // std::string &Session::operator[](std::string key) {
        // 	return session[key];
        // }

        void Session::deleteSession(cgicc::HTTPCookie *_cookie) {
            SessionFilePersistance sp(_cookie->getValue());
            sp.remove();
            session.clear();
        }

        void Session::deleteSession() {
            std::string buffer = "";
            // TODO:forse va salvato a livello di classe sessione senza ricercarlo ogni
            // volta!
            cookieJar.findValue(SESSION_SID, buffer);
            if (buffer.length() != 0) {
                SessionFilePersistance sp(buffer);
                sp.remove();
                session.clear();
            }
        }

        void Session::writeSession() {
            std::string buffer = "";
            // TODO:forse va salvato a livello di classe sessione senza ricercarlo ogni
            // volta!
            cookieJar.findValue(SESSION_SID, buffer);
            if (buffer.length() != 0) {
                //        std::time_t lastAcceess = std::time(nullptr);
                SessionFilePersistance sp(buffer);
                sp.write(session);
            } else {
                // un danno
            }
        }

        void Session::startSession() {
            if (!isStarted) {
                std::string cookieSessionNameValue = "";
                // TODO:forse va salvato a livello di classe sessione senza ricercarlo ogni
                // volta!
                cookieJar.findValue(SESSION_SID, cookieSessionNameValue);
                if (cookieSessionNameValue.length() != 0) {
                    SessionFilePersistance sp(cookieSessionNameValue);
                    sp.read(session);
                } else {
                    // errore... è successo un casino!!! non c'è il cookie di sessione, non so
                    // che fare
                }
                isStarted = true;
            }
        }

        void Session::getNUMCookie(std::vector<cgicc::HTTPCookie> pCookieVector) {
            if (pCookieVector.size() == 0) {
                prepareSessionCookie(nullptr);
                return;
            } else {
                std::vector<cgicc::HTTPCookie>::iterator itVectorData;
                for (itVectorData = pCookieVector.begin();
                     itVectorData != pCookieVector.end(); itVectorData++) {
                    cgicc::HTTPCookie theCookie = *(itVectorData);
                    if (theCookie.getName().compare(SESSION_SID) == 0) {
                        prepareSessionCookie(&theCookie);
                        return;
                    }
                }
                prepareSessionCookie(nullptr);
                return;
            }
        }

        void Session::prepareSessionCookie(cgicc::HTTPCookie *_cookie) {
            //	if (_cookie != nullptr)
            //		std::cerr << "FIND: " << _cookie->getValue() << " " <<
            //_cookie->getName() << " ->" << _cookie->getMaxAge() << " " <<
            //_cookie->isRemoved() << std::endl; 	else { 		std::cerr << "Cookye nullptr " <<
            //std::endl;
            //	}

            cgicc::HTTPCookie cookie;
            cookie.setName(SESSION_SID);

            if (_cookie == nullptr) {
                cookie.setValue(generateUnicCookie());
                cookie.setMaxAge(setMaxAge());
            } else {
                if (isCookieValid(_cookie)) {
                    // deleteSession(_cookie);
                    cookie.setValue(_cookie->getValue());
                    cookie.setMaxAge(setMaxAge());
                    //			std::cerr << "SETT:" << cookie.getValue() << " "
                    //<< cookie.getName() << " ->" << cookie.getMaxAge() << std::endl;
                } else {
                    // non è + valido, faccio cancellare la persistaza e creo un nuovo cookie
                    cookie.setValue(generateUnicCookie());
                    cookie.setMaxAge(setMaxAge());
                }
            }

            cookieJar.addCookie(cookie);
        }

        bool Session::isCookieValid(cgicc::HTTPCookie *_cookie) {
            //	unsigned long now = std::time(NULL);
            //	unsigned long maxAge = _cookie->getMaxAge();

            // return (maxAge > now);
            return true;
        }

        unsigned long Session::setMaxAge() {
            // std::cerr << "AGE NEW: "<< (std::time(NULL) + SESSION_TIMEOUT_MS) <<
            // std::endl;
            return (/*std::time(NULL) +*/ SESSION_TIMEOUT_MS);
        }

        // unsigned long Session::getMaxAge(){
        //
        //
        //
        //	return (std::time(NULL) + SESSION_TIMEOUT_MS);
        //}

        std::string Session::generateUnicCookie() {
            return "lsdS_" + StringUtil::randomStrind(25);
        }

        void CookieJar::findValue(std::string name, std::string &buffer) {
            for (std::vector<cgicc::HTTPCookie>::iterator it = cookieJar.begin();
                 it != cookieJar.end(); ++it) {
                if (it->getName().compare(name) == 0) {
                    buffer = it->getValue();
                    break;
                }
            }
        }

        Session::~Session() {}
    }// namespace site
}// namespace codelithic