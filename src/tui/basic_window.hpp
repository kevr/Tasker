#ifndef SRC_TUI_BASIC_WINDOW_HPP
#define SRC_TUI_BASIC_WINDOW_HPP

#include "../ncurses.hpp"
#include "object.hpp"
#include <algorithm>
#include <list>
#include <memory>

namespace tasker::tui
{

/**
 * @brief A window base meant to be derived into a real window object.
 *
 * basic_window<CI> uses the `object` interface, along with several more
 * pieces of functionality related to ncurses windows.
 *
 * See implementations `root_window<CI>` and `window<CI>` for core
 * derivatives of `basic_window<CI>` which utilize ncurses.
 *
 * Construction of this class should only take place via std::make_shared,
 * as it utilizes the std::enable_shared_from_this interface.
 **/
template <typename CI>
class basic_window : public object,
                     public std::enable_shared_from_this<basic_window<CI>>
{
protected:
    CI *ncurses = nullptr;
    WINDOW *m_win = nullptr;

    using basic_window_ptr = std::shared_ptr<basic_window<CI>>;

    std::list<basic_window_ptr> m_children;

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

    int refresh_all() noexcept
    {
        if (auto rc = refresh())
            return rc;

        for (auto &win : this->m_children) {
            if (auto rc = win->refresh()) {
                return rc;
            }
        }

        return OK;
    }

    void add_child(basic_window_ptr window)
    {
        m_children.emplace_back(std::move(window));
    }

    void remove_child(basic_window_ptr window)
    {
        auto it = std::find(m_children.begin(), m_children.end(), window);
        m_children.erase(it);
    }
};

}; // namespace tasker::tui

#endif /* SRC_TUI_BASIC_WINDOW_HPP */
