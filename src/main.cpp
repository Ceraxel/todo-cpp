#include <iostream>
#include <string>
#include "auth.hpp"
#include "todo.hpp"
#include "database.hpp"

int main(int argc, char* argv[]) {
    if (strcmp(argv[1], "--create-table"))
        Database().create_table();

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
