#pragma once
#include <pqxx/pqxx>
#include "user/user.hpp"

class Database {
   public:
    inline static pqxx::connection conn{
        "dbname=project user=postgres password=postgres hostaddr="};
};

class Auth {
   public:

    static void registerUser(UserObj& user);
    static void loginUser(UserObj& user);
    static void deleteUser(UserObj& user);
};

class Session {
public:
    inline static std::string user{};
    inline static bool isAuthenticated{false};
    static void setUser(std::string_view authUser) { user = authUser; }
    static std::string& getUser() { return user; };
};
