
#ifndef LSD_DATEUTIL_H
#define LSD_DATEUTIL_H

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

// http://stackoverflow.com/questions/16357999/current-date-and-time-as-string

namespace codelithic {

    class DateUtil {
    public:
        static std::string getNow(std::string format = "%d-%m-%Y %H-%M-%S") {

            // http://en.cppreference.com/w/cpp/io/manip/put_time
            std::time_t t = std::time(nullptr);
            // std::tm tm = *std::localtime(&t);

            std::ostringstream oss;
            // fixed in gcc5!!!!
            // oss << std::put_time(&tm, format.c_str());

            char mbstr[100];
            if (std::strftime(mbstr, sizeof(mbstr), format.c_str(), std::localtime(&t))) {
                oss << mbstr;
            }

            return oss.str();
        }
    };
}// namespace codelithic

#endif// LSD_DATEUTIL_H
