/**
 * Copyright (C) 2022 Kevin Morris <kevr@0cost.org>
 * All Rights Reserved.
 **/
#include "project.hpp"
#include <stdexcept>
using namespace tasker;

project::project(int id)
    : m_id(id)
{
    if (m_id < 1) {
        throw std::invalid_argument(
            "`project_id` must be a valid integer id >= 1");
    }
}

project::project(const project &o)
    : m_id(o.m_id)
{
}

project &project::operator=(const project &o)
{
    m_id = o.m_id;
    return *this;
}

const int project::id() const
{
    return m_id;
}
