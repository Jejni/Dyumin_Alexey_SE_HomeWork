//
// Created by alexey on 11/20/16.
//

#include "Controller.h"
#include <iostream>

Controller::Controller(int type) {
    lib = new Library(type);
}

bool Controller::check_book_index(int get_index) {
    return get_index > lib->getBooks().size() - 1;
}

bool Controller::check_user_index(int get_index) {
    return get_index > lib->getUsers().size() - 1;
}

bool Controller::give_book(int book_number, int user_number) {
    book_number -= 1;
    user_number -= 1;
    if (check_book_index(book_number) || check_user_index(user_number)) return false;
    auto book_it = lib->getBooks().begin();
    auto user_it = lib->getUsers().begin();
    for (int i = 0; (i < book_number) && (book_it != lib->getBooks().end()); i++, book_it++);
    for (int i = 0; (i < user_number) && (user_it != lib->getUsers().end()); i++, user_it++);

    if (lib->can_give(*user_it, *book_it)) {
        user_it->add_book(book_it->getObject_id());
        book_it->setOwner_id(user_it->getObject_id(), time(0));
    } else return false;
    write_on_disc();
    return true;
}

bool Controller::return_book(int book_number) {
    book_number -= 1;
    if (check_book_index(book_number)) return false;
    auto it = lib->getBooks().begin();
    for (int i = 0; (i < book_number) && (it != lib->getBooks().end()); i++, it++);
    if (it->is_free()) return false;
    get_user_ref(it->getOwner_id()).return_book(it->getObject_id());
    it->free();
    write_on_disc();
    return true;
}

bool Controller::return_book(std::string book_id) {
    auto it = lib->getBooks().begin();
    for (; it->getObject_id() != book_id; it++);
    if (it->is_free()) return false;
    get_user_ref(it->getOwner_id()).return_book(it->getObject_id());
    it->free();
    write_on_disc();
    return true;
}

bool Controller::return_book(std::string u_number, std::string b_number) {
    int user_index = stoi(u_number) - 1, book_index = stoi(b_number) - 1;
    if (check_user_index(user_index)) return false;
    int check = get_user_ref(user_index).getOwns().size() - 1;
    if (check < book_index) return false;
    b_number = get_user_ref(user_index).getOwns()[book_index];
    return return_book(b_number);
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
                Book &book = get_book_ref(*b_it);
                if (!lib->check_book_date(book))
                    std::cout << "\t" << book.getName() << " " << book.getOwned_date_Str() << std::endl;
            }
        }
    }
}


void Controller::show_user_books(std::string u_index) {
    int user_undex = stoi(u_index) - 1;
    if (check_user_index(user_undex)) return;

    auto user_it = lib->getUsers().begin();
    for (int i = 0; i < user_undex; i++, user_it++);
    int i = 1;
    for (auto book_it = lib->getBooks().begin(); book_it != lib->getBooks().end(); book_it++, i++) {
        if (book_it->getOwner_id() == user_it->getObject_id())
            std::cout << i << ": " << book_it->getName() << std::endl;
    }
}

User &Controller::get_user_ref(const std::string &id) {
    for (auto it = lib->getUsers().begin(); it != lib->getUsers().end(); it++) if (it->getObject_id() == id) return *it;
    User *temp = new User("free");
    return *temp;
}

User &Controller::get_user_ref(int index) {
    if (check_user_index(index)) {
        User *temp = new User("free");
        return *temp;
    }
    auto it = lib->getUsers().begin();
    for (int i = 0; i < index; i++, it++);
    return *it;
}

Book &Controller::get_book_ref(const std::string &id) {
    for (auto it = lib->getBooks().begin(); it != lib->getBooks().end(); it++) if (it->getObject_id() == id) return *it;
    Book *temp = new Book("free");
    return *temp;
}

void Controller::show_books() {
    int i = 1;
    for (auto it = lib->getBooks().begin(); it != lib->getBooks().end(); it++, i++)
        std::cout << i << ": " << it->getName() << " (" << get_user_ref(it->getOwner_id()).getName() << ")"
                  << std::endl;
}

void Controller::read_from_disc() {
    lib->read_from_disc();
}

void Controller::write_on_disc() {
    lib->write_on_disc();
}

