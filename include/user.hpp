#pragma once

#include <string>
#include <string_view>


class User {
   public:
    User() = default;
    User(std::string username, std::string password)
        : username_{username}, password_{password} {}
    
    std::string getUsername() const;
    std::string getPassword() const;

    void setUsername(std::string_view username);
    void setPassword(std::string_view password);


   private:
    std::string username_{};
    std::string password_{};
};