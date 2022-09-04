/**
 * Description: Main program entry-point.
 *
 * Copyright (C) 2022 Kevin Morris <kevr@0cost.org>
 * All Rights Reserved.
 **/
#include "task.hpp"
#include <stdexcept>
using namespace tasker;

task::task(task_data data)
    : m_data(std::move(data))
{
    if (m_data.id < 1) {
        throw std::invalid_argument("`id` must be a valid integer id >= 1");
    }
}

const int task::id() const
{
    return m_data.id;
}

const std::string &task::title() const
{
    return m_data.title;
}

const std::string &task::body() const
{
    return m_data.body;
}
