#ifndef SRC_CALLBACK_HPP
#define SRC_CALLBACK_HPP

#include <functional>
#include <map>

namespace tasker
{

template <typename T>
class callback
{
private:
    using function_t = std::function<int()>;
    std::map<T, function_t> table;

public:
    function_t &operator[](const T &key)
    {
        return table[key];
    }

    bool exists(const T &key) const
    {
        return table.find(key) != table.end();
    }

    int call(const T &key) const
    {
        return table.at(key)();
    }
};

}; // namespace tasker

#endif /* SRC_CALLBACK_HPP */
