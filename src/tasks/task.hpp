#ifndef SRC_TASKS_TASK_HPP
#define SRC_TASKS_TASK_HPP

#include "../projects/project.hpp"

namespace tasker
{

/**
 * @brief A kanban task object.
 **/
class task
{
private:
    int m_id;           //! Issue/Task ID
    project &m_project; //! Project which owns the task

public:
    /*! Construct from scratch */
    task(project &project_id, int id);

    /*! Copy constructor/assignment operator */
    task(const task &o);
    task &operator=(const task &o);

    /*! Task ID */
    const int id() const;

    /*! Project ID */
    const project &parent() const;
};

}; // namespace tasker

#endif /* SRC_TASKS_TASK_HPP */
