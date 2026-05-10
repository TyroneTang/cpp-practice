module;

#include <ios>
#include <string>
#include <iostream>

export module csv;

class FileHandler {
    public:
        explicit FileHandler(const std::string& filename, std::ios::openmode mode = std::ios::in);
        ~FileHandler();

};

class CSVParser {

};