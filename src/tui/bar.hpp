#ifndef SRC_TUI_BAR_HPP
#define SRC_TUI_BAR_HPP

#include "color.hpp"
#include "config.hpp"
#include "config/config.hpp"
#include "window.hpp"

namespace tasker::tui
{

template <typename CI>
class bar : public window<CI>
{
private:
    logger logging;

    std::tuple<int, int> m_parent_xy;

public:
    using window<CI>::window;

    virtual int init() noexcept override
    {
        logging.debug(LOGTRACE());
        return window<CI>::init();
    }

    virtual int draw() noexcept override
    {
        logging.debug(LOGTRACE());

        auto color = COLOR_PAIR(theme::project_bar);
        this->ncurses->wattr_enable(this->handle(), color);
        auto [x, _] = this->dimensions();
        auto status = fmt::format(" {0} {1}", PROG, VERSION);
        std::string spaces(x - status.size(), ' ');
        status += spaces;
        this->ncurses->w_add_str(this->handle(), status.c_str());
        this->ncurses->wattr_disable(this->handle(), color);

        return OK;
    }
};

}; // namespace tasker::tui

#endif /* SRC_TUI_BAR_HPP */
