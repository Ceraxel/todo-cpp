#include "todo.hpp"
#include <iostream>
#include <string>
#include "auth.hpp"
#include "database.hpp"
#include "pqxx/pqxx"

Todo::Todo() {
    pqxx::work txn{Database::conn};
    for (auto [title, status] : txn.query<std::string, std::string>(
             "SELECT title, status FROM todos WHERE user = '" + Session::user + "'")) {
        todo_list_.emplace_back(title, status);
    }
    std::cout << "Todo initialized\n";
}
void Todo::add_todo(TodoItem* item) {
    todo_list_.push_back(*item);
    pqxx::work txn{Database::conn};
    txn.exec("INSERT INTO todos VALUES ('" + Session::user + "', '" + item->get_title() + "', '" +
             item->get_status() + "')");
    txn.commit();
    std::cout << "Todo item added\n";
}
void Todo::remove_todo(TodoItem* item) {
    todo_list_.remove_if([&](const TodoItem& items) {
        return items.get_title() == item->get_title();
    });
    pqxx::work txn{Database::conn};
    txn.exec("DELETE FROM todos WHERE username = '" + Session::user + "' AND title = '" + item->get_title() + "'");
    txn.commit();
    std::cout << "Todo item removed\n";
};

std::list<TodoItem>& Todo::get_todos() {
    return todo_list_;
}
