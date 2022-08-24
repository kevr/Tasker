#ifndef SRC_TUI_HPP
#define SRC_TUI_HPP

#include "utility.hpp"
#include <stdexcept>

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

    // `tui` state
    int m_return_code = 0;
    bool m_created = false;
    bool m_ended = true;

public:
    tui(CI &ncurses) noexcept
        : ncurses(ncurses)
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

        if (!ncurses.initscr()) {
            m_return_code = error(1, "initscr() failed");
            return *this;
        }

        if (auto rc = ncurses.keypad(ncurses.root(), true)) {
            m_return_code = error(2, "keypad(...) failed: ", rc);
            return *this;
        }

        if (auto rc = ncurses.raw()) {
            m_return_code = error(3, "raw() failed: ", rc);
            return *this;
        }

        if (auto rc = ncurses.noecho()) {
            m_return_code = error(4, "noecho() failed: ", rc);
            return *this;
        }

        return *this;
    }

    int refresh() noexcept
    {
        return ncurses.refresh();
    }

    int return_code() const noexcept
    {
        return m_return_code;
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

        // Just run endwin() in all cases; in the case where we error
        // out after running initscr(), this will be able to undo what
        // initscr() did if it can.
        auto rc = ncurses.endwin();

        // Only update m_return_code if it's not already errored out.
        if (m_return_code == 0)
            m_return_code = rc;

        return return_code();
    }
};

}; // namespace tasker::tui

#endif /* SRC_TUI_HPP */
