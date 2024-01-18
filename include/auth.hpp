#pragma once
#include <pqxx/pqxx>
#include "user.hpp"

enum class AuthOpt { Login, Register, Exit };
AuthOpt getAuthOpt();
std::optional<User> inputCredentials(AuthOpt opt);
void authenticate();

class Auth {
   public:
    static void register_user(User* user);
    static void login_user(User* user);
    static void delete_user(User* user);
};

class Session {
   public:
    inline static std::string user{};
    inline static bool is_authenticated{false};
    inline static bool is_active{true};
    static void set_user(std::string_view authUser) { user = authUser; }
    static std::string& get_user() { return user; };
};
