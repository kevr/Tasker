#ifndef SRC_TUI_WINDOW_HPP
#define SRC_TUI_WINDOW_HPP

#include "errors.hpp"
#include "ncurses.hpp"
#include "object.hpp"
#include "utility.hpp"
#include <memory>

namespace tasker::tui
{

template <typename CI>
class basic_window : public object
{
protected:
    CI *ncurses = nullptr;
    WINDOW *m_win = nullptr;

public:
    basic_window() = default;

    basic_window(CI &ncurses)
        : ncurses(&ncurses)
    {
    }

    // Defaulted move/copy constructors and assignments
    basic_window(basic_window &&p) noexcept = default;
    basic_window &operator=(basic_window &&p) noexcept = default;
    basic_window(const basic_window &p) noexcept = default;
    basic_window &operator=(const basic_window &p) noexcept = default;

    // Defaulted destructor override.
    virtual ~basic_window() noexcept = default;

    // Utility functions
    operator bool() const noexcept
    {
        return m_win != nullptr;
    }

    WINDOW *handle() const noexcept
    {
        return m_win;
    }
};

template <typename CI>
class root_window : public basic_window<CI>
{
public:
    using basic_window<CI>::basic_window;

    // Defaulted destructor override.
    ~root_window() noexcept
    {
        end();
    }

    int init() noexcept final override
    {
        if (!this->ncurses) {
            return error(ERROR, "root_window::ncurses was null during init()");
        }

        this->m_win = this->ncurses->initscr();
        if (this->m_win == nullptr) {
            return error(ERROR_INITSCR, "initscr() returned a nullptr");
        }

        return OK;
    }

    // Virtual overrides
    int refresh() noexcept final override
    {
        if (!*this) {
            return error(ERR,
                         "root_window::refresh() called on a null handle");
        }

        return this->ncurses->refresh();
    }

    int end() noexcept final override
    {
        if (this->m_win) {
            auto rc = this->ncurses->endwin();
            this->m_win = nullptr;
            return rc;
        }
        return ERR;
    }
};

}; // namespace tasker::tui

#endif /* SRC_TUI_WINDOW_HPP */
