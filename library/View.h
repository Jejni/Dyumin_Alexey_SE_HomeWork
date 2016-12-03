//
// Created by alexey on 11/20/16.
//

#ifndef HW1_VIEW_H
#define HW1_VIEW_H

#include "Controller.h"
#include <iostream>

class View {
    Controller *con;
protected:
    void show_users();

    /*
   Пользователи с просроченными книгами.
   */
    void show_bad_users();

    void show_books();

    /*
   Перегрузка, чтобы не вводить зановво номер пользователя.
   */
    std::string user_select_menu(std::string user_number);

    std::string user_select_menu();

    std::string books_give_menu(int book_index);

    std::string books_select_menu(int book_number);

    std::string books_select_menu();

    std::string books_menu();

    std::string users_menu();

    std::string main_menu() const;

public:
    /*
     Бесконечный цикл меню с условием выхода
     Каждое из подменю возвращает строку для мониторинга местонахождения пользователя     
     */

    View();

    void start_routine();
};


#endif //HW1_VIEW_H
