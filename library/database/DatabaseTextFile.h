//
// Created by alexey on 12/3/16.
//

#ifndef HW1_DATA_BASE_TEXT_FILE_H
#define HW1_DATA_BASE_TEXT_FILE_H

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "Database.h"

class DatabaseTextFile : public Database {
public:
    DatabaseTextFile(const std::string &path);

    void write_on_disc();

    void read_from_disc();
};

#endif //HW1_DATA_BASE_TEXT_FILE_H
