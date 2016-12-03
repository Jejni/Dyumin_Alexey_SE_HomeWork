//#include <fstream>
//#include <iostream>
//#include <cereal/types/list.hpp>
//#include <cereal/types/vector.hpp>
//#include <cereal/types/string.hpp>
//#include <cereal/types/memory.hpp>
//#include <cereal/archives/portable_binary.hpp>
//#include "Database.h"
//
//class data_base_serial : public Database {
//public:
//    data_base_serial(const std::string &path = "/home/alexey/Dropbox/SafeBoard/Software_Engineering/Hw1/data.bin")
//            : Database(path) {};
//
//    void write_on_disc() {
//        std::ofstream file(path, std::ios::binary);
//        if (!file.is_open()) {
//            std::cout << "Error, can't write serial file!";
//            return;
//        }
//        cereal::PortableBinaryOutputArchive oarchive(file); // Create an output archive
//        oarchive(users, books); // Write the data to the archive
//        file.close();
//    }
//
//    void read_from_disc() const {
//        std::ifstream file(path, std::ios::binary);
//        if (!file.is_open()) {
//            std::cout << "Error, can't open serial file!";
//            return;
//        }
//        cereal::PortableBinaryInputArchive iarchive(file); // Create an input archive
//        iarchive(users, books); // Read the data from the archive
//        file.close();
//    }
//
//    template<class Archive>
//    void serialize(Archive &archive) {
//        archive(books, users); // serialize things by passing them to the archive
//    }
//};