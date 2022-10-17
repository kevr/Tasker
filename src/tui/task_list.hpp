#ifndef SRC_TUI_TASK_LIST_HPP
#define SRC_TUI_TASK_LIST_HPP

#include "config/config.hpp"
#include "pane.hpp"
#include "tasks/task.hpp"
#include "title_bar.hpp"
#include "window.hpp"
#include <algorithm>
#include <optional>
#include <vector>

namespace tasker::tui
{

/**
 * @brief Vertical list of task_info objects
 **/
template <typename CI>
class task_list : public window<CI>
{
private:
    using title_bar_t = title_bar<CI>;
    using title_bar_ptr = std::shared_ptr<title_bar_t>;

    using pane_t = pane<CI>;
    using pane_ptr = std::shared_ptr<pane_t>;

private:
    // List title window (populated with m_name)
    title_bar_ptr m_title;

    // List pane (where the tasks go)
    pane_ptr m_pane;

    // Focus flag
    bool m_focus = false;

    // Logging object
    logger logging;

private:
    void enforce_title() const
    {
        if (!m_title) {
            throw std::logic_error(
                "set_name(std::string) called on an uninitialized list");
        }
    }

public:
    using window<CI>::window;

    const std::string &name() const
    {
        enforce_title();
        return m_title->name;
    }

    task_list &set_name(const std::string &name)
    {
        enforce_title();
        m_title->name = name;
        return *this;
    }

    bool focused() const
    {
        return m_focus;
    }

    void focus()
    {
        m_focus = true;
    }

    void unfocus()
    {
        m_focus = false;
    }

    /**
     * @brief Initialize the task_list
     *
     * This action includes initialization of the internal task_list
     * window, title bar as well as a pane in which content resides.
     *
     * Member windows affected by this initialization:
     * - m_win
     * - m_title
     * - m_pane
     **/
    virtual int init() noexcept override
    {
        auto &conf = cfg::config::ref();
        auto task_list_width = conf.get<int>("style.task_list.width");

        auto [x, y] = this->m_parent->dimensions();
        auto width = std::min(task_list_width, x);
        this->set_dimensions(width, y);

        if (auto rc = window<CI>::init()) {
            return rc;
        }

        m_title = std::make_shared<title_bar_t>(*this->ncurses,
                                                this->shared_from_this());
        m_title->set_dimensions(width, 1);
        if (auto rc = m_title->init()) {
            return rc;
        }

        m_pane =
            std::make_shared<pane_t>(*this->ncurses, this->shared_from_this());
        m_pane->offset(0, 1);
        m_pane->set_dimensions(width, y - 1);
        if (auto rc = m_pane->init()) {
            return rc;
        }

        return OK;
    }

    virtual int draw() noexcept override
    {
        logging.debug(LOGTRACE());

        m_title->focus(m_focus);
        if (auto rc = m_title->draw()) {
            return rc;
        }

        return OK;
    }
};

}; // namespace tasker::tui

#endif /* SRC_TUI_TASK_LIST_HPP */
