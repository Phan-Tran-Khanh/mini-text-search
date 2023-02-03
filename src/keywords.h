#pragma once

#include <map>
#include <vector>
#include <cmath>

#define _kwTFIDF 0.0004

class Keywords {
public:
    static std::vector<std::map<std::string, float>> TFIDF_Extraction(std::vector<std::map<std::string, int>> docs);
};