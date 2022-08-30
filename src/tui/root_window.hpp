#ifndef SRC_TUI_ROOT_WINDOW_HPP
#define SRC_TUI_ROOT_WINDOW_HPP

#include "../errors.hpp"
#include "../utility.hpp"
#include "basic_window.hpp"
#include "tui/color.hpp"

namespace tasker::tui
{

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
            return error(ERR, "root_window::ncurses was null during init()");
        }

        this->m_win = this->ncurses->initscr();
        if (this->m_win == nullptr) {
            return error(ERROR_INITSCR, "initscr() returned a nullptr");
        }

        int x, y;
        this->ncurses->get_max_yx(this->m_win, y, x);
        if (x == -1) {
            return error(ERROR_GETMAXYX, "get_max_yx() failed");
        }
        this->dimensions(x, y);

        return OK;
    }

    virtual int draw() noexcept override
    {
        // Set a border on `root`.
        auto pair = COLOR_PAIR(theme::root_border);
        this->ncurses->wattr_enable(this->m_win, pair);
        int rc = this->ncurses->wborder(this->handle(),
                                        ACS_VLINE,
                                        ACS_VLINE,
                                        ACS_HLINE,
                                        ACS_HLINE,
                                        ACS_ULCORNER,
                                        ACS_URCORNER,
                                        ACS_LLCORNER,
                                        ACS_LRCORNER);
        this->ncurses->wattr_disable(this->m_win, pair);
        return rc;
    }

    int refresh() noexcept final override
    {
        if (!this->ncurses) {
            return error(
                ERR, "root_window::refresh() called on a null ncurses handle");
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

#endif /* SRC_TUI_ROOT_WINDOW_HPP */
