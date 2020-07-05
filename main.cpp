#include <iostream>
#include <codelithic/utils/color.hpp>
#include <codelithic/convert/md5.h>
#include <codelithic/convert/sha1.h>

#include <codelithic/web/utils/httpcontentfile.hpp>
#include <codelithic/web/utils/httpcontentstring.hpp>
#include <codelithic/web/utils/httpresponsestring.hpp>
#include <codelithic/web/utils/uri.hpp>
#include <codelithic/web/zaphodhand.hpp>

#include <wrapperlog4cpp.hpp>

auto longWay(std::string_view path_, std::string &buffer) ->long {

    ZaphodHands::ZaphodHand hand{};
    std::string path{path_};

    Beeblebrox::Uri uri(path.c_str());
    uri.followUri(true);
    uri << Beeblebrox::Uri::Method::GET;

    Beeblebrox::HttpResponseString response{};
    hand << &uri << &response;
    hand.Run();

    buffer.assign(response.getContent());
    return uri.getHttpReturnCode();

}


auto main() -> int {

//    std::cout  << COLOR_Black       << "md5 of 'grape': " << md5("grape")  << COLOR_Color_Off << std::endl;
//    std::cout  << COLOR_Black       << "sha1('grape'):" << sha1("grape") << COLOR_Color_Off << std::endl;
//    std::cout  << COLOR_Black       << "sha1('grape'):" << sha1("grape") << COLOR_Color_Off << std::endl;
//    SHA1 checksum;
//    std::cout  << COLOR_Black       << "sha1.from_file('/tmp/file'):" << checksum.from_file("/tmp/file") << COLOR_Color_Off << std::endl;
//    std::cout  << COLOR_Black       << "CONFIRM sh1sum: a3cca16dda7fc3d28fd7007afb69c025d262b4c3"  << COLOR_Color_Off << std::endl;

    std::string buffer;
    auto A=longWay("https://thenumbat.github.io/cpp-course/sdl2/08/08.html#perf",buffer);
    std::cout  << COLOR_Black   <<A  << " "    << buffer << COLOR_Color_Off << std::endl;


  CLC_LOGGER_INIZIALIZE("/home/emmet/Projects/codelithic2/3ty/logs/log.ini");
  auto logger = CLC_LOGGER("ApplicationConsole");
  CLC_LOGGER_DEBUG(logger, "Application Started");

//

    return 0;
}
