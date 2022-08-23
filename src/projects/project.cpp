/**
 * Copyright (C) 2022 Kevin Morris <kevr@0cost.org>
 * All Rights Reserved.
 **/
#include "project.hpp"
#include <stdexcept>
using namespace tasker;

project::project(project_data data)
    : m_data(std::move(data))
{
    if (m_data.id < 1) {
        throw std::invalid_argument(
            "`project_id` must be a valid integer id >= 1");
    }
}

const int project::id() const
{
    return m_data.id;
}

const std::string &project::name() const
{
    return m_data.name;
}

const std::string &project::full_name() const
{
    return m_data.full_name;
}
