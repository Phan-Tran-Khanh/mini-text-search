#include "features.h"

Features::Features() {
    std::ifstream inFile(_metadata, std::ios_base::in);
    if (inFile.is_open()) {
        while (!inFile.eof()) {
            std::map<std::string, float> words;
            int totalWords = 0;
            std::string line;

            std::getline(inFile, line);
            this->filenames.push_back(line);

            inFile >> totalWords;
            for(int i = 0; std::getline(inFile, line) && i < totalWords; ++i) {
                std::istringstream ss(line);
                std::string word; float val;
                ss >> word >> val;
                words[word] = val;
            }
            this->metadata.push_back(words);
        }
        inFile.close();
    }
}

std::vector<std::string> Features::Parse(std::string query) {
    std::istringstream ss(query);
    std::vector<std::string> tokens;
    std::string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

std::map<float, std::string, std::greater<float>> Features::Searching(std::vector<std::string> tokens) {
    int i = 0;
    std::map<float, std::string, std::greater<float>> result;
    for(auto doc : this->metadata) {
        float score = 0;
        int wordsMatched = 0;
        for (auto token : tokens) {
            score += doc[token];
            if (doc[token] != 0) {
                ++wordsMatched;
            }
        }
        result[score * wordsMatched] += this->filenames[i] + ' ';
        ++i;
    }
    return result;
}

void Features::addDir(const std::string& dirPath) {
    PreProc pp(_swPath);
    std::vector<std::map<std::string, int>> docs;
    std::vector<std::string> filenames;
    for (const auto & entry : std::filesystem::directory_iterator(dirPath)) {
        std::string filename{entry.path().filename().u8string()};
        std::wifstream inFile(entry.path(), std::ios_base::in);
        if (inFile.is_open()) {
            std::cout << "Extracting " << filename << "...";
            filenames.push_back(filename);
            
            inFile.imbue(std::locale(
                inFile.getloc(),
                new std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>
            ));

            std::vector<std::string> words;
            for(std::wstring line, utf16W; std::getline(inFile, line);) {
                std::wistringstream ss(line);
                while (ss >> utf16W) {
                    std::string utf8W = pp.toLatin(utf16W);
                    if (!utf8W.empty()) {
                        words.push_back(utf8W);
                    }
                }
            }
            inFile.close();

            std::map<std::string, int> kws;
            for (int i = 0; i < words.size();) {
                std::string kw = "";
                int jmp = -1;
                for (int j = 0; (j < _swSize) && (i + j < words.size()); ++j) {
                    kw += words[i + j];
                    if (pp.isStopword(kw)) {
                        jmp = j;
                    }
                }
                if (jmp == -1) {
                    ++kws[words[i]];
                    ++jmp;
                }
                i += jmp + 1;
            }
            docs.push_back(kws);

            std::cout << "...100%!\n";
        }
        else {
            std::cout << "Cannot open " << filename << " !" << std::endl;
        }
    }

    std::vector<std::map<std::string, float>> metaDocs = Keywords::TFIDF_Extraction(docs);

    std::ofstream outFile(_metadata, std::ios_base::app | std::ios_base::out);
    if (outFile.is_open()) {
        int i = 0;
        for (auto doc : metaDocs) {
            outFile << filenames[i++] << std::endl;
            outFile << doc.size() << std::endl;
            for (auto kw : doc) {
                outFile << kw.first << ' ' << std::fixed << std::setprecision(5) << kw.second << std::endl;
            }
        }
        outFile.close();
    }
}
