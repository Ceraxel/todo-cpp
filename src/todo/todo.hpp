#include <list>
#include <string>
#include <string_view>
#include "pqxx/pqxx"
#include "user/user.hpp"

class TodoItem {
   public:
    TodoItem(std::string_view title, std::string_view status)
        : title_{title}, status_{status} {}

    std::string getTitle() const { return title_; }
    std::string getStatus() const { return status_; }

   private:
    std::string title_{};
    std::string status_{};
};

class Todo {
   public:
    Todo(User* user);
    void addTodo(TodoItem* item);
    void removeTodo(TodoItem* item, int idx);


   private:
    std::list<TodoItem> todoList_;
};
