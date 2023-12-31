#include <ftxui/screen/screen.hpp>
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/util/ref.hpp"

#include <pqxx/pqxx>
#include "auth.hpp"
#include "user.hpp"
#include "database.hpp"

void Auth::register_user(User* user) {
    pqxx::work txn{Database::conn};
    txn.exec("INSERT INTO accounts VALUES ('" + user->get_username() + "', '" +
             user->get_password() + "')");
    txn.commit();
}

void Auth::delete_user(User* user) {
    pqxx::work txn{Database::conn};
    txn.exec("DELETE FROM accounts WHERE username = '" + user->get_username() +
             "'");
    txn.commit();
};

void Auth::login_user(User* user) {
    pqxx::work txn{Database::conn};
    pqxx::result res{txn.exec("SELECT * FROM accounts WHERE username = '" +
                              user->get_username() + "' AND password = '" +
                              user->get_password() + "'")};
    if (!res.empty()) {
        Session::user = user->get_username();
        Session::is_authenticated = true;
    }
}

void authenticate() {
    std::optional<User> acc{};
    while (!Session::is_authenticated) {
        switch (getAuthOpt()) {
            case AuthOpt::Login:
                acc = inputCredentials(AuthOpt::Login);
                Auth::login_user(&acc.value());
                break;
            case AuthOpt::Register:
                acc = inputCredentials(AuthOpt::Register);
                Auth::register_user(&acc.value());
                break;
            case AuthOpt::Exit:
            default:
                exit(0);
        }
    }
}

AuthOpt getAuthOpt() {
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

std::optional<User> inputCredentials(AuthOpt opt) {
    using namespace ftxui;
    std::string selected_opt{opt == AuthOpt::Login ? "LOGIN" : "REGISTER"};
    auto screen = ScreenInteractive::Fullscreen();

    bool is_submitted{false};
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
        is_submitted = true;
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

    if (is_submitted)
        return User(username, password);
    return std::nullopt;
}

