#include "user.hpp"
#include <string>

std::string User::getUsername() const {
    return username_;
}
std::string User::getPassword() const {
    return password_;
}

void User::setUsername(std::string_view username) {
    username_ = username;
};
void User::setPassword(std::string_view password) {
    password_ = password;
};
