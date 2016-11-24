//
// Created by alexey on 11/22/16.
//

//
// Created by alexey on 11/22/16.
//

#include <fstream>
#include <library.h>
#include <string>
#include <iostream>
#include <cereal/archives/portable_binary.hpp>

class data_base {
protected:
    std::string path;
public:
    data_base(const std::string &path) {
        data_base::path = path;
    }

    const std::string &getPath() const {
        return path;
    }

    void setPath(const std::string &path) {
        data_base::path = path;
    }

public:
    virtual void read_from_disc(library &library) {};

    virtual void write_on_disc(const library &library) {};
};


class data_base_serial : public data_base {
public:
    data_base_serial(const std::string &path = "/home/alexey/Dropbox/SafeBoard/Software_Engineering/Hw1/data.bin")
            : data_base(path) {};

    void write_on_disc(const library &library) {
        std::ofstream file(path, std::ios::binary);
        if (!file.is_open()) {
            std::cout << "Error, can't write serial file!";
            return;
        }
        cereal::PortableBinaryOutputArchive oarchive(file); // Create an output archive
        oarchive(library); // Write the data to the archive
        file.close();
    }

    void read_from_disc(library &library) const {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) {
            std::cout << "Error, can't open serial file!";
            return;
        }
        cereal::PortableBinaryInputArchive iarchive(file); // Create an input archive
        iarchive(library); // Read the data from the archive
        file.close();
    }
};

class data_base_serial_rw : public data_base {
public:
    data_base_serial_rw(const std::string &path = "/home/alexey/Dropbox/SafeBoard/Software_Engineering/Hw1/data.txt")
            : data_base(path) {};

    void write_on_disc(const library &library) {
        std::fstream file(path, std::ios::out);
        if (!file.is_open()) {
            std::cout << "Error, can't write file!";
            return;
        }

        for (auto it = library.getUsers().begin(); it != library.getUsers().end(); it++) {
            file << "\nuser=" << it->getName() << "\nuser_id=" << it->getObject_id() << "\n";
            for (auto it2 = library.getBooks().begin(); it2 != library.getBooks().end(); it2++)
                if (it2->getOwner_id() == it->getObject_id()) {
                    file << "book=" << it2->getName() << "\nbook_id=" << it2->getObject_id() << "\ntime="
                         << it2->getOwned_date_Str() << "\n";
                }
        }

        for (auto it = library.getBooks().begin(); it != library.getBooks().end(); it++)
            if (it->is_free())
                file << "\nbook_free_book=" << it->getName() << "\nbook_free_book_id=" << it->getObject_id();
        file.close();
    }

    void read_from_disc(library &library) const {
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
                user temp(get_data);
                library.getUsers().push_back(temp);
            } else if (type == "book") {
                book temp(get_data);
                library.getBooks().push_back(temp);
                library.getBooks().back().setOwner_id(library.getUsers().back().getObject_id());
            } else if (type == "user_id") {
                library.getUsers().back().setObject_id(get_data);
            } else if (type == "book_id") {
                library.getBooks().back().setObject_id(get_data);
                library.getUsers().back().add_book(library.getBooks().back().getObject_id());
            } else if (type == "book_free_book_id") {
                library.getBooks().back().setObject_id(get_data);
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
                library.getBooks().back().setOwned_date(mktime(mtm));
            } else if (type == "book_free_book") {
                book temp(get_data);
                library.getBooks().push_back(temp);
            }
        }
        file.close();
    }
};
