//
// Created by alexey on 11/20/16.
//

#ifndef HW1_CONTROLLER_H
#define HW1_CONTROLLER_H

#include <string>
#include "Library.h"

class Controller {
    Library *lib;
public:
    Controller(int);

    bool give_book(int book_number, int user_number);

    bool return_book(int book_number);

    bool return_book(std::string u_number, std::string b_number);

    void show_users();

    void show_bad_users();

    void show_user_books(std::string u_index);

    void show_books();

    void read_from_disc();

    void write_on_disc();
};

#endif //HW1_CONTROLLER_H
