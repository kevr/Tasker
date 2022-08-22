/**
 * Description: Main program entry-point.
 *
 * Copyright (C) 2022 Kevin Morris <kevr@0cost.org>
 * All Rights Reserved.
 **/
#include "task.hpp"
#include <stdexcept>
using namespace tasker;

task::task(tasker::project &p, int id)
    : m_id(id)
    , m_project(p)
{
    if (m_id < 1) {
        throw std::invalid_argument("`id` must be a valid integer id >= 1");
    }
}

task::task(const task &o)
    : m_id(o.m_id)
    , m_project(o.m_project)
{
}

task &task::operator=(const task &o)
{
    m_id = o.m_id;
    m_project = o.m_project;
    return *this;
}

const int task::id() const
{
    return m_id;
}

const project &task::parent() const
{
    return m_project;
}
