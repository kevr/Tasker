#ifndef SRC_TASKS_TASK_HPP
#define SRC_TASKS_TASK_HPP

#include "../projects/project.hpp"
#include <string>

namespace tasker
{

struct task_data {
    int id;
    std::string title;
    std::string body;
};

/**
 * @brief A kanban task object.
 **/
class task
{
private:
    task_data m_data; //! Metadata related to this task

public:
    task() = default;

    /*! Construct from scratch */
    task(task_data data);

    // Copy constructor/assignment
    task(const task &o) = default;
    task &operator=(const task &o) = default;

    // Move constructor/assignment
    task(task &&o) = default;
    task &operator=(task &&o) = default;

    /*! Task ID */
    const int id() const;

    /*! Task title */
    const std::string &title() const;

    /*! Task body */
    const std::string &body() const;
};

}; // namespace tasker

#endif /* SRC_TASKS_TASK_HPP */
