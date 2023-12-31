#include <iostream>
#include <memory>
#include <string>
#include "auth.hpp"
#include "todo.hpp"
#include "database.hpp"

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--create-table"){
            Database().create_table();
        }
    }

    /* authenticate(); */
    /* pqxx::work txn{Database::conn}; */
    /* for (auto [username, password] : */
    /*      txn.query<std::string, std::string>("SELECT * FROM accounts")) { */
    /*     std::cout << "Account {" << username << ", " << password << ", " */
    /*               << "}" << '\n'; */
    /* } */
    /* txn.abort(); */

    Session::user = "adrian";

    TodoItem test1 {"Test1", "ONGOING"};
    TodoItem test2 {"Test2", "COMPLETED"};
    TodoItem test3 {"Test3", "ONGOING"};

    std::unique_ptr<Todo> todos = std::make_unique<Todo>();
    todos->add_todo(&test1);
    todos->add_todo(&test2);
    todos->add_todo(&test3);
    system("read");
    for (const auto& item : todos->get_todos()) {
        std::cout << "Title: " << item.get_title() << " Status: " << item.get_status() << '\n';
    }
    system("read");
    todos->remove_todo(&test1);
    todos->remove_todo(&test2);
    todos->remove_todo(&test3);
    for (const auto& item : todos->get_todos()) {
        std::cout << "Title: " << item.get_title() << " Status: " << item.get_status() << '\n';
    }

    system("read");

    return 0;
}
