#ifndef SRC_TUI_BASIC_WINDOW_HPP
#define SRC_TUI_BASIC_WINDOW_HPP

#include "../ncurses.hpp"
#include "logging.hpp"
#include "object.hpp"
#include <algorithm>
#include <memory>
#include <vector>

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

    std::vector<basic_window_ptr> m_children;

    // Dimensions
    int m_x { 0 };
    int m_y { 0 };

    // Padding
    struct padding_t {
        int top = 0;
        int right = 0;
        int bottom = 0;
        int left = 0;
    } m_padding;

    logger logging;

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

    basic_window &set_dimensions(int x, int y)
    {
        auto str = fmt::format("set_dimensions({0}, {1})", x, y);
        logging.debug(LOG(str));
        m_x = x;
        m_y = y;
        return *this;
    }

    std::tuple<int, int> dimensions() const
    {
        auto str = fmt::format("dimensions() -> ({0}, {1})", m_x, m_y);
        logging.debug(LOG(str));
        return std::make_tuple(m_x, m_y);
    }

    basic_window &padding(int left, int top, int right, int bottom)
    {
        auto str = fmt::format(
            "padding({0}, {1}, {2}, {3})", left, top, right, bottom);
        logging.debug(LOG(str));
        m_padding.left = left;
        m_padding.top = top;
        m_padding.right = right;
        m_padding.bottom = bottom;
        return *this;
    }

    const padding_t &padding() const
    {
        auto str = fmt::format("padding() -> ({0}, {1}, {2}, {3})",
                               m_padding.left,
                               m_padding.top,
                               m_padding.right,
                               m_padding.bottom);
        logging.debug(LOG(str));
        return m_padding;
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

    virtual void resize() noexcept override
    {
        // no-op symbol definition for concrete class
    }

    int erase() noexcept
    {
        logging.debug(LOGTRACE());
        return this->ncurses->werase(this->handle());
    }

    int refresh_all() noexcept
    {
        logging.debug(LOGTRACE());
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
        logging.debug(LOGTRACE());
        m_children.emplace_back(std::move(window));
    }

    void remove_child(basic_window_ptr window)
    {
        logging.debug(LOGTRACE());
        auto it = std::find(m_children.begin(), m_children.end(), window);
        m_children.erase(it);
    }
};

}; // namespace tasker::tui

#endif /* SRC_TUI_BASIC_WINDOW_HPP */
