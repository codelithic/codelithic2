#ifndef BEEBLEBROX_ZephodHand_ZephodHandFtp_HPP
#define BEEBLEBROX_ZephodHand_ZephodHandFtp_HPP

#include "uridocker.hpp"
#include "zaphodhand.hpp"

namespace ZaphodHands {

    class ZaphodHandFtp : public ZaphodHands::ZaphodHand {
    public:
    protected:
        virtual bool addCurlOptions() { return false; }

    private:
    };

}// namespace ZaphodHands

#endif// BEEBLEBROX_ZephodHand_ZephodHandFtp_HPP