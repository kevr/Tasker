#ifndef SRC_TUI_PANE_HPP
#define SRC_TUI_PANE_HPP

#include "window.hpp"
#include <fmt/format.h>
#include <stdexcept>
#include <vector>

namespace tasker::tui
{

template <typename CI>
class pane : public window<CI>
{
private:
    size_t m_i = 0;

    logger logging;

public:
    using window<CI>::window;

    ~pane() = default;

    virtual int init() noexcept override
    {
        logging.debug(LOGTRACE());
        return window<CI>::init();
    }

    void focus(size_t i)
    {
        auto size = this->m_children.size();
        auto n = size - 1;
        if (!size) {
            throw std::out_of_range("pane::m_children is empty");
        } else if (i > n) {
            auto message = fmt::format("provided i={0} is out of bounds, "
                                       "there are {1} children ({2}-{3})",
                                       i,
                                       size,
                                       0,
                                       n);
            throw std::out_of_range(message);
        }

        m_i = i;
    }

    virtual int draw() noexcept override
    {
        // Draw focused child.
        if (this->m_children.size()) {
            if (auto rc = this->m_children[m_i]->draw()) {
                return rc;
            }
        }
        return OK;
    }

    int refresh() noexcept final override
    {
        logging.debug(LOGTRACE());

        if (auto rc = window<CI>::refresh()) {
            return rc;
        }

        if (this->m_children.size()) {
            if (auto rc = this->m_children[m_i]->refresh()) {
                return rc;
            }
        }

        return OK;
    }

    int refresh_all() noexcept = delete;
};

}; // namespace tasker::tui

#endif /* SRC_TUI_PANE_HPP */
