
#ifndef S2V1_UUID_HPP
#define S2V1_UUID_HPP

#include "uuid/uuid.h"
#include <string>

namespace Beeblebrox {

    class Uuid {
    public:
        Uuid() = default;

        static std::string getTimeSafe() {
            uuid_t uuid;

            uuid_generate_time_safe(uuid);

            char uuid_str[37];// ex. "1b4e28ba-2fa1-11d2-883f-0016d3cca427" + "\0"
            uuid_unparse_lower(uuid, uuid_str);

            return std::string(uuid_str);
        }

        virtual ~Uuid() = default;

    private:
    };
}// namespace Beeblebrox
#endif /* UUID_HPP */
