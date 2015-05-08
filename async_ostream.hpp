#ifndef __ASYNC_OSTREAM_HPP__
#define __ASYNC_OSTREAM_HPP__

#include "concurrent.hpp"
#include <iostream>
#include <tuple>

struct async_ostream{
    static concurrent<std::ostream&>& get_async_ostream(std::ostream& os){
        static concurrent<std::ostream&> _(os);
        return _;
    }
};

namespace detail{
    template<std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I == sizeof...(Tp), void>::type
    print(std::ostream& os, const std::tuple<Tp...>& t){ }

    template<std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I < sizeof...(Tp), void>::type
    print(std::ostream& os, const std::tuple<Tp...>& t){
        os << std::get<I>(t);
        print<I + 1, Tp...>(os, t);
    }
}// namespace detail

template <class ...Args>
void async_cout(const Args& ...args){

    auto params_pack = std::make_tuple(args...);

    auto& async_ostream = async_ostream::get_async_ostream(std::cout);

    async_ostream([=](std::ostream &os) {
        ///XXX This is a workaround since pack expansion
        //// is not supported in lambdas in gcc <= 4.8.2
        detail::print(os, params_pack);
    });
}

#endif /* __ASYNC_OSTREAM_HPP__ */
