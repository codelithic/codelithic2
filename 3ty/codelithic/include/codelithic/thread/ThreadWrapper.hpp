
#pragma once
#include <codelithic/delegate/Delegate.hpp>
#include <condition_variable>
#include <exception>
#include <thread>

namespace codelithic {

    class ThreadWrapper {
    public:
        void test() {}

        void Start(bool background = false) {
            thread = std::thread(&ThreadWrapper::BodyWrapper, this);
            // another way to do it:
            // std::thread tmp(&ThreadWrapper::BodyWrapper, this);
            // thread.swap(tmp);
            if (background) thread.detach();
        }

        void Join() {
            if (thread.joinable())
                thread.join();
            else {
                std::unique_lock<std::mutex> ul(joinMutex);
                joinEvent.wait(ul, [=] { return done; });
            }
        }

        void PutToSleep() {
            std::lock_guard<std::mutex> lock(stateMutex);
            state = Action::sleep;
            stateEvent.notify_one();
        }

        void WakeUp() {
            std::lock_guard<std::mutex> lock(stateMutex);
            state = Action::wakeup;
            stateEvent.notify_one();
        }

        void Abort() { SetAbort(true); }

    private:
        enum class Action {
            sleep = 0,
            wakeup = 1,
            shallowAbort = wakeup | 2,
            deepAbort = wakeup | 4
        };

        class ThreadAbortException : std::exception {};

        void BodyWrapper() {
            try {
                done = false;
                Body();
            } catch (ThreadAbortException &) {
                Aborted();
            } catch (std::exception &ex) {
                ExceptionCaught(ex);
            } catch (...) {
                UnknownExceptionCaught();
            }
            std::lock_guard<std::mutex> lock(joinMutex);
            done = true;
            joinEvent.notify_all();
            Terminated();
        }

        std::thread thread;
        std::condition_variable stateEvent, joinEvent;
        std::mutex stateMutex, joinMutex;
        Action state = Action::wakeup;
        bool done = false;

    protected:
        void SyncPoint(bool autoReset = false) {
            std::unique_lock<std::mutex> ul(stateMutex);
            stateEvent.wait(ul, [=] {
                auto result = ((int) state & (int) Action::wakeup) > 0;
                if (state == Action::deepAbort)
                    throw ThreadAbortException();
                else if (state == Action::shallowAbort)
                    throw ShallowThreadAbortException();
                if (autoReset) state = Action::sleep;
                return result;
            });
        }

        class ShallowThreadAbortException : ThreadAbortException {};

        void SetAbort(bool set, bool shallow = true) {
            std::lock_guard<std::mutex> lock(stateMutex);
            if (set) {
                if (shallow)
                    state = Action::shallowAbort;
                else
                    state = Action::deepAbort;
            } else
                state = Action::wakeup;
            stateEvent.notify_one();
        }

        static void InitializeSyncPointDelegate(codelithic::delegate<void(bool)> &del,
                                                ThreadWrapper *instance) {
            del = codelithic::delegate<void(
                    bool)>::create<ThreadWrapper, &ThreadWrapper::SyncPoint>(instance);
        }

        virtual void Body() = 0;
        virtual void Aborted() {}
        virtual void Terminated() {}
        virtual void ExceptionCaught(std::exception &exception) {}
        virtual void UnknownExceptionCaught() {}
    };
}// namespace codelithic