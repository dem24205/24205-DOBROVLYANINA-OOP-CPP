#ifndef TUPLEPRINTER_H
#define TUPLEPRINTER_H

#include <tuple>
#include <iostream>

template<int Index, typename Ch, typename Tr, typename ...Args>
class TuplePrinter {
public:
    static void printTuple(std::basic_ostream<Ch, Tr>& o, std::tuple<Args...> const& t) {
        TuplePrinter<Index - 1, Ch, Tr, Args...>::printTuple(o, t);
        o << std::get<Index>(t) << " ";
    }
};

template<typename Ch, typename Tr, typename ...Args>
class TuplePrinter<0, Ch, Tr, Args...> {
public:
    static void printTuple(std::basic_ostream<Ch, Tr>& o, std::tuple<Args...> const& t) {
        o << std::get<0>(t) << " ";
    }
};

template<typename Ch, typename Tr, typename ...Args>
std::basic_ostream<Ch, Tr>& operator<<(std::basic_ostream<Ch, Tr>& o, std::tuple<Args...> const& t) {
    o << "( ";
    if constexpr (sizeof...(Args) > 0) {
        TuplePrinter<sizeof...(Args) - 1, Ch, Tr, Args...>::printTuple(o, t);
    }
    return o << ")";
}

#endif