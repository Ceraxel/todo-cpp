#include <iostream>
#include <memory>
#include <string>
#include "auth.hpp"
#include "database.hpp"
#include "todo.hpp"

void crud_test();
int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--create-table")
            Database().create_table();
    }

    /* authenticate(); */

    Session::user = "adrian";
    std::unique_ptr<Todo> todos = std::make_unique<Todo>();
    std::cout << "Displaying todo \n";

    while (Session::is_active) {
        todos->run();
    }

    return 0;
}

/* void crud_test() { */

    /* TodoItem test1{"Test1", "ONGOING"}; */
    /* TodoItem test2{"Test2", "COMPLETED"}; */
    /* TodoItem test3{"Test3", "ONGOING"}; */

    /* system("read"); */

    /* std::cout << "Deleting todo \n"; */
    /* todos->remove_todo(&test1); */
    /* todos->remove_todo(&test2); */
    /* todos->remove_todo(&test3); */
    /* for (const auto& item : todos->get_todos()) { */
    /*     std::cout << "Title: " << item.get_title() << " Status: " << item.get_status() << '\n'; */
    /* } */
/* } */
