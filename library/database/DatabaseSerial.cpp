#include <fstream>
#include <iostream>
#include <cereal/types/list.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/archives/portable_binary.hpp>
#include "DatabaseSerial.h"

DatabaseSerial::DatabaseSerial(const std::string &path) : Database(path) {
    read_from_disc();
};

void DatabaseSerial::write_on_disc() {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Error, can't write serial file!";
        return;
    }
    cereal::PortableBinaryOutputArchive oarchive(file); // Create an output archive
    oarchive(users, books); // Write the data to the archive
    file.close();
}

void DatabaseSerial::read_from_disc() {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Error, can't open serial file!";
        return;
    }
    cereal::PortableBinaryInputArchive iarchive(file); // Create an input archive
    iarchive(users, books); // Read the data from the archive
    file.close();
}
