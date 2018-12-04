#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <stdexcept>

namespace gk {
  namespace sys {
    namespace process {

      /**
       * @brief The thread_pool class
       * @see https://github.com/progschj/ThreadPool/blob/master/ThreadPool.h
       */
      class thread_pool
      {
       public:
        thread_pool( size_t );
        ~thread_pool();

       public:
        template<class F, class ... Args>
        auto enqueue( F&& f, Args&& ... args )
        ->std::future<typename std::result_of<F( Args ... )>::type>;

       private:
        // need to keep track of threads so we can join them
        std::vector<std::thread> workers;
        // the task queue
        std::queue<std::function<void()>> tasks;

       private:
        // synchronization
        std::mutex queue_mutex;
        std::condition_variable condition;
        bool stop;
      };

    }
  }
}

// ---- ---- ---- ----

#include <gk/sys/process/thread_pool.ipp>
