#ifndef SRC_TUI_BOARD_HPP
#define SRC_TUI_BOARD_HPP

#include "../config/config.hpp"
#include "../defaults.hpp"
#include "../logging.hpp"
#include "task_list.hpp"
#include "window.hpp"
#include <vector>

namespace tasker::tui
{

template <typename CI>
class board : public window<CI>
{
private:
    using task_list_t = task_list<CI>;
    using task_list_ptr = std::shared_ptr<task_list_t>;

    std::vector<task_list_ptr> m_lists;
    size_t m_focus = 0;

    logger logging;

public:
    using window<CI>::window;

    int quickswitch(int i)
    {
        auto si = static_cast<size_t>(i);
        if (m_lists.empty() || si >= m_lists.size() || i < 0) {
            return ERR;
        }

        this->erase();
        this->focus(i);
        this->draw();
        this->refresh_all();

        return OK;
    }

    void focus(size_t i)
    {
        m_lists[m_focus]->unfocus();
        m_focus = i;
        m_lists[m_focus]->focus();
    }

    int add_list(task_list_ptr list)
    {
        auto &conf = cfg::config::ref();
        auto task_list_width = conf.get<int>("style.task_list.width");

        auto [x, y] = this->dimensions();
        auto width = std::min(task_list_width, x);

        // TODO: Why does this fuck up rendering on > 1 list?
        auto n = m_lists.size();

        // TODO: Should we really task_list::init() here?
        // This is kinda coupling list init with adding it
        // to a board; not sure this is what we want, though
        // it does work right now
        list->offset(n * width, 0);

        if (!list->initialized()) {
            if (auto rc = list->init()) {
                return error(rc, LOG("task_list::init() failed"));
            }
        }

        m_lists.emplace_back(std::move(list));

        if (m_lists.size() == 1) {
            // If we only have one list, set focus to that list
            m_focus = 0;
            m_lists.back()->focus();
        }

        return OK;
    }

    void remove_list(const task_list_ptr &list)
    {
        m_lists.erase(std::find(m_lists.begin(), m_lists.end(), list));
    }

    void remove_list(size_t i)
    {
        m_lists.erase(i);
    }

    virtual int init() noexcept override
    {
        if (auto rc = window<CI>::init()) {
            return rc;
        }

        auto root = reinterpret_cast<root_window<CI> *>(this->m_root.get());

        // Bind the left arrow key to focus on the next-left list, if any
        root->context.keybinds[KEY_LEFT] = [this]() -> int {
            return this->quickswitch(m_focus - 1);
        };

        // Bind the right arrow key to focus on the next-right list, if any
        root->context.keybinds[KEY_RIGHT] = [this]() -> int {
            return this->quickswitch(m_focus + 1);
        };

        // TODO: Make this keybind tunable
        auto &conf = cfg::config::ref();
        auto key = conf.get<char>("keybindings.project.new_list");
        root->context.keybinds[key] = [this]() -> int {
            using task_list_t = task_list<CI>;
            logging.debug("added list");
            auto list = std::make_shared<task_list_t>(
                *this->ncurses, this->shared_from_this());
            this->erase();

            if (auto rc = this->add_list(list)) {
                return rc;
            }

            this->draw();

            return OK;
        };

        return OK;
    }

    virtual int draw() noexcept override
    {
        logging.debug(LOGTRACE());

        this->erase();

        auto &conf = cfg::config::ref();
        auto key = conf.get<char>("keybindings.project.new_list");
        if (m_lists.empty()) {

            auto str = fmt::format("Press '{0}' to add a list...", key);
            if (auto rc =
                    this->ncurses->w_add_str(this->handle(), str.c_str())) {
                auto str = fmt::format("w_add_str() failed: {0}", rc);
                return error(ERROR_WADDSTR, str);
            }

        } else {

            auto str = fmt::format("lists: {0}", m_lists.size());
            logging.debug(LOG(str));
            size_t i = 0;
            for (auto &list : m_lists) {
                // Try to draw the list. If draw() returns a non-zero
                // error code, return it.
                if (auto rc = list->draw()) {
                    auto str = fmt::format("failed to draw list {0}", i);
                    return error(rc, str);
                }
                ++i;
            }

            this->refresh_all();
        }

        return OK;
    }
};

}; // namespace tasker::tui

#endif /* SRC_TUI_BOARD_HPP */
