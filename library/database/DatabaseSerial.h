//
// Created by alexey on 12/3/16.
//

#ifndef HW1_DATABASESERIAL_H
#define HW1_DATABASESERIAL_H

#include "Database.h"

class DatabaseSerial : public Database {
public:
    DatabaseSerial(const std::string &path);

    void write_on_disc();

    void read_from_disc();

};

#endif //HW1_DATABASESERIAL_H
