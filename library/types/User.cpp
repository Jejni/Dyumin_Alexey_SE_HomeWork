//
// Created by alexey on 11/20/16.
//

#include "User.h"

User::User() {}

User::User(const std::string &get_name) : DefaultInfo(get_name) {}

void User::add_book(const std::string &get_book_id) {
    owns.push_back(get_book_id);
}

void User::return_book(const std::string &get_book_id) {
    for (auto it = owns.begin(); it != owns.end(); it++) {
        if (*it == get_book_id) {
            owns.erase(it);
            return;
        }
    }
}

int User::get_number_of_books() const {
    return owns.size();
}

const std::vector<std::string> &User::getOwns() const {
    return owns;
}

void User::setOwns(const std::vector<std::string> &owns) {
    User::owns = owns;
}
