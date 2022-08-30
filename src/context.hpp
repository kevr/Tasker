#ifndef SRC_CONTEXT_HPP
#define SRC_CONTEXT_HPP

#include "callback.hpp"

namespace tasker
{

template <typename T>
class context
{
public:
    bool running = true;
    callback<T> keybinds;

    bool keybind_exists(const T &key) const
    {
        return keybinds.exists(key);
    }

    void call_keybind(const T &key) const
    {
        return keybinds.call(key);
    }

    operator bool() const
    {
        return running;
    }

    template <typename Config>
    static void bind_keys(context<T> &ctx, Config &conf)
    {
        ctx.keybinds[conf.template get<char>("key_quit")] = [&ctx] {
            ctx.running = false;
        };
    }
};

}; // namespace tasker

#endif /* SRC_CONTEXT_HPP */
