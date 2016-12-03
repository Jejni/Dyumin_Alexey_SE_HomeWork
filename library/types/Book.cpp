//
// Created by alexey on 11/20/16.
//

#include "Book.h"

Book::Book() {}

Book::Book(const std::string get_name) : DefaultInfo(get_name) { setOwner_id("", 0); }

void Book::free() {
    owner_id = "";
    owned_date = 0;
}

bool Book::is_free() const { return owner_id.length() == 0; }

const std::string &Book::getOwner_id() const {
    if (is_free()) return free_str;
    else return owner_id;
}

void Book::setOwner_id(const std::string &owner_id) {
    Book::owner_id = owner_id;
}

void Book::setOwner_id(const std::string &owner_id, const time_t &owned_date) {
    Book::owner_id = owner_id;
    Book::owned_date = owned_date;
}

time_t Book::getOwned_date() const {
    return owned_date;
}

std::string Book::getOwned_date_Str() const {
    int max_size = 11;
    char *temp = new char(max_size);
    strftime(temp, max_size, "%e.%m.%Y", localtime(&owned_date));
    std::string temp_str = temp;
    return temp_str;
}

void Book::setOwned_date(time_t owned_date) {
    Book::owned_date = owned_date;
}
