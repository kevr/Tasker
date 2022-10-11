#ifndef SRC_TUI_BASIC_WINDOW_HPP
#define SRC_TUI_BASIC_WINDOW_HPP

#include "../context.hpp"
#include "../ncurses.hpp"
#include "logging.hpp"
#include "object.hpp"
#include <algorithm>
#include <memory>
#include <stdexcept>
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
    basic_window_ptr m_root;
    std::vector<basic_window_ptr> m_children;

    // Dimensions
    std::tuple<int, int> m_size;

    // Offsets
    std::tuple<int, int> m_offset;

    logger logging;

public:
    inline static int instances = 0;

public:
    tasker::context<int> context;

public:
    basic_window_ptr root()
    {
        return m_root;
    }

    basic_window() = default;

    basic_window(CI &ncurses)
        : ncurses(&ncurses)
    {
        auto str = fmt::format("basic_window({0}) constructed", ++instances);
        logging.debug(LOG(str));
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
        m_size = std::make_tuple(x, y);
        return *this;
    }

    std::tuple<int, int> dimensions() const
    {
        auto str = fmt::format("dimensions() -> ({0}, {1})",
                               std::get<0>(m_size),
                               std::get<1>(m_size));
        logging.debug(LOG(str));
        return m_size;
    }

    basic_window &offset(int x, int y)
    {
        auto str = fmt::format("offset({0}, {1})", x, y);
        logging.debug(LOG(str));
        m_offset = std::make_tuple(x, y);
        return *this;
    }

    std::tuple<int, int> offset() const
    {
        auto str = fmt::format("offset() -> ({0}, {1})",
                               std::get<0>(m_offset),
                               std::get<1>(m_offset));
        logging.debug(LOG(str));
        return m_offset;
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

    int erase() noexcept
    {
        logging.debug(LOGTRACE());
        return this->ncurses->werase(this->handle());
    }

    virtual int refresh() noexcept override
    {
        logging.debug(R_LOGTRACE());
        if (!*this) {
            return error(ERR,
                         LOG("window::refresh() called on a null handle"));
        }

        return this->ncurses->wrefresh(this->m_win);
    }

    virtual int refresh_all() noexcept override
    {
        if (auto rc = refresh()) {
            return rc;
        }

        for (auto &child : this->m_children) {
            if (auto rc = child->refresh_all()) {
                return rc;
            }
        }

        return OK;
    }

    void add_child(basic_window_ptr window)
    {
        logging.debug(R_LOGTRACE());

        auto str = fmt::format("children: {0}", m_children.size());
        logging.debug(LOG(str));
        if (std::find(m_children.begin(), m_children.end(), window) !=
            m_children.end()) {
            throw std::logic_error(LOG("window already exists"));
        }

        m_children.emplace_back(std::move(window));
    }

    void remove_child(const basic_window_ptr &window)
    {
        logging.debug(LOGTRACE());
        auto it = std::find(m_children.begin(), m_children.end(), window);
        m_children.erase(it);
    }

    virtual int draw() noexcept override
    {
        for (auto &child : this->m_children) {
            if (auto rc = child->draw()) {
                return rc;
            }
        }
        return OK;
    }

    virtual int end() noexcept override
    {
        if (!this->ncurses) {
            return error(ERR, "ncurses was null during end()");
        }

        while (!this->m_children.empty()) {
            auto child = this->m_children.back();
            if (auto rc = child->end()) {
                return rc;
            }
        }
        return OK;
    }
};

}; // namespace tasker::tui

#endif /* SRC_TUI_BASIC_WINDOW_HPP */
