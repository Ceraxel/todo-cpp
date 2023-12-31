#include "todo.hpp"
#include <iostream>
#include <string>
#include "pqxx/pqxx"
#include "user.hpp"
#include "database.hpp"

Todo::Todo(User* user) {
    pqxx::work txn{Database::conn};
    for (auto [title, status] : txn.query<std::string, std::string>(
             "SELECT * FROM todos WHERE user = '" + user->get_username() +
             "'")) {
         std::cout << "Todo initialized\n";
    }
}
void Todo::add_todo(TodoItem* item) {
    todo_list_.push_back(*item); 
    pqxx::work txn{Database::conn};
    txn.exec("INSERT INTO todos VALUES ('" + item->get_title() + "', '" +
             item->get_status() + "')");
    txn.commit();
    std::cout << "Todo item added\n";
}
void Todo::remove_todo(TodoItem* item, int idx) {
    todo_list_.remove_if([&](const TodoItem& items){
        return items.get_title() == item->get_title();
    });
    pqxx::work txn{Database::conn};
    txn.exec("DELETE FROM todos WHERE title = '" + item->get_title() + "'");
    txn.commit();
    std::cout << "Todo item removed\n";
};
