#ifndef S2V1_LOGFILExx_HPP
#define S2V1_LOGFILExx_HPP

#define WITH_FILE_LOG

#ifdef WITH_FILE_LOG

#include <fstream>
#include <iostream>

#include <chrono>
#include <ctime>
#include <iomanip>

#define FILE_LOG_DEBUG(Y) Logfile(Logfile::typelog::DEBUG) << Y
#define FILE_LOG_INFO(Y) Logfile(Logfile::typelog::INFO) << Y
#define FILE_LOG_WARN(Y) Logfile(Logfile::typelog::WARN) << Y
#define FILE_LOG_ERROR(Y) Logfile(Logfile::typelog::ERROR) << Y

class Logfile {
public:
    enum class typelog { DEBUG,
                         INFO,
                         WARN,
                         ERROR };

    struct structlog {
        bool headers = true;
        typelog level = typelog::DEBUG;
    } LOGCFG;

    Logfile() { openFile(); }

    explicit Logfile(typelog type) {
        openFile();

        msglevel = type;
        if (LOGCFG.headers) {
            std::time_t t = std::time(0);

            // http://en.cppreference.com/w/cpp/chrono/time_point
            std::chrono::system_clock::time_point now =
                    std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);

            operator<<("[" + getLabel(type) + "] ");
            operator<<(std::put_time(std::localtime(&now_c), "%F %T"));
            //        operator<<((std::localtime(&now_c), "%F %T"));
            operator<<(" ");
        }
    }

    ~Logfile() {
        if (opened) {
            myfile << std::endl;
        }
        opened = false;
        myfile.close();
    }

    template<class T>
    Logfile &operator<<(const T &msg) {
        if (msglevel >= LOGCFG.level) {
            myfile << msg;
            opened = true;
        }

        myfile.flush();

        return *this;
    }

private:
    void openFile() {
        // todo: should be put externam
        myfile.open("/tmp/kungfu_server.log", std::ios::out | std::ios::app);
    }

    std::ofstream myfile;

    bool opened = false;
    typelog msglevel = typelog::DEBUG;

    inline std::string getLabel(typelog type) {
        std::string label;
        switch (type) {
            case typelog::DEBUG:
                label = "DEBUG";
                break;
            case typelog::INFO:
                label = "INFO ";
                break;
            case typelog::WARN:
                label = "WARN ";
                break;
            case typelog::ERROR:
                label = "ERROR";
                break;
        }
        return label;
    }
};

#else
#define FILE_LOG_DEBUG(Y)
#define FILE_LOG_INFO(Y)
#define FILE_LOG_WARN(Y)
#define FILE_LOG_ERROR(Y)
#endif

#endif