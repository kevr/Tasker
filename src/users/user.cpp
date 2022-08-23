/**
 * Description: Main program entry-point.
 *
 * Copyright (C) 2022 Kevin Morris <kevr@0cost.org>
 * All Rights Reserved.
 **/
#include "user.hpp"
using namespace tasker;

user::user(user_data data)
    : m_data(std::move(data))
{
}

const int user::id() const
{
    return m_data.id;
}

const std::string &user::login() const
{
    return m_data.login;
}
