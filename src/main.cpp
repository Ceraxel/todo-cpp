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
#include "todo/todo.hpp"
#include "user/user.hpp"

enum class AuthOpt { Login, Register, Exit };

AuthOpt getUserChoice();
void inputCredentials(const AuthOpt opt);

int main() {
    std::unique_ptr<Auth> auth = std::make_unique<Auth>();
    while (!Session::isAuthenticated) {
    switch (getUserChoice()) {
        case AuthOpt::Login:
            inputCredentials(AuthOpt::Login);
            break;
        case AuthOpt::Register:
            inputCredentials(AuthOpt::Register);
            break;
        case AuthOpt::Exit:
        default:
            exit(0);
    }
    }

    std::cout << "Is auth: " << Session::isAuthenticated << '\n';

    pqxx::work txn{Database::conn};
    for (auto [username, password] :
         txn.query<std::string, std::string>("SELECT * FROM accounts")) {
        std::cout << "Account {" << username << ", " << password << ", "
                  << "}" << '\n';
    }
    txn.abort();

    system("read");

    return 0;
}

AuthOpt getUserChoice() {
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
        return AuthOpt::Login;
    else if (selected == 1)
        return AuthOpt::Register;
    else
        return AuthOpt::Exit;
}

void inputCredentials(AuthOpt opt) {
    UserObj user = std::make_unique<User>();
    std::string selected_opt{opt == AuthOpt::Login ? "LOGIN" : "REGISTER"};
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
        user->setUsername(username);
        user->setPassword(password);
        if (opt == AuthOpt::Login)
            Auth::loginUser(user);
        if (opt == AuthOpt::Register)
            Auth::registerUser(user);
        return screen.ExitLoopClosure()();
    };
    auto menu = Menu(&entries, &selected, option);

    auto component =
        Container::Vertical({input_username, input_password, menu});
    component = component | center;
    auto renderer = Renderer(component, [&] {
        return window(
                   text(selected_opt) | hcenter | bold,
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
