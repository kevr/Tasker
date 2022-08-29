#ifndef SRC_TUI_WINDOW_HPP
#define SRC_TUI_WINDOW_HPP

#include "basic_window.hpp"
#include "errors.hpp"
#include "root_window.hpp"
#include "utility.hpp"
#include <memory>
#include <tuple>

namespace tasker::tui
{

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

    int init() noexcept override
    {
        if (!this->ncurses) {
            return error(ERROR, "window::ncurses was null during init()");
        }

        this->m_win = this->ncurses->subwin(
            m_parent->handle(), this->m_y, this->m_x, m_y_offset, m_x_offset);
        if (this->m_win == nullptr) {
            return error(ERROR_SUBWIN, "subwin() returned a nullptr");
        }

        m_parent->add_child(this->shared_from_this());
        return OK;
    }

    int refresh() noexcept override
    {
        if (!*this) {
            return error(ERR, "window::refresh() called on a null handle");
        }

        return this->ncurses->wrefresh(this->m_win);
    }

    int end() noexcept override
    {
        if (this->m_win) {
            auto rc = this->ncurses->delwin(this->m_win);
            this->m_parent->remove_child(this->shared_from_this());
            this->m_win = nullptr;
            return rc;
        }
        return ERR;
    }
};

}; // namespace tasker::tui

#endif /* SRC_TUI_WINDOW_HPP */
