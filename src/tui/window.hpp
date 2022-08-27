#ifndef SRC_TUI_WINDOW_HPP
#define SRC_TUI_WINDOW_HPP

#include "errors.hpp"
#include "ncurses.hpp"
#include "object.hpp"
#include "utility.hpp"
#include <memory>
#include <tuple>

namespace tasker::tui
{

template <typename CI>
class basic_window : public object
{
protected:
    CI *ncurses = nullptr;
    WINDOW *m_win = nullptr;

    // Dimensions
    int m_x { 0 };
    int m_y { 0 };

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

    basic_window &dimensions(int x, int y)
    {
        m_x = x;
        m_y = y;
        return *this;
    }

    std::tuple<int, int> dimensions() const
    {
        return std::make_tuple(m_x, m_y);
    }

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

        int x, y;
        this->ncurses->get_max_yx(this->m_win, y, x);
        if (x == -1) {
            return error(ERROR_GETMAXYX, "get_max_yx() failed");
        }
        this->dimensions(x, y);

        return OK;
    }

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

template <typename CI>
class window : public basic_window<CI>
{
private:
    template <typename T>
    using basic_window_ptr = std::shared_ptr<basic_window<T>>;

    basic_window_ptr<CI> m_parent;

    // Offsets
    int m_x_offset { 0 };
    int m_y_offset { 0 };

public:
    using basic_window<CI>::basic_window;

    window(CI &ci) = delete;

    window(CI &ci, basic_window_ptr<CI> parent)
        : basic_window<CI>(ci)
        , m_parent(std::move(parent))
    {
        // By default, set this child's dimensions to the same as the parent
        auto [x, y] = m_parent->dimensions();
        this->m_x = x;
        this->m_y = y;
    }

    window &offset(int x_offset, int y_offset)
    {
        m_x_offset = x_offset;
        m_y_offset = y_offset;
        return *this;
    }

    std::tuple<int, int> offset() const
    {
        return std::make_tuple(m_x_offset, m_y_offset);
    }

    int init() noexcept final override
    {
        if (!this->ncurses) {
            return error(ERROR, "window::ncurses was null during init()");
        }

        this->m_win = this->ncurses->subwin(
            m_parent->handle(), this->m_y, this->m_x, m_y_offset, m_x_offset);
        if (this->m_win == nullptr) {
            return error(ERROR_SUBWIN, "subwin() returned a nullptr");
        }

        return OK;
    }

    int refresh() noexcept final override
    {
        if (!*this) {
            return error(ERR, "window::refresh() called on a null handle");
        }

        return this->ncurses->wrefresh(this->m_win);
    }

    int end() noexcept final override
    {
        if (this->m_win) {
            auto rc = this->ncurses->delwin(this->m_win);
            this->m_win = nullptr;
            return rc;
        }
        return ERR;
    }
};

}; // namespace tasker::tui

#endif /* SRC_TUI_WINDOW_HPP */
