#include "concurrent_queue.hpp"
#include <future>
#include <queue>


namespace detail {

template<typename Future, typename F, typename T>
    void set_value(std::promise<Future> &p, F &f, T &t)
    { p.set_value(f(t)); }

template<typename F, typename T>
    void set_value(std::promise<void> &p, F &f, T &t)
    { f(t); p.set_value(); }

}

template<class T>
class concurrent {

private:
    mutable T _t;
    mutable concurrent_queue<std::function<void()>> _q;
    bool _done;
    std::thread _thd;

public:
    concurrent(T t=T{}):
        _t(t), _done(false), _thd([=]{ while(!this->_done) _q.pop()(); }) {}

    ~concurrent() { _q.push([=]{ _done = true; }); _thd.join(); }

    template<typename F>
    auto operator()(F f) const -> std::future<decltype(f(_t))>
    {
        auto p = std::make_shared<std::promise<decltype(f(_t))> >();
        auto ret = p->get_future();
        _q.push([=]{ 
            try        { detail::set_value(*p, f, _t); }
            catch(...) { p->set_exception(std::current_exception()); }
        });
        return ret;
    }
};
