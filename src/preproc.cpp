#include "preproc.h"

PreProc::PreProc(const std::string& filename) {
    std::ifstream inFile(filename, std::ios_base::in);
    if (inFile.is_open()) {
        std::cout << "Reading " << filename << std::endl;
        std::string sw;
        while (!inFile.eof()) {
            std::getline(inFile, sw);
            this->sws.insert(sw);
        }
        inFile.close();
    }
    else {
        // Show Error
        std::cout << "Cannot open " << filename << std::endl;
    }
}

wchar_t PreProc::toNonAccent(const wchar_t& letter) {
    for (auto lt1 = this->from.begin(), lt2 = this->to.begin(); lt1 != this->from.end(); ++lt1, ++lt2) {
        if (*lt1 == letter) {
            return *lt2;
        }
    }
    return L' ';
}

bool PreProc::isUpper(const wchar_t& c) {
    return ('A' <= c && c <= 'Z');
}

bool PreProc::isLower(const wchar_t& c) {
    return ('a' <= c && c <= 'z');
}

bool PreProc::isNumber(const wchar_t& c) {
    return ('0' <= c && c <= '9');
}

bool PreProc::isStopword(const std::string& w) {
    return this->sws.contain(w);
}

std::string PreProc::toLatin(const std::wstring& w) {
    std::wstring latinW;
    for (auto lt : w) {
        if (this->isUpper(lt)) {
            latinW += wchar_t(lt + (L'a' - L'A'));
        }
        else if (!this->isLower(lt) && !this->isNumber(lt)) {
            wchar_t wc = this->toNonAccent(lt);
            if (wc != L' ') {
                latinW += wc;
            }
        }
        else {
            latinW += lt;
        }
    }
    return std::string(latinW.begin(), latinW.end());
}

std::vector<std::string> PreProc::getDirectories(const std::string& path) {
    std::vector<std::string> dirs;
    for(auto& p : std::filesystem::recursive_directory_iterator(path)) {
        if (p.is_directory()) {
            dirs.push_back(p.path().u8string());
        }
    }
    return dirs;
}

void PreProc::exec() {
    PreProc pp(_swPath);
    std::vector<std::string> folders = pp.getDirectories(_dataPath);
    for (auto folder : folders) {
        /*
            Traverse all documents in every folder
        */
        std::vector<std::map<std::string, int>> docs;
        std::vector<std::string> filenames;
        for (const auto & entry : std::filesystem::directory_iterator(folder)) {
            std::string filename{entry.path().filename().u8string()};
            std::wifstream inFile(entry.path(), std::ios_base::in);
            if (inFile.is_open()) {
                std::cout << "Extracting " << filename << "...";
                filenames.push_back(filename);
                /* 
                    Tell wifstream that the file is encoded in UTF-16LE
                */
                //  std::consume_header: Consume the BOM of file stream
                inFile.imbue(std::locale(
                    inFile.getloc(),
                    new std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>
                ));

                /* 
                    Remove Vietnamese Accents of each word on article.
                */
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

                /* 
                    Remove stop words from the article.
                */
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
                // Show Error
                std::cout << "Cannot open " << filename << " !" << std::endl;
            }
        }

        /* 
            Extract keywords using Term Frequency – Inverse Document Frequency formula.
        */
        std::vector<std::map<std::string, float>> metaDocs = Keywords::TFIDF_Extraction(docs);

        /* 
            Write data of each folder (topic of content) in metadata.
        */
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
}