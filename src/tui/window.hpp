#ifndef SRC_TUI_WINDOW_HPP
#define SRC_TUI_WINDOW_HPP

#include "basic_window.hpp"
#include "errors.hpp"
#include "logging.hpp"
#include "root_window.hpp"
#include "utility.hpp"
#include <memory>
#include <tuple>

namespace tasker::tui
{

template <typename CI>
class window : public basic_window<CI>
{
private:
    template <typename T>
    using basic_window_ptr = std::shared_ptr<basic_window<T>>;

protected:
    basic_window_ptr<CI> m_parent;

private:
    // Logging object
    logger logging;

    std::tuple<int, int> m_old_parent_xy;

public:
    inline static int instances = 0;

public:
    using basic_window<CI>::basic_window;

    window(CI &ci) = delete;

    window(CI &ci, basic_window_ptr<CI> parent)
        : basic_window<CI>(ci)
        , m_parent(std::move(parent))
    {
    }

    virtual void resize() noexcept
    {
        logging.debug(LOGTRACE());

        auto [ox, oy] = this->dimensions();
        auto x_delta = std::get<0>(m_old_parent_xy) - ox;
        auto y_delta = std::get<1>(m_old_parent_xy) - oy;

        auto [x, y] = m_parent->dimensions();
        this->set_dimensions(x - x_delta, y - y_delta);
    }

    virtual int draw() noexcept override
    {
        auto str = "draw() -> ERROR_NOIMPL";
        logging.debug(LOG(str));
        return ERROR_NOIMPL;
    }

    bool initialized() const
    {
        return this->m_win;
    }

    virtual int init() noexcept override
    {
        logging.debug(R_LOGTRACE());

        if (!this->ncurses) {
            return error(ERR, LOG("window::ncurses was null during init()"));
        }

        if (auto [x, _] = this->dimensions(); !x) {
            resize();
        }

        auto [x, y] = this->dimensions();
        m_old_parent_xy = m_parent->dimensions();

        auto [xo, yo] = this->offset();

        auto message = fmt::format("derwin({0}, {1}, {2}, {3})", y, x, yo, xo);
        logging.debug(LOG(message));

        this->m_win = this->ncurses->derwin(m_parent->handle(), y, x, yo, xo);
        if (this->m_win == nullptr) {
            return error(ERROR_DERWIN, LOG("derwin() returned a nullptr"));
        }

        auto str = fmt::format("window::init({0}) [0x{1:2x}]",
                               ++instances,
                               (unsigned long)&*this);
        logging.debug(LOG(str));

        this->m_root = m_parent->root();
        m_parent->add_child(this->shared_from_this());
        return OK;
    }

    virtual int end() noexcept override
    {
        logging.debug(R_LOGTRACE());
        if (auto rc = basic_window<CI>::end()) {
            return rc;
        }

        if (this->m_win) {
            auto rc = this->ncurses->delwin(this->m_win);
            this->m_win = nullptr;
            auto str = fmt::format("window::end({0}) [0x{1:2x}]",
                                   instances--,
                                   (unsigned long)&*this);
            logging.debug(LOG(str));
            return rc;
        }

        this->m_parent->remove_child(this->shared_from_this());
        return OK;
    }
};

}; // namespace tasker::tui

#endif /* SRC_TUI_WINDOW_HPP */
