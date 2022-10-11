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
            m_return_code = error(ERROR_KEYPAD, LOG(str));
            return *this;
        }

        if (auto rc = ncurses.raw()) {
            auto str = fmt::format("raw() failed: {0}", rc);
            m_return_code = error(ERROR_RAW, LOG(str));
            return *this;
        }

        if (auto rc = ncurses.noecho()) {
            auto str = fmt::format("noecho() failed: {0}", rc);
            m_return_code = error(ERROR_ECHO, LOG(str));
            return *this;
        }

        if (auto rc = ncurses.curs_set(0); rc == ERR) {
            auto str = fmt::format("curs_set(0) failed: {0}", rc);
            m_return_code = error(ERROR_ECHO, LOG(str));
            return *this;
        }

        if (ncurses.has_colors()) {
            auto &conf = cfg::config::ref();
            ncurses.start_color();
            ncurses.use_default_colors();

            ncurses.init_pair(theme::default_color, -1, -1);

            auto color = conf.get<short>("color.root_border");
            ncurses.init_pair(theme::root_border, color, COLOR_BLACK);

            auto color_bg = conf.get<short>("color.project_bar_bg");
            auto color_fg = conf.get<short>("color.project_bar_fg");
            ncurses.init_pair(theme::project_bar, color_fg, color_bg);
        }

        auto [x, y] = root->dimensions();
        logging.info(fmt::format("new dimensions: {0}, {1}", x, y));
        m_pane->set_dimensions(x - 2, y - 1);
        m_pane->offset(1, 0);
        if (auto rc = m_pane->init()) {
            auto str = fmt::format("m_pane->init() failed: {0}", rc);
            m_return_code = error(rc, LOG(str));
            return *this;
        }

        m_project->set_dimensions(x - 2, y - 1);
        if (auto rc = m_project->init()) {
            auto str = fmt::format("m_project->init() failed: {0}", rc);
            m_return_code = error(rc, LOG(str));
            return *this;
        }

        if (auto rc = draw()) {
            auto str = fmt::format("tui::init()'s draw() failed: {0}", rc);
            m_return_code = error(rc, LOG(str));
            return *this;
        }

        return *this;
    }

    void resize() noexcept
    {
        // Erase the contents of the root window in preparation
        // for a clean resizing of the terminal.
        root->erase();

        // End the TUI
        end();

        // Global ncurses refresh() must be called here after
        // endwin(), to cause ncurses to reinitialize the screen
        // size on the next initscr().
        this->ncurses.refresh();

        // Initialize the TUI
        init();

        // Refresh all windows owned by `root`, including itself
        refresh();
    }

    int draw() noexcept
    {
        if (auto rc = root->draw()) {
            auto str = fmt::format("root->draw() failed: {0}", rc);
            return error(rc, LOG(str));
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

    std::shared_ptr<root_window<CI>> get_root() const
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
        logging.debug(LOGTRACE());

        if (!m_created || m_ended) {
            return m_return_code;
        }
        m_ended = true;
        m_created = false;

        // Just destruct the root window in all cases; in the case where we
        // error out after running initscr(), this will be able to undo
        // what initscr() did if it can.
        if (auto rc = root->end(); m_return_code == OK && rc) {
            m_return_code = rc;
        }

        return return_code();
    }
};

}; // namespace tasker::tui

#endif /* SRC_TUI_HPP */
