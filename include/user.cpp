//
// Created by alexey on 11/20/16.
//

#include "user.h"

user::user() {}

user::user(const std::string &get_name) : default_info(get_name) {}

void user::add_book(const std::string &get_book_id) {
    owns.push_back(get_book_id);
}

void user::return_book(const std::string &get_book_id) {
    for (auto it = owns.begin(); it != owns.end(); it++) {
        if (*it == get_book_id) {
            owns.erase(it);
            return;
        }
    }
}

int user::get_number_of_books() const {
    return owns.size();
}

const std::vector<std::string> &user::getOwns() const {
    return owns;
}

void user::setOwns(const std::vector<std::string> &owns) {
    user::owns = owns;
}
