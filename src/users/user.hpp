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

    user(const user &o);
    user &operator=(const user &o);

    user(user &&o);
    user &operator=(user &&o);

    const int id() const;
    const std::string &login() const;
};

}; // namespace tasker

#endif /* SRC_USERS_USER_HPP */
