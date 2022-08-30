#ifndef SRC_TUI_WINDOW_HPP
#define SRC_TUI_WINDOW_HPP

#include "basic_window.hpp"
#include "errors.hpp"
#include "logging.hpp"
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

    // Logging object
    logger logging;

public:
    using basic_window<CI>::basic_window;

    window(CI &ci) = delete;

    window(CI &ci, basic_window_ptr<CI> parent)
        : basic_window<CI>(ci)
        , m_parent(std::move(parent))
    {
        inherit();
    }

    window &inherit()
    {
        auto [x, y] = m_parent->dimensions();
        this->m_x = x - (m_parent->padding() * 2);
        this->m_y = y - (m_parent->padding() * 2);
        return *this;
    }

    std::tuple<int, int> dimensions() const
    {
        int x = this->m_x - (this->m_padding * 2);
        int y = this->m_y - (this->m_padding * 2);
        auto message = fmt::format("dimensions() = ({0}, {1})", x, y);
        logging.debug(message);
        return std::make_tuple(x, y);
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

    virtual int draw() noexcept override
    {
        return ERROR_NOIMPL;
    }

    int init() noexcept override
    {
        if (!this->ncurses) {
            return error(ERROR, "window::ncurses was null during init()");
        }

        int y = this->m_y - (this->m_padding * 2);
        int x = this->m_x - (this->m_padding * 2);
        int yo = m_y_offset + this->m_padding;
        int xo = m_x_offset + this->m_padding;

        auto message = fmt::format("derwin({0}, {1}, {2}, {3})", y, x, yo, xo);
        logging.debug(message);

        this->m_win = this->ncurses->derwin(m_parent->handle(), y, x, yo, xo);
        if (this->m_win == nullptr) {
            return error(ERROR_DERWIN, "derwin() returned a nullptr");
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
