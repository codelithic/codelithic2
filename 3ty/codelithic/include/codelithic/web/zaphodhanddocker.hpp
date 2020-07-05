#ifndef BEEBLEBROX_ZephodHand_ZephodHandDocker_HPP
#define BEEBLEBROX_ZephodHand_ZephodHandDocker_HPP

#include "uridocker.hpp"
#include "zaphodhand.hpp"

namespace ZaphodHands {

    class ZaphodHandDocker : public ZaphodHands::ZaphodHand {
    public:
    protected:
        bool addCurlOptions() override {
            auto dockerUri = dynamic_cast<DockerUri *>(mUri);
            if (dockerUri) {
                // std::cout << "IN addCurlOptions: >>" << dockerUri->getSocketFile() <<
                // "<<  \n";
                resCurl = curl_easy_setopt(curl_handle, CURLOPT_UNIX_SOCKET_PATH,
                                           dockerUri->getSocketFile().c_str());
                if (resCurl != CURLE_OK) {
                    return true;
                }
            }
            return false;
        }

    private:
    };

}// namespace ZaphodHands

#endif