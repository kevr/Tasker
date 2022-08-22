#ifndef SRC_PROJECTS_PROJECT_HPP
#define SRC_PROJECTS_PROJECT_HPP

#include <string>

namespace tasker
{

struct project_data {
    int id;
    std::string name;
    std::string full_name;
};

class project
{
private:
    project_data m_data;

public:
    project() = default;

    //! Construct from scratch.
    project(project_data data);

    // Constructors/assignment
    project(const project &o);
    project &operator=(const project &o);

    // Move constructors/assignment
    project(project &&o);
    project &operator=(project &&o);

    //! Project ID
    const int id() const;

    //! Project name
    const std::string &name() const;

    //! Project full name
    const std::string &full_name() const;
};

}; // namespace tasker

#endif /* SRC_PROJECTS_PROJECT_HPP */
