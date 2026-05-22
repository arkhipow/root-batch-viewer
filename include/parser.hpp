#pragma once

#include <string>
#include <vector>

struct Data {
    std::string cname;

    std::string name;
    std::vector<float> x;
    std::vector<float> y;

    std::string axisX;
    std::string axisY;

    int bins;
};

class File {
public:
    File(const std::string& path);

    static File open();

    const std::string& getName() const;
    const std::string& getPath() const;
    std::vector<Data>& getData();

    static std::string save();

private:
    std::string name;
    std::string path;

    std::vector<Data> data;
};

class Parser final {
public:
    static void parse(File& file);
};