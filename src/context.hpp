#ifndef SRC_CONTEXT_HPP
#define SRC_CONTEXT_HPP

#include "callback.hpp"
#include "ncurses.hpp"

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

    int call_keybind(const T &key) const
    {
        return keybinds.call(key);
    }

    operator bool() const
    {
        return running;
    }

    template <typename Config>
    void bind_keys(Config &conf)
    {
        keybinds[conf.template get<char>("keybindings.quit")] =
            [this]() -> int {
            this->running = false;
            return OK;
        };
    }

    template <typename Pointer, typename Func>
    void bind_pointer(char key, Pointer p, Func fn)
    {
        keybinds[key] = [p, fn] {
            fn(p);
        };
    }
};

template <typename CI, typename Context>
void input_loop(CI &ncurses, Context &ctx)
{
    // TUI input logic, wait-state until quit key is pressed
    int ch;
    while (ctx && (ch = ncurses.getchar())) {
        if (ctx.keybind_exists(ch)) {
            ctx.call_keybind(ch);
        }
    }
}

}; // namespace tasker

#endif /* SRC_CONTEXT_HPP */
