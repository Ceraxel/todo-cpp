#include "auth.hpp"
#include "user/user.hpp"
#include <pqxx/pqxx>

void Auth::registerUser(User* user) const {
    pqxx::work txn {Database::conn};
    txn.exec("INSERT INTO accounts VALUES ('" + user->getUsername() + "', '" + user->getPassword() + "')");
    txn.commit();
}

void Auth::deleteUser(User* user) const {
    pqxx::work txn {Database::conn};
    txn.exec("DELETE FROM accounts WHERE username = '" + user->getUsername() + "'");
    txn.commit();
};

void Auth::loginUser(User* user) const {
    pqxx::work txn {Database::conn};
    pqxx::result res {txn.exec("SELECT * FROM accounts WHERE username = '" + user->getUsername() + "' AND password = '" + user->getPassword() + "'")};
    if (!res.empty()) {
        Auth::isAuthenticated = true;
    }
}

