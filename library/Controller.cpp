//
// Created by alexey on 11/20/16.
//

#include "Controller.h"
#include <iostream>

Controller::Controller(int type) {
    lib = new Library(type);
}

bool Controller::give_book(int book_number, int user_number) {
    book_number -= 1; //now index :)
    user_number -= 1;
    if (lib->check_book_index(book_number) || lib->check_user_index(user_number)) return false;
    auto book_it = lib->getBooks().begin();
    auto user_it = lib->getUsers().begin();
    for (int i = 0; (i < book_number) && (book_it != lib->getBooks().end()); i++, book_it++);
    for (int i = 0; (i < user_number) && (user_it != lib->getUsers().end()); i++, user_it++);
    lib->give_book(*user_it, *book_it);
}

bool Controller::return_book(int book_number) {
    book_number -= 1;
    if (lib->check_book_index(book_number)) return false;
    auto it = lib->getBooks().begin();
    for (int i = 0; (i < book_number) && (it != lib->getBooks().end()); i++, it++);
    lib->return_book(*it);
}

bool Controller::return_book(std::string u_number, std::string b_number) {
    int user_index = stoi(u_number) - 1, book_index = stoi(b_number) - 1;
    if (lib->check_user_index(user_index)) return false;
    int check = lib->get_user_ref(user_index).getOwns().size() - 1;
    if (check < book_index) return false;
    b_number = lib->get_user_ref(user_index).getOwns()[book_index]; //now id :)
    lib->return_book(lib->get_book_ref(b_number));
}

void Controller::show_users() {
    int i = 1;
    for (auto it = lib->getUsers().begin(); it != lib->getUsers().end(); it++, i++)
        std::cout << i << ": " << it->getName() << " (" << it->get_number_of_books() << ")" << std::endl;
}

void Controller::show_bad_users() {
    int i = 1;
    for (auto it = lib->getUsers().begin(); it != lib->getUsers().end(); it++, i++) {
        if (!lib->check_user_date(*it)) {
            std::cout << i << ": " << it->getName() << " (" << it->get_number_of_books() << ")" << std::endl;
            for (auto b_it = it->getOwns().begin(); b_it != it->getOwns().end(); b_it++) {
                Book &book = lib->get_book_ref(*b_it);
                if (!lib->check_book_date(book))
                    std::cout << "\t" << book.getName() << " " << book.getOwned_date_Str() << std::endl;
            }
        }
    }
}


void Controller::show_user_books(std::string u_index) {
    int user_undex = stoi(u_index) - 1;
    if (lib->check_user_index(user_undex)) return;

    auto user_it = lib->getUsers().begin();
    for (int i = 0; i < user_undex; i++, user_it++);
    int i = 1;
    for (auto book_it = lib->getBooks().begin(); book_it != lib->getBooks().end(); book_it++) {
        if (book_it->getOwner_id() == user_it->getObject_id()) {
            std::cout << i << ": " << book_it->getName() << std::endl;
            i++;
        }
    }
}

void Controller::show_books() {
    int i = 1;
    for (auto it = lib->getBooks().begin(); it != lib->getBooks().end(); it++, i++)
        std::cout << i << ": " << it->getName() << " (" << lib->get_user_ref(it->getOwner_id()).getName() << ")"
                  << std::endl;
}

void Controller::read_from_disc() {
    lib->read_from_disc();
}

void Controller::write_on_disc() {
    lib->write_on_disc();
}

