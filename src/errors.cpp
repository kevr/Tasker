#include <iostream>

int raw_error(int rc, const std::string &message)
{
    std::cerr << message;
    return rc;
}
