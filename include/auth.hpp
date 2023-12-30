#pragma once
#include <pqxx/pqxx>
#include "user.hpp"

enum class AuthOpt { Login, Register, Exit };
AuthOpt getAuthOpt();
std::optional<User> inputCredentials(AuthOpt opt);
void authenticate();


class Auth {
   public:

    static void registerUser(User* user);
    static void loginUser(User* user);
    static void deleteUser(User* user);
};

class Session {
public:
    inline static std::string user{};
    inline static bool isAuthenticated{false};
    static void setUser(std::string_view authUser) { user = authUser; }
    static std::string& getUser() { return user; };
};
