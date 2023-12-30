#include <iostream>
#include <string>
#include "auth.hpp"
#include "todo.hpp"

int main() {

    authenticate();

    pqxx::work txn{Database::conn};
    for (auto [username, password] :
         txn.query<std::string, std::string>("SELECT * FROM accounts")) {
        std::cout << "Account {" << username << ", " << password << ", "
                  << "}" << '\n';
    }
    txn.abort();

    system("read");

    return 0;
}
