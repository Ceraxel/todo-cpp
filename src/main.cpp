#include <ftxui/screen/screen.hpp>
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/util/ref.hpp"

#include <iostream>
#include <memory>
#include <string>
#include "auth/auth.hpp"
#include "user/user.hpp"
#include "todo/todo.hpp"

enum class MenuOpt { Login, Register, Exit };

MenuOpt getUserChoice();
void inputCredentials(UserObj& User, const std::string& type);

int main() {
    std::unique_ptr<Auth> auth = std::make_unique<Auth>();
    UserObj user = std::make_unique<User>();
    /* while (!Auth::isAuthenticated) { */
        switch (getUserChoice()) {
            case MenuOpt::Login:
                inputCredentials(user, std::string("LOGIN"));
                auth->loginUser(user);
                break;
            case MenuOpt::Register:
                inputCredentials(user, std::string("REGISTER"));
                break;
            case MenuOpt::Exit:
            default:
                exit(0);
        }
    /* } */

    Todo todolist {user};
    TodoItem item1 {"Task 1", "Complete"};
    system("read");
    todolist.addTodo(&item1);
    system("read");
    todolist.removeTodo(&item1, 0);


    pqxx::work txn{Database::conn};
    for (auto [username, password] :
         txn.query<std::string, std::string>("SELECT * FROM accounts")) {
        std::cout << "Account {" << username << ", " << password << ", "
                  << "}" << '\n';
    }
    txn.abort();

    system("read");

    /* auth->deleteUser(user); */

    return 0;
}

MenuOpt getUserChoice() {
    using namespace ftxui;
    auto screen = ScreenInteractive::Fullscreen();

    std::vector<std::string> entries = {
        "1.   Login",
        "2.   Register",
        "3.   Exit",
    };
    int selected = 0;
    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();
    auto menu = Menu(&entries, &selected, option);
    menu = menu | border | bold | center;

    auto render = Renderer(
        menu, [&] { return vbox(/* Add Text */ menu->Render()) | center; });
    screen.Loop(render);

    if (selected == 0)
        return MenuOpt::Login;
    else if (selected == 1)
        return MenuOpt::Register;
    else
        return MenuOpt::Exit;
}

void inputCredentials(UserObj& User, const std::string& type) {
    using namespace ftxui;
    auto screen = ScreenInteractive::Fullscreen();

    std::string username{};
    std::string password{};

    Component input_username = Input(&username, "username");

    InputOption password_option;
    password_option.password = true;
    Component input_password = Input(&password, "password", password_option);

    std::vector<std::string> entries{"Submit", "Cancel"};
    int selected = 0;
    MenuOption option;
    option.on_enter = [&] {
        if (selected != 0)
            return screen.ExitLoopClosure()();
        User->setUsername(username);
        User->setPassword(password);
        if (type.compare(std::string("REGISTER")) == 0)
            Auth::registerUser(User);
        return screen.ExitLoopClosure()();
    };
    auto menu = Menu(&entries, &selected, option);

    auto component =
        Container::Vertical({input_username, input_password, menu});
    component = component | center;
    auto renderer = Renderer(component, [&] {
        return window(
                   text(type) | hcenter | bold,
                   vbox({
                       hbox(text(" Username   : "), input_username->Render()),
                       hbox(text(" Password   : "), input_password->Render()),
                       separator(),
                       menu->Render(),
                   })) |
               center;
    });

    screen.Loop(renderer);
}
