
#ifndef CODELITHIC_SAFEQUEUE_H
#define CODELITHIC_SAFEQUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

namespace codelithic {

    template<class T>
    class SafeQueue {
    public:
        SafeQueue() : q(), m(), c(), _exit(false) {}

        ~SafeQueue() {}

        void enqueue(T *t) {
            std::lock_guard<std::mutex> lock(m);
            q.push(t);
            c.notify_one();
        }

        T *dequeue() {
            std::unique_lock<std::mutex> lock(m);
            while (q.empty()) {
                // http://www.cplusplus.com/reference/type_traits/is_base_of/
                // http://www.cplusplus.com/reference/type_traits/is_convertible/
                // http://www.cplusplus.com/reference/type_traits/is_same/
                if (_exit) return nullptr;

                c.wait(lock);
            }

            if (_exit) {
                return nullptr;
            }

            T *val = q.front();
            q.pop();
            return val;
        }

        void exit() {
            _exit = true;
            c.notify_one();
        }

    private:
        bool _exit;
        std::queue<T *> q;
        mutable std::mutex m;
        std::condition_variable c;
    };

    template<class T>
    class ServerSafeQueue {
    private:
        SafeQueue<T> queue{};
        std::thread *_server_thread{nullptr};
        bool _run{false};

        void loop() {
            //		std::cout << "Server QUEUE started" << std::endl;
            while (_run) {
                dequeueSafeQueue(queue.dequeue());
            }
        }

    public:
        ServerSafeQueue() = default;

        void startSafeQueue() {
            _run = true;
            _server_thread = new std::thread([this] { this->loop(); });
        }

        void stopSafeQueue() {
            _run = false;
            // queue.enqueue(nullptr);
            queue.exit();
            // enqueue(nullptr); //send "close" and parse all other message before
            // close;
            if (_server_thread != nullptr) {
                _server_thread->join();
                delete (_server_thread);
                _server_thread = NULL;
                //			std::cout << "Server Queue Stopped" <<
                //std::endl;
            }
        }

        virtual void dequeueSafeQueue(T *t) = 0;

        virtual ~ServerSafeQueue() = default;

    protected:
        void enqueueSafeQueue(T *t) { queue.enqueue(t); };
    };
}// namespace codelithic

#endif /* CODELITHIC_SAFEQUEUE_H */
