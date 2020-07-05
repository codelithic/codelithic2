
#ifndef LSD_APPLICATION_H
#define LSD_APPLICATION_H

#include "Controller.hpp"
#include "cgicc/Cgicc.h"
#include <functional>
#include <pthread.h>


#define THREAD_COUNT 20

namespace codelithic {
    namespace site {

        class Application {
        private:
            Controller controller{};

        public:
            Application() = default;
            explicit Application(int nThread);

            virtual ~Application();

            Controller *getController();

        protected:
            //        virtual Controller *createController();

        private:
            //        std::string getNUMCookie(std::vector< cgicc::HTTPCookie >
            //        pCookieVector); std::string generateUnicCookie();
            int nThread{THREAD_COUNT};
            void dispose();
            //-------------------------------thread
        private:
            //        static int counts[THREAD_COUNT];
            pthread_t *id{nullptr};
            //        void* threadloop(void* elements);
        public:
            int starThread(int argc, const char **argv, char **envp,
                           void *applicationParams, bool keepAlive = true);
            int starThread(int argc, const char **argv, char **envp,
                           bool keepAlive = true);
            //-------------------------------thread
        };
    }// namespace site
}// namespace codelithic

#endif// LSD_APPLICATION_H
