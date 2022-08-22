#ifndef SRC_PROJECTS_PROJECT_HPP
#define SRC_PROJECTS_PROJECT_HPP

/**
 * Description: Header file.
 *
 * Copyright (C) 2022 Kevin Morris <kevr@0cost.org>
 * All Rights Reserved.
 **/
namespace tasker
{

class project
{
private:
    int m_id;

public:
    //! Construct from scratch.
    project(int id);

    // Constructors/assignments
    project(const project &o);
    project &operator=(const project &o);

    //! Project ID
    const int id() const;
};

}; // namespace tasker

#endif /* SRC_PROJECTS_PROJECT_HPP */
