//
// Created by alexey on 12/3/16.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include "Database.h"
#include "DatabaseTextFile.h"

DatabaseTextFile::DatabaseTextFile(const std::string &path) : Database(path) {}

void DatabaseTextFile::write_on_disc() {
    std::fstream file(path, std::ios::out);
    if (!file.is_open()) {
        std::cout << "Error, can't write file!";
        return;
    }

    for (auto it = users.begin(); it != users.end(); it++) {
        file << "\nUser=" << it->getName() << "\nuser_id=" << it->getObject_id() << "\n";
        for (auto it2 = books.begin(); it2 != books.end(); it2++)
            if (it2->getOwner_id() == it->getObject_id()) {
                file << "Book=" << it2->getName() << "\nbook_id=" << it2->getObject_id() << "\ntime="
                     << it2->getOwned_date_Str() << "\n";
            }
    }

    for (auto it = books.begin(); it != books.end(); it++)
        if (it->is_free())
            file << "\nbook_free_book=" << it->getName() << "\nbook_free_book_id=" << it->getObject_id();
    file.close();
}

void DatabaseTextFile::read_from_disc() {

    std::fstream file(path, std::ios::in);
    if (!file.is_open()) {
        std::cout << "Error, can't open file!";
        return;
    }
    for (std::string line; getline(file, line);) {

        if (line.length() == 0) continue;

        std::string type = line.substr(0, line.find("="));
        std::string get_data = line.substr(line.find("=") + 1, line.length());

        if (type == "user") {
            User temp(get_data);
            users.push_back(temp);
        } else if (type == "book") {
            Book temp(get_data);
            books.push_back(temp);
            books.back().setOwner_id(users.back().getObject_id());
        } else if (type == "user_id") {
            users.back().setObject_id(get_data);
        } else if (type == "book_id") {
            books.back().setObject_id(get_data);
            users.back().add_book(books.back().getObject_id());
        } else if (type == "book_free_book_id") {
            books.back().setObject_id(get_data);
        } else if (type == "time") {
            std::stringstream ss(get_data);
            std::vector<std::string> temp;
            std::string temp_str;
            while (getline(ss, temp_str, '.')) temp.push_back(temp_str);

            struct tm *mtm = new tm;
            mktime(mtm);
            mtm->tm_mday = stoi(temp[0]);
            mtm->tm_mon = stoi(temp[1]) - 1;
            mtm->tm_year = stoi(temp[2]) - 1900;
            books.back().setOwned_date(mktime(mtm));
        } else if (type == "book_free_book") {
            Book temp(get_data);
            books.push_back(temp);
        }
    }
    file.close();
}
