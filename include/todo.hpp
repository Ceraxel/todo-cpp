#include <list>
#include <string>
#include <string_view>
#include "pqxx/pqxx"

class TodoItem {
   public:
    TodoItem(std::string_view title, std::string_view status)
        : title_{title}, status_{status} {}

    std::string get_title() const { return title_; }
    std::string get_status() const { return status_; }

   private:
    std::string title_{};
    std::string status_{};
};

class Todo {
   public:
    Todo();
    void add_todo(TodoItem* item);
    void remove_todo(TodoItem* item);
    std::list<TodoItem>& get_todos();


   private:
    std::list<TodoItem> todo_list_;
};
