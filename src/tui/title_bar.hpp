#ifndef SRC_TUI_TITLE_BAR_HPP
#define SRC_TUI_TITLE_BAR_HPP

#include "window.hpp"

namespace tasker::tui
{

template <typename CI>
class title_bar : public window<CI>
{
private:
    bool m_focus = false;

public:
    using window<CI>::window;

    std::string name = "Untitled";

    bool focused() const
    {
        return m_focus;
    }

    void focus(bool focused)
    {
        m_focus = focused;
    }

    virtual int draw() noexcept override
    {
        // Deduce background color based on focus
        auto color = COLOR_PAIR(theme::default_color);
        if (m_focus) {
            color = COLOR_PAIR(theme::project_bar);
        }

        // Set background color attribute
        this->ncurses->wbkgd(this->handle(), color);

        // Construct output with space padding
        auto [x, _] = this->dimensions();
        std::string output(name);
        output += std::string(x - output.size() - 1, ' ');

        // Move the cursor to the beginning of the window and add the title str
        this->ncurses->wmove(this->handle(), 0, 0);
        if (auto rc =
                this->ncurses->w_add_str(this->handle(), output.c_str())) {
            return error(ERROR_WADDSTR, "w_add_str failed");
        }

        return OK;
    }

    virtual int init() noexcept override
    {
        return window<CI>::init();
    }
};

}; // namespace tasker::tui

#endif /* SRC_TUI_TITLE_BAR_HPP */
