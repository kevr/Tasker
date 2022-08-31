#ifndef SRC_TUI_PROJECT_HPP
#define SRC_TUI_PROJECT_HPP

#include "bar.hpp"
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
    std::shared_ptr<bar_t> m_bar;

    using pane_t = pane<CI>;
    std::shared_ptr<pane_t> m_content;

    logger logging;

public:
    using window<CI>::window;

    virtual int init() noexcept override
    {
        logging.info("project::init()");
        this->inherit();
        if (auto rc = window<CI>::init()) {
            return rc;
        }

        m_bar =
            std::make_shared<bar_t>(*this->ncurses, this->shared_from_this());
        m_bar->inherit();
        if (auto rc = m_bar->init()) {
            return rc;
        }

        m_content =
            std::make_shared<pane_t>(*this->ncurses, this->shared_from_this());
        m_content->inherit();

        auto [x, y] = this->m_content->dimensions();
        m_content->set_dimensions(x, y - 1);
        m_content->offset(0, 1);
        if (auto rc = m_content->init()) {
            return rc;
        }

        return OK;
    }

    virtual int draw() noexcept override
    {
        m_bar->draw();
        auto &conf = cfg::config::ref();
        auto key_quit = conf.get<char>("key_quit");
        auto message = fmt::format("Press '{0}' to quit...", key_quit);
        if (auto rc = this->ncurses->w_add_str(m_content->handle(),
                                               message.c_str())) {
            return error(ERROR_WADDSTR, "w_add_str() failed: ", rc);
        }
        return OK;
    }

    virtual int refresh() noexcept override
    {
        m_bar->refresh();
        m_content->refresh();
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
