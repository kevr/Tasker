#ifndef SRC_TUI_PROJECT_HPP
#define SRC_TUI_PROJECT_HPP

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

public:
    using window<CI>::window;

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
