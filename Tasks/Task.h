#ifndef TASK_H
#define TASK_H

#include <atomic>
#include <memory>
#include <functional>
#include <future>
#include <iostream>

enum TaskStatus {
    INACTIVE,
    ACTIVE,
    PAUSED,
    FINISHED,
    ERROR
};

// Simplified example from:
// https://github.com/KjellKod/concurrent/blob/master/src/concurrent.hpp
template<typename Call, typename Args>
   auto call(Call func, Args args...) const
     -> std::future<typename std::result_of::type> {
 
   typedef typename std::result_of::type result_type;
   typedef std::packaged_task task_type;
 
   // weak compiler support for expanding parameter pack in a lambda. std::function
   // is the work-around. With better compiler support it can be changed to
   // something similar to:
   //       auto bgCall = [&, args...]{ return (_worker.*func)(args...); };
   //
   auto bgCall = std::bind(func, _worker.get(), std::forward(args)...);
   task_type task(std::move(bgCall));
   std::future result = task.get_future();
 
  /* put the task in a job queue for the
     thread(s) to execute */
 
   return result;
}

template<typename _funcT, typename... _argsT>
class Task {

typedef typename std::result_of<_funcT(_argsT...)>::type _resT;
typedef typename std::result_of<std::async(std::launch, _funcT, _argsT...)>::type _applyT;


public:
    Task(bool pausable, bool infinity, _funcT func, _argsT... args) : 
        _func(func),
        _args(std::launch::async, func, args...)
    {
        _progress = -1;
        _pausable = pausable;
        //_bind = std:bind(func, args...)
    };
    virtual ~Task() {};

    void start(){
        //std::invoke_result_t<std::async, std::launch, _funcT, _argsT...);
        //std::apply(std::async, _args);
        //_future = run(this, std::async, _args);
    };

    void setProgress(double val) {if (val>_progress) _progress = val > 100 ? 100 : val < 0 ? 0 : val;};
    double getProgress() {return _progress;};

private:
    template<typename Function, typename Tuple, size_t ... I>
    auto run(Function f, Tuple t, std::index_sequence<I ...>)
    {
         return f(std::get<I>(t) ...);
    }

    template<typename Function, typename Tuple>
    auto run(Function f, Tuple t)
    {
        static constexpr auto size = std::tuple_size<Tuple>::value;
        return run(f, t, std::make_index_sequence<size>{});
    }

    std::atomic<bool> _pausable;
    std::atomic<double> _progress;

    _funcT _func;
    std::tuple<std::launch, _funcT, _argsT...> _args;
    std::future<_resT> _future;
};

#endif /* TASK_H */

//typedef typename result_of<_funcT(Task, _argsT...)>::type _resT;