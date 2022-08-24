#ifndef SRC_UTILITY_HPP
#define SRC_UTILITY_HPP

#include <iostream>

template <typename T>
int _error(int rc, T &&arg)
{
    std::cerr << arg << std::endl;
    return rc;
}

template <typename T, typename... Args>
int _error(int rc, T &&arg, Args &&...args)
{
    std::cerr << arg;
    return _error(rc, std::forward<Args>(args)...);
}

template <typename... Args>
int error(int rc, Args &&...args)
{
    std::cerr << "error: ";
    return _error(rc, std::forward<Args>(args)...);
}

template <typename T>
int error(int rc, T &&arg)
{
    std::cerr << "error: ";
    return _error(rc, std::move(arg));
}

#endif /* SRC_UTILITY_HPP */
