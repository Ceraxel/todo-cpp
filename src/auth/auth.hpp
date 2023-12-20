#pragma once
#include "user/user.hpp"
#include <pqxx/pqxx>

class Database {
public:
    inline static pqxx::connection conn{"dbname=project user=postgres password=postgres hostaddr="};
};

class Auth {
public:
    inline static bool isAuthenticated { false };

    static void registerUser(UserObj& user);
    static void loginUser(UserObj& user);
    static void deleteUser(UserObj& user);
};
