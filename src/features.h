#pragma once

#include "preproc.h"

class Features {
    private:
        std::vector<std::string> filenames;
        std::vector<std::map<std::string, float>> metadata;
        bool isModified = false;

    public:
        Features();

    public:
        std::vector<std::string> Parse(std::string query);
        std::map<float, std::string, std::greater<float>> Searching(std::vector<std::string> tokens);
        void addDir(const std::string& dirPath);
};