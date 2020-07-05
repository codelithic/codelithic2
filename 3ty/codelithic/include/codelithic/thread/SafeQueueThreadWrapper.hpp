
#pragma once
#include <codelithic/delegate/Delegate.hpp>
#include <codelithic/thread/ThreadWrapper.hpp>
#include <queue>

namespace codelithic {
    template<typename PARAM>
    class SafeQueueThreadWrapper : public ThreadWrapper {
    public:
        SafeQueueThreadWrapper() {
            ThreadWrapper::InitializeSyncPointDelegate(syncDelegate, this);
        }

        ~SafeQueueThreadWrapper() {
            std::lock_guard<std::mutex> lock(queueMutex);
            while (!queue.empty()) {
                auto element = queue.front();
                queue.pop();
                delete element;
            }
        };

        using SyncDelegate = codelithic::delegate<void(bool)>;
        using TaskDelegate = codelithic::delegate<void(SyncDelegate &, PARAM *)>;

        void Enqueue(TaskDelegate &task, PARAM *argument) {
            std::lock_guard<std::mutex> lock(queueMutex);
            queue.push(new QueueItem(task, argument));
            queueCondition.notify_one();
        }

        enum class AbortDepth {
            currentTask,
            entierThread,
        };
        void Abort(AbortDepth depth = AbortDepth::currentTask) {
            SetAbort(true, depth == AbortDepth::currentTask);
        }

        codelithic::delegate<void(const PARAM *)> CurrentTaskAborted;

    protected:
        virtual void OnCurrentTaskAborted(const PARAM *current) {
            if (!CurrentTaskAborted.isNull()) CurrentTaskAborted(current);
        }

    protected:
        void Body() override final {
            while (true) {
                QueueItem task;
                getTask(task);
                try {
                    (*task.delegate)(syncDelegate, task.parameter);
                } catch (ShallowThreadAbortException &) {
                    SetAbort(false);
                    OnCurrentTaskAborted(task.parameter);
                }
            }
        }

    private:
        SyncDelegate syncDelegate;

        struct QueueItem {
            QueueItem() = default;
            QueueItem(TaskDelegate &del, PARAM *param) {
                delegate = &del;
                parameter = param;
            }
            TaskDelegate *delegate;
            PARAM *parameter;
        };

        std::queue<QueueItem *> queue;
        std::mutex queueMutex;
        std::condition_variable queueCondition;

        void getTask(QueueItem &itemCopy) {
            std::unique_lock<std::mutex> ul(queueMutex);
            queueCondition.wait(ul, [=] { return queue.size() > 0; });
            auto front = queue.front();
            itemCopy.delegate = front->delegate;
            itemCopy.parameter = front->parameter;
            queue.pop();
            delete front;
        }
    };
}// namespace codelithic