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

    void registerUser(User* user) const;
    void loginUser(User* user) const;
    void deleteUser(User* user) const;
};
