#include "user.hpp"
#include <string>

std::string User::get_username() const {
    return username_;
}
std::string User::get_password() const {
    return password_;
}

void User::set_username(std::string_view username) {
    username_ = username;
};
void User::set_password(std::string_view password) {
    password_ = password;
};
