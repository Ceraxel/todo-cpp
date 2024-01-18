#include "todo.hpp"
#include <algorithm>
#include <memory>
#include <string>
#include "auth.hpp"
#include "database.hpp"
#include "pqxx/pqxx"

#include <ftxui/dom/node.hpp>
#include <ftxui/screen/screen.hpp>
#include <vector>
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/util/ref.hpp"

void TodoItem::set_status(std::string_view status) {
    status_ = status;
}

void Todo::run() {
    pqxx::work txn{Database::conn};
    using namespace ftxui;
    std::vector<std::string> title_list{};
    Elements status_list;

    for (auto [id, title, status] :
         txn.query<std::string, std::string, std::string>(
             "SELECT id, title, status FROM todos WHERE username = '" +
             Session::user + "'")) {
        todo_list_.emplace_back(id, title, status);
        title_list.emplace_back(title);
        status_list.emplace_back(text(status));
    }
    txn.abort();

    display_todo(title_list, status_list);
}

void Todo::display_todo(std::vector<std::string> title_list,
                        ftxui::Elements status_list) {
    using namespace ftxui;

    auto screen = ScreenInteractive::Fullscreen();

    int selected_todo = 0;
    MenuOption todo_list_opt;
    todo_list_opt.on_enter = [&] {
        edit_todo(&todo_list_[selected_todo]);
        screen.ExitLoopClosure()();
    };
    auto todo_list = Menu(&title_list, &selected_todo, todo_list_opt);

    std::vector<std::string> entries = {"Add todo", "Exit"};
    int selected_action = 0;
    MenuOption action_option;
    action_option.on_enter = [&] {
        if (selected_action != 0){
            Session::is_active = false;
            return screen.ExitLoopClosure()();
        }
        add_todo_form();
        screen.ExitLoopClosure()();
    };
    auto actionMenu = Menu(&entries, &selected_action, action_option);
    actionMenu = actionMenu | bold | center | hcenter;

    auto layout = Container::Vertical({todo_list, actionMenu});
    auto render = Renderer(layout, [&] {
        return vbox(window(text("LIST OF TODO") | hcenter | bold,
                           hbox(vbox(text("TASKS") | center | bold, separator(),
                                     todo_list->Render()) |
                                    flex,
                                separator(),
                                vbox(text("STATUS") | center | bold,
                                     separator(), status_list | center | bold) |
                                    flex) |
                               size(WIDTH, GREATER_THAN, 50) | center),

                    actionMenu->Render() | center) |
               center;
    });
    screen.Loop(render);
};

void Todo::add_todo_form() {
    using namespace ftxui;
    auto screen = ScreenInteractive::Fullscreen();
    std::string title;
    std::string status;

    auto text_area = Input(&title, "Title");
    text_area = text_area | border | hcenter | size(WIDTH, GREATER_THAN, 50);

    std::vector<std::string> entries = {"Ok", "Cancel"};
    int selected{};
    MenuOption menuOption;
    menuOption.on_enter = screen.ExitLoopClosure();
    Component action_menu = Menu(&entries, &selected, menuOption);
    action_menu = action_menu | center | bold;

    std::vector<std::string> status_list{"ONGOING", "COMPLETED"};
    int selected_status{};
    auto status_menu = Radiobox(&status_list, &selected_status) | center;
    auto layout = Container::Vertical({text_area, status_menu, action_menu}) |
                  border | hcenter;

    auto component = Renderer(layout, [&] {
        return vbox(
                   {vbox({text("ADD TASK") | center | bold, text_area->Render(),
                          status_menu->Render(), separatorEmpty(), separator(),
                          action_menu->Render()}) |
                    border}) |
               center;
    });
    screen.Loop(component);
    status = (selected_status == 0) ? "ONGOING" : "COMPLETED";
    if (selected != 0 || title.empty()) {
        return;
    }

    add_todo(title, status);
}

void Todo::add_todo(std::string& title, std::string& status) {
    pqxx::work txn{Database::conn};
    txn.exec("INSERT INTO todos (username, title, status) VALUES ('" +
             Session::user + "', '" + title + "', '" + status + "')");
    txn.commit();
}

void Todo::edit_todo(TodoItem* item) {
    using namespace ftxui;
    auto screen = ScreenInteractive::Fullscreen();

    std::vector<std::string> task_status{"ONGOING", "COMPLETED"};
    int chosen_status = (item->get_status() == "COMPLETED") ? 1 : 0;
    RadioboxOption option;
    auto status = Radiobox(&task_status, &chosen_status);

    std::vector<std::string> action_list{"Update", "Delete", "Cancel"};
    int selected_action{};
    MenuOption action;
    action.on_enter = [&] {
        switch (selected_action) {
            case 0:
                update_todo(item, chosen_status);
                screen.ExitLoopClosure()();
                break;
            case 1: /* delete */;
                remove_todo(item);
                screen.ExitLoopClosure()();
                break;
            case 2:
            default:
                screen.ExitLoopClosure()();
        }
    };
    auto menu = Menu(&action_list, &selected_action, action);

    auto layout = Container::Vertical({status, menu});
    auto renderer = Renderer(layout, [&] {
        return vbox(vbox(text(item->get_title()) | hcenter | bold, separator(),
                         separatorEmpty(), status->Render() | center,
                         separatorEmpty(), separator(),
                         menu->Render() | center) |
                    border | size(WIDTH, GREATER_THAN, 50)) |
               center | border;
    });

    screen.Loop(renderer);
}

void Todo::update_todo(TodoItem* item, int status) {
    std::string task_status = (status == 0) ? "ONGOING" : "COMPLETED";
    item->set_status(task_status);
    pqxx::work txn{Database::conn};
    std::cout << "Task status: " << task_status
              << " Title: " << item->get_title();
    txn.exec("UPDATE todos SET status = '" + task_status + "' WHERE title = '" +
             item->get_title() + "';");
    /* txn.commit(); */
}

void Todo::remove_todo(TodoItem* item) {
    todo_list_.erase(std::remove_if(
        todo_list_.begin(), todo_list_.end(), [&](const TodoItem& items) {
            return items.get_id() == item->get_id();
        }));
    pqxx::work txn{Database::conn};
    txn.exec("DELETE FROM todos WHERE username = '" + Session::user +
             "' AND id = '" + item->get_id() + "'");
    txn.commit();
};
