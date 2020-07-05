#ifndef BEEBLEBROX_SHA1_HPP
#define BEEBLEBROX_SHA1_HPP

#include <string>

namespace codelithic {

    class sha1File {
    public:
        sha1File(std::string val);
        virtual ~sha1File();

        std::string parse();
        int parseFile(std::string &buffer);

        static std::string parseString(std::string str);

    public:
        void setBuffer(unsigned int bufferLen);

    private:
        std::string mVal{""};
        sha1File() = default;
        unsigned int bufferLen{0};
    };

}// namespace codelithic

#endif
