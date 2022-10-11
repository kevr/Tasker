#ifndef SRC_TUI_ROOT_WINDOW_HPP
#define SRC_TUI_ROOT_WINDOW_HPP

#include "../context.hpp"
#include "../errors.hpp"
#include "../logging.hpp"
#include "../utility.hpp"
#include "basic_window.hpp"
#include "tui/color.hpp"

namespace tasker::tui
{

template <typename CI>
class root_window : public basic_window<CI>
{
private:
    logger logging;

public:
    using basic_window<CI>::basic_window;

    // Defaulted destructor override.
    ~root_window() noexcept
    {
    }

    virtual int init() noexcept final override
    {
        logging.debug(LOGTRACE());
        if (!this->ncurses) {
            return error(ERR, LOG("ncurses was null during init()"));
        }

        if (this->m_win == nullptr) {
            logging.debug(LOG("initscr()"));
            this->m_win = this->ncurses->initscr();
            if (this->m_win == nullptr) {
                return error(ERROR_INITSCR,
                             LOG("initscr() returned a nullptr"));
            }
        }

        int x, y;
        this->ncurses->get_max_yx(this->m_win, y, x);
        if (x == -1) {
            return error(ERROR_GETMAXYX, LOG("get_max_yx() failed"));
        }
        this->set_dimensions(x, y);

        auto str = fmt::format("get_max_yx({0}, {1})", y, x);
        logging.debug(LOG(str));

        this->m_root = this->shared_from_this();
        return OK;
    }

    virtual int draw() noexcept final override
    {
        logging.debug(LOGTRACE());

        // Set color attributes before actual drawing
        auto pair = COLOR_PAIR(theme::root_border);
        this->ncurses->wattr_enable(this->m_win, pair);

        // Draw a border around the window. If it fails to draw,
        // we return the error code with a message.
        auto rc = this->ncurses->wborder(this->handle(),
                                         ACS_VLINE,
                                         ACS_VLINE,
                                         ACS_HLINE,
                                         ACS_HLINE,
                                         ACS_ULCORNER,
                                         ACS_URCORNER,
                                         ACS_LLCORNER,
                                         ACS_LRCORNER);
        if (rc) {
            return error(rc, "root_window wborder failed to draw");
        }

        // Revert color attributes
        this->ncurses->wattr_disable(this->m_win, pair);

        return basic_window<CI>::draw();
    }

    virtual int refresh() noexcept final override
    {
        if (!this->ncurses) {
            return error(ERR, "ncurses was null during refresh");
        }

        return this->ncurses->refresh();
    }

    virtual int end() noexcept final override
    {
        basic_window<CI>::end();

        if (this->m_win) {
            if (auto rc = this->ncurses->endwin()) {
                auto str = fmt::format("endwin() failed: {0}", rc);
                return error(rc, LOG(str));
            }

            this->m_win = nullptr;
        }
        return OK;
    }
};

}; // namespace tasker::tui

#endif /* SRC_TUI_ROOT_WINDOW_HPP */
