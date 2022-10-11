#ifndef SRC_TUI_PROJECT_HPP
#define SRC_TUI_PROJECT_HPP

#include "../context.hpp"
#include "bar.hpp"
#include "board.hpp"
#include "pane.hpp"
#include "projects/project.hpp"
#include "window.hpp"

namespace tasker::tui
{

template <typename CI>
class project : public window<CI>, public tasker::project
{
private:
    using window_t = window<CI>;
    using window_ptr = std::shared_ptr<window_t>;
    using bar_t = bar<CI>;
    using pane_t = pane<CI>;
    using board_t = board<CI>;

private:
    std::shared_ptr<bar_t> m_bar;
    std::shared_ptr<pane_t> m_content;
    std::shared_ptr<board_t> m_board;

    logger logging;

public:
    using window<CI>::window;

    virtual int init() noexcept override
    {
        logging.debug(LOGTRACE());
        if (auto rc = window<CI>::init()) {
            return rc;
        }

        auto [x, y] = this->dimensions();

        m_bar =
            std::make_shared<bar_t>(*this->ncurses, this->shared_from_this());
        m_bar->set_dimensions(x, 1);
        if (auto rc = m_bar->init()) {
            return error(rc, "m_bar->init() failed");
        }

        m_content =
            std::make_shared<pane_t>(*this->ncurses, this->shared_from_this());
        m_content->set_dimensions(x, y - 1);
        m_content->offset(0, 1);
        if (auto rc = m_content->init()) {
            return error(rc, "m_content->init() failed");
        }

        m_board = std::make_shared<board_t>(*this->ncurses, m_content);
        auto [cx, cy] = m_content->dimensions();
        // Pad x one column on both sides & cut off the top of y.
        m_board->set_dimensions(cx - 2, cy - 1);
        m_board->offset(1, 1);

        if (auto rc = m_board->init()) {
            return error(rc, "m_board->init() failed");
        }

        return OK;
    }

    virtual int draw() noexcept override
    {
        logging.debug(LOGTRACE());

        m_bar->draw();
        if (auto rc = m_content->draw()) {
            return rc;
        }

        return OK;
    }

    // Needs to be overridden; projects are meta objects which
    // contain a bar and a pane; these need to be refreshed.
    virtual int refresh() noexcept override
    {
        logging.debug(LOGTRACE());

        m_bar->refresh_all();
        m_content->refresh_all();
        return OK;
    }

    int id(int id)
    {
        this->m_project.id = id;
        return this->m_project;
    }

    const std::string &name(const std::string &name)
    {
        this->m_project.name = name;
        return this->m_project.name;
    }

    const std::string &full_name(const std::string &full_name)
    {
        this->m_project.full_name = full_name;
        return this->m_project.full_name;
    }
};

}; // namespace tasker::tui

#endif /* SRC_TUI_PROJECT_HPP */
