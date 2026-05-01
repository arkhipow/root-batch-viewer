#pragma once

#include <string>
#include <vector>

struct Data {
    std::string name;
    std::vector<float> x;
    std::vector<float> y;
};

class File {
public:
    File(const std::string& path);

    static File open();

    const std::string& getName() const;
    const std::string& getPath() const;
    std::vector<Data>& getData();

private:
    std::string name;
    std::string path;

    std::vector<Data> data;
};

class Parser final {
public:
    static void parse(File& file);
};