#pragma once

#include <string>
#include <string_view>


class User {
   public:
    User() = default;
    User(std::string username, std::string password)
        : username_{username}, password_{password} {}
    
    std::string get_username() const;
    std::string get_password() const;

    void set_username(std::string_view username);
    void set_password(std::string_view password);


   private:
    std::string username_{};
    std::string password_{};
};
