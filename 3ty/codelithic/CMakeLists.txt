cmake_minimum_required(VERSION 3.5.1)
project(codelithic)
find_package(PkgConfig)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS " -Wall -fPIC")

#?????????????????
link_directories(/usr/lib/)
include_directories(include)
#?????????????????

SET(CODELITHIC_DELEGATE
        include/codelithic/delegate/Delegate.hpp
        include/codelithic/delegate/DelegateBase.hpp
        include/codelithic/delegate/MultiCastDelegate.hpp
        )

SET(CODELITHIC_THREAD
        include/codelithic/thread/InterlockedProperty.hpp
        include/codelithic/thread/SafeQueueThreadWrapper.hpp
        include/codelithic/thread/ThreadWrapper.hpp
        )

SET(CODELITHIC_UTILS
        include/codelithic/utils/color.hpp
        include/codelithic/utils/FileUtil.hpp
        include/codelithic/utils/Logfile.hpp

        include/codelithic/utils/Sha1.hpp
        src/utils/Sha1.cpp
        src/utils/StringUtil.cpp
        )


SET(CODELITHIC_CONVERT
        include/codelithic/convert/md5.h src/convert/md5.cpp
        include/codelithic/convert/sha1.h src/convert/sha1.cpp
        include/codelithic/convert/sha224.h src/convert/sha224.cpp
        include/codelithic/convert/sha256.h src/convert/sha256.cpp
        include/codelithic/convert/sha384.h src/convert/sha384.cpp
        include/codelithic/convert/sha512.h src/convert/sha512.cpp
        )

add_library(codelithic STATIC ${CODELITHIC_UTILS} ${CODELITHIC_CONVERT})
TARGET_LINK_LIBRARIES(codelithic pthread)


SET(CODELITHIC_SITE
        include/codelithic/site/PageNotFound.hpp
        include/codelithic/site/Session.hpp
        include/codelithic/site/Services.hpp
        include/codelithic/site/PageSite.hpp
        include/codelithic/site/Routes.hpp
        include/codelithic/site/Site.hpp
        include/codelithic/site/Application.hpp
        include/codelithic/site/Controller.hpp
        include/codelithic/site/codelithic_site_define.hpp
        include/codelithic/site/FCgiIO.h
        include/codelithic/site/PageParameters.hpp

        src/site
        src/site/Routes.cpp
        src/site/FCgiIO.cpp
        src/site/Session.cpp
        src/site/PageParameters.cpp
        src/site/Application.cpp
        src/site/Controller.cpp
        src/site/PageNotFound.cpp
        )

add_library(codelithic_site STATIC ${CODELITHIC_SITE})
TARGET_LINK_LIBRARIES(codelithic_site codelithic pthread fcgi++ fcgi cgicc)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

SET(CODELITHIC_WEB
        include/codelithic/web/hand.hpp
        include/codelithic/web/zaphodhand.hpp
        include/codelithic/web/zaphodhandftp.hpp
        include/codelithic/web/utils
        include/codelithic/web/utils/query.hpp
        include/codelithic/web/utils/httpcontentdockerstdin.hpp
        include/codelithic/web/utils/uri.hpp
        include/codelithic/web/utils/httpresponsefile.hpp
        include/codelithic/web/utils/auth.hpp
        include/codelithic/web/utils/Uuid.hpp
        include/codelithic/web/utils/httpresponse.hpp
        include/codelithic/web/utils/identifier.hpp
        include/codelithic/web/utils/httpcontent.hpp
        include/codelithic/web/utils/httpresponsestream.hpp
        include/codelithic/web/utils/cookie.hpp
        include/codelithic/web/utils/authuserpassword.hpp
        include/codelithic/web/utils/httpcontentstring.hpp
        include/codelithic/web/utils/httpresponsevnddockerrawstream.hpp
        include/codelithic/web/utils/define.hpp
        include/codelithic/web/utils/httpcontentfile.hpp
        include/codelithic/web/utils/httpresponsestring.hpp
        include/codelithic/web/zaphodhanddocker.hpp
        include/codelithic/web/uridocker.hpp

        src/web/uri.cpp

        )

add_library(codelithic_web STATIC ${CODELITHIC_WEB})
TARGET_LINK_LIBRARIES(codelithic_web codelithic uuid ${CURL_LIBRARIES} pthread)