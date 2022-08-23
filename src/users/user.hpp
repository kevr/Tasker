#ifndef SRC_USERS_USER_HPP
#define SRC_USERS_USER_HPP

#include <string>

namespace tasker
{

struct user_data {
    int id;            //! User ID
    std::string login; //! Username
};

class user
{
private:
    user_data m_data;

public:
    user() = default;
    user(user_data data);

    user(const user &o) = default;
    user &operator=(const user &o) = default;

    user(user &&o) = default;
    user &operator=(user &&o) = default;

    const int id() const;
    const std::string &login() const;
};

}; // namespace tasker

#endif /* SRC_USERS_USER_HPP */
