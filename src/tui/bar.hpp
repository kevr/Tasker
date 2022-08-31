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

public:
    using window<CI>::window;

    virtual int init() noexcept override
    {
        logging.info("bar::init()");
        return window<CI>::init();
    }

    bar &inherit()
    {
        auto [x, _] = this->m_parent->dimensions();

        int padding = this->m_parent->padding();
        this->m_x = x - (padding * 2);
        this->m_y = 1;

        this->offset(0, 0);

        return *this;
    }

    virtual int draw() noexcept override
    {
        auto color = COLOR_PAIR(theme::project_bar);
        this->ncurses->wattr_enable(this->handle(), color);
        auto [x, _] = this->dimensions();
        auto status = fmt::format("{0} {1}", PROG, VERSION);
        std::string spaces(x - status.size(), ' ');
        status += spaces;
        this->ncurses->w_add_str(this->handle(), status.c_str());
        this->ncurses->wattr_disable(this->handle(), color);
        return OK;
    }

    virtual int refresh() noexcept override
    {
        logging.debug("bar::refresh()");
        return window<CI>::refresh();
    }
};

}; // namespace tasker::tui

#endif /* SRC_TUI_BAR_HPP */
