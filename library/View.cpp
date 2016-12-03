//
// Created by alexey on 11/20/16.
//

#include "View.h"

void View::show_users() {
    con->show_users();
}

void View::show_bad_users() {
    con->show_bad_users();
}

void View::show_books() {
    con->show_books();
}

std::string View::user_select_menu(std::string user_number) {
    std::string choice;
    std::cout << "\nActions: list User's books - 1, free Book - 2, add Book - 3, back - q: ";
    std::cin >> choice;

    switch (*choice.c_str()) {
        case '1': {
            con->show_user_books(user_number);
            break;
        }
        case '2': {
            std::string book_number;
            std::cout << "Book number: ";
            std::cin >> book_number;

            if (con->return_book(user_number, book_number)) std::cout << "Done!\n";
            else std::cout << "Error!\n";
            return user_select_menu(user_number);
        }
        case '3': {
            show_books();
            std::string book_number;
            std::cout << "Book number: ";
            std::cin >> book_number;

            if (con->give_book(stoi(book_number), stoi(user_number))) std::cout << "Done!\n";
            else std::cout << "Error!\n";
            break;
        }
        case 'q': {
            return "user_select_menu_back";
        }

        default: {
            std::cout << "Error!\n";
            return user_select_menu(user_number);
        }
    }
    return user_select_menu(user_number);
}

std::string View::user_select_menu() {
    std::string user_number;
    std::cout << "User number: ";
    std::cin >> user_number;

    std::string choice;
    std::cout << "\nActions: list User's books - 1, free Book - 2, add Book - 3, back - q: ";
    std::cin >> choice;

    switch (*choice.c_str()) {
        case '1': {
            con->show_user_books(user_number);
            break;
        }
        case '2': {
            std::string book_number;
            std::cout << "Book number: ";
            std::cin >> book_number;

            if (con->return_book(user_number, book_number)) std::cout << "Done!\n";
            else std::cout << "Error!\n";
            return user_select_menu(user_number);
        }
        case '3': {
            show_books();
            std::string book_number;
            std::cout << "Book number: ";
            std::cin >> book_number;

            if (con->give_book(stoi(book_number), stoi(user_number))) std::cout << "Done!\n";
            else std::cout << "Error!\n";
            break;
        }
        case 'q': {
            return "user_select_menu_back";
        }

        default: {
            std::cout << "Error!\n";
            return user_select_menu(user_number);
        }
    }
    return user_select_menu(user_number);
}

std::string View::books_give_menu(int book_index) {
    show_users();

    std::string choice;
    std::cout << "Give to (back - q): ";
    std::cin >> choice;

    if (choice == "q") return "books_give_menu_back";

    if (con->give_book(book_index, stoi(choice))) std::cout << "Done!\n";
    else std::cout << "Error!\n";
    return "books_give_menu_back";
}

std::string View::books_select_menu(int book_number) {
    std::string choice;
    std::cout << "\nActions: free - 1, give - 2, back - q: ";
    std::cin >> choice;

    switch (*choice.c_str()) {
        case '1': {
            if (con->return_book(book_number)) std::cout << "Done!\n";
            else std::cout << "Error!\n";
            return "books_menu";
        }
        case '2': {
            return books_give_menu(book_number);
        }
        case 'q': {
            return "books_select_menu_back";
        }
        default: {
            std::cout << "Error!\n";
            return books_select_menu();
        }
    }
}

std::string View::books_select_menu() {
    std::string book_number;
    std::cout << "Book number: ";
    std::cin >> book_number;

    std::string choice;
    std::cout << "\nActions: free - 1, give - 2, back - q: ";
    std::cin >> choice;

    switch (*choice.c_str()) {
        case '1': {
            if (con->return_book(stoi(book_number))) std::cout << "Done!\n";
            else std::cout << "Error!\n";
            return "books_menu";
        }
        case '2': {
            return books_give_menu(stoi(book_number));
        }
        case 'q': {
            return "books_select_menu_back";
        }
        default: {
            std::cout << "Error!\n";
            return books_select_menu(stoi(book_number));
        }
    }
}

std::string View::books_menu() {
    std::string choice;
    std::cout << "\nList books - 1, Select Book - 2, back - q: ";
    std::cin >> choice;

    switch (*choice.c_str()) {
        case '1': {
            show_books();
            return "books_menu";
        }
        case '2': {
            return books_select_menu();
        }
        case 'q': {
            return "books_menu_back";
        }
        default: {
            std::cout << "Error!\n";
            return books_menu();
        }
    }
}

std::string View::users_menu() {
    std::string choice;
    std::cout << "\nList users - 1, Select User - 2, List bad users - 3, back - q: ";
    std::cin >> choice;

    switch (*choice.c_str()) {
        case '1': {
            show_users();
            break;
        }
        case '2': {
            return user_select_menu();
        }
        case '3': {
            show_bad_users();
            break;
        }
        case 'q': {
            return "users_menu_back";
        }
        default: {
            std::cout << "Error!\n";
            return users_menu();
        }
    }
    return "users_menu";
}

std::string View::main_menu() const {
    std::string choice;
    std::cout << "\nUsers - 1, Books - 2, quit - q: ";
    std::cin >> choice;

    switch (*choice.c_str()) {
        case '1': {
            return "users_menu";
        }
        case '2': {
            return "books_menu";
        }
        case 'q': {
            return "quit";
        }
        default: {
            std::cout << "Error!\n";
            return main_menu();
        }
    }
}

void View::start_routine() {
    std::string menu = "main";
    for (;;) {
        if (menu == "main" || menu == "users_menu_back" || menu == "books_menu_back") menu = main_menu();
        else if (menu == "users_menu" || menu == "user_select_menu_back") menu = users_menu();
        else if (menu == "books_menu" || menu == "books_select_menu_back" || menu == "books_give_menu_back")
            menu = books_menu();
        else if (menu == "quit") {
            con->write_on_disc();
            return;
        }
    }
}

View::View() {
    std::cout << "Data Source:\n\t Text - 1, Bin - 2: ";
    int type;
    std::cin >> type;
    con = new Controller(type);
    start_routine();
}
