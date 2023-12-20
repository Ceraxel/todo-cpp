#include "todo.hpp"
#include <iostream>
#include <string>
#include "auth/auth.hpp"
#include "pqxx/pqxx"
#include "user/user.hpp"

Todo::Todo(UserObj& user) {
    pqxx::work txn{Database::conn};
    for (auto [title, status] : txn.query<std::string, std::string>(
             "SELECT * FROM todos WHERE user = '" + user->getUsername() +
             "'")) {
         std::cout << "Todo initialized\n";
    }
}
void Todo::addTodo(TodoItem* item) {
    todoList_.push_back(*item); 
    pqxx::work txn{Database::conn};
    txn.exec("INSERT INTO todos VALUES ('" + item->getTitle() + "', '" +
             item->getStatus() + "')");
    txn.commit();
    std::cout << "Todo item added\n";
}
void Todo::removeTodo(TodoItem* item, int idx) {
    todoList_.remove_if([&](const TodoItem& items){
        return items.getTitle() == item->getTitle();
    });
    pqxx::work txn{Database::conn};
    txn.exec("DELETE FROM todos WHERE title = '" + item->getTitle() + "'");
    txn.commit();
    std::cout << "Todo item removed\n";
};
