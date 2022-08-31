#ifndef SRC_TUI_HPP
#define SRC_TUI_HPP

#include "config/config.hpp"
#include "errors.hpp"
#include "logging.hpp"
#include "tui/color.hpp"
#include "tui/pane.hpp"
#include "tui/project.hpp"
#include "tui/window.hpp"
#include "utility.hpp"
#include <fmt/format.h>
#include <stdexcept>
#include <tuple>

namespace tasker::tui
{

/**
 * @brief Textual User Interface
 *
 * This class takes the responsibility of running tasker's TUI.
 * It owns the memory location of the root window and children.
 *
 * @tparam CI Curses Interface (e.g. tasker::ext::ncurses)
 **/
template <typename CI>
class tui
{
private:
    // curses library interface, bound on construction
    CI &ncurses;

    std::shared_ptr<root_window<CI>> root;

    std::shared_ptr<pane<CI>> m_pane;
    std::shared_ptr<project<CI>> m_project;

    // `tui` state
    int m_return_code = 0;
    bool m_created = false;
    bool m_ended = true;

    logger logging;

public:
    tui(CI &ncurses) noexcept
        : ncurses(ncurses)
        , root(std::make_shared<root_window<CI>>(ncurses))
        , m_pane(std::make_shared<pane<CI>>(ncurses, root))
        , m_project(std::make_shared<project<CI>>(ncurses, m_pane))
    {
    }

    tui(tui &&o) noexcept = default;
    tui &operator=(tui &&o) noexcept = default;
    tui(const tui &o) noexcept = default;
    tui &operator=(const tui &o) noexcept = default;

    ~tui() noexcept
    {
        end();
    }

    tui &init() noexcept
    {
        if (!m_ended || m_created) {
            return *this;
        }
        m_ended = false;
        m_created = true;

        if (auto rc = root->init()) {
            m_return_code = rc;
            return *this;
        }

        if (auto rc = ncurses.keypad(root->handle(), true)) {
            auto str = fmt::format("keypad() failed: {0}", rc);
            m_return_code = error(ERROR_KEYPAD, str);
            return *this;
        }

        if (auto rc = ncurses.raw()) {
            auto str = fmt::format("raw() failed: {0}", rc);
            m_return_code = error(ERROR_RAW, str);
            return *this;
        }

        if (auto rc = ncurses.noecho()) {
            auto str = fmt::format("noecho() failed: {0}", rc);
            m_return_code = error(ERROR_ECHO, str);
            return *this;
        }

        if (ncurses.has_colors()) {
            auto &conf = cfg::config::ref();
            ncurses.start_color();

            auto color = conf.get<short>("color.root_border");
            ncurses.init_pair(theme::root_border, color, COLOR_BLACK);

            auto color_bg = conf.get<short>("color.project_bar_bg");
            auto color_fg = conf.get<short>("color.project_bar_fg");
            ncurses.init_pair(theme::project_bar, color_fg, color_bg);
        }

        m_pane->inherit();           // Update sizes relative to the root
        m_pane->padding(1, 0, 1, 1); // Left, Top, Right, Bottom
        if (auto rc = m_pane->init()) {
            auto str = fmt::format("m_pane->init() failed: {0}", rc);
            m_return_code = error(rc, str);
            return *this;
        }

        m_project->inherit();
        if (auto rc = m_project->init()) {
            auto str = fmt::format("m_project->init() failed: {0}", rc);
            m_return_code = error(rc, str);
            return *this;
        }

        if (auto rc = draw()) {
            auto str = fmt::format("tui::init()'s draw() failed: {0}", rc);
            m_return_code = error(rc, str);
            return *this;
        }

        return *this;
    }

    void resize() noexcept
    {
        root->erase();
        end();
        root->refresh();
        init();
        root->refresh_all();
    }

    int draw() noexcept
    {
        if (auto rc = root->draw()) {
            auto str = fmt::format("root->draw() failed: {0}", rc);
            return error(rc, str);
        }

        if (auto rc = m_pane->draw()) {
            return rc;
        }

        return OK;
    }

    int refresh() noexcept
    {
        return root->refresh_all();
    }

    int return_code() const noexcept
    {
        return m_return_code;
    }

    std::shared_ptr<root_window<CI>> window() const
    {
        return root;
    }

    std::tuple<int, int> dimensions() const
    {
        return root->dimensions();
    }

    operator bool() const noexcept
    {
        return m_return_code == 0;
    }

    int end() noexcept
    {
        if (!m_created || m_ended) {
            return m_return_code;
        }
        m_ended = true;
        m_created = false;

        m_pane->end();

        // Just destruct the root window in all cases; in the case where we
        // error out after running initscr(), this will be able to undo
        // what initscr() did if it can.
        auto rc = root->end();

        // Only update m_return_code if it's not already errored out.
        if (m_return_code == 0)
            m_return_code = rc;

        return return_code();
    }
};

}; // namespace tasker::tui

#endif /* SRC_TUI_HPP */
