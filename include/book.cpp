//
// Created by alexey on 11/20/16.
//

#include "book.h"

book::book() {}

book::book(const std::string get_name) : default_info(get_name) { setOwner_id("", 0); }

void book::free() {
    owner_id = "";
    owned_date = 0;
}

bool book::is_free() const { return owner_id.length() == 0; }

const std::string &book::getOwner_id() const {
    if (is_free()) return free_str;
    else return owner_id;
}

void book::setOwner_id(const std::string &owner_id) {
    book::owner_id = owner_id;
}

void book::setOwner_id(const std::string &owner_id, const time_t &owned_date) {
    book::owner_id = owner_id;
    book::owned_date = owned_date;
}

time_t book::getOwned_date() const {
    return owned_date;
}

std::string book::getOwned_date_Str() const {
    int max_size = 11;
    char *temp = new char(max_size);
    strftime(temp, max_size, "%e.%m.%Y", localtime(&owned_date));
    std::string temp_str = temp;
    return temp_str;
}

void book::setOwned_date(time_t owned_date) {
    book::owned_date = owned_date;
}
