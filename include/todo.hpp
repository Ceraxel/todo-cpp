#include <ftxui/dom/node.hpp>
#include <vector>
#include <string>
#include <string_view>
#include "pqxx/pqxx"

class TodoItem;


class Todo {
   public:
    void run();
    void display_todo(std::vector<std::string> title_list, ftxui::Elements status_list);
    void add_todo_form();
    void add_todo(std::string& title, std::string& status);
    void edit_todo(TodoItem* item);
    void update_todo(TodoItem* item, int status);
    void remove_todo(TodoItem* item);

   private:
    std::vector<TodoItem> todo_list_;
};

class TodoItem {
   public:
    TodoItem(std::string_view id, std::string_view title, std::string_view status)
        : unique_id_(id), title_{title}, status_{status} {}

    std::string get_title() const { return title_; }
    std::string get_id() const { return unique_id_; }
    std::string get_status() const { return status_; }
    void set_title();
    void set_status(std::string_view status);
    friend void Todo::run();

   private:
    std::string unique_id_{};
    std::string title_{};
    std::string status_{};
};
