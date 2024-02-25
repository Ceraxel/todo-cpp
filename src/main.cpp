#include <iostream>
#include <memory>
#include <string>
#include "auth.hpp"
#include "database.hpp"
#include "todo.hpp"

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--create-table")
            Database().create_table();
    }

    authenticate();

    Session::user = "adrian";
    std::unique_ptr<Todo> todos = std::make_unique<Todo>();
    std::cout << "Displaying todo \n";

    while (Session::is_active) {
        todos->run();
    }

    return 0;
}
