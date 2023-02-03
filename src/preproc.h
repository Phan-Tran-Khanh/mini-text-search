#pragma once

#include <map>
#include <vector>
#include <fstream>
#include <filesystem>

#include "trie.h"
#include "keywords.h"

#define _swPath "./stopwords.txt"
#define _swSize 4
#define _dataPath "../data"
#define _metadata "./metadata.txt"

class PreProc {
    private:
        std::wstring from = L"ÀÁÂÃÈÉÊÌÍÒÓÔÕÙÚÝàáâãèéêìíòóôõùúýĂăĐđĨĩŨũƠơƯưẠạẢảẤấẦầẨẩẪẫẬậẮắẰằẲẳẴẵẶặẸẹẺẻẼẽẾếỀềỂểỄễỆệỈỉỊịỌọỎỏỐốỒồỔổỖỗỘộỚớỜờỞởỠỡỢợỤụỦủỨứỪừỬửỮữỰựỲỳỴỵỶỷỸỹớ";
        std::wstring to = L"aaaaeeeiioooouuyaaaaeeeiioooouuyaaddiiuuoouuaaaaaaaaaaaaaaaaaaaaaaaaeeeeeeeeeeeeeeeeiiiioooooooooooooooooooooooouuuuuuuuuuuuuuyyyyyyyyo";
        Trie sws;

    public:
        PreProc(const std::string& filename);

    public:
        bool isUpper(const wchar_t& c);
        bool isLower(const wchar_t& c);
        bool isNumber(const wchar_t& c);
        bool isStopword(const std::string& w);
        wchar_t toNonAccent(const wchar_t& letter);
        std::string toLatin(const std::wstring& w);
        std::vector<std::string> getDirectories(const std::string& path);

    public:
        static void exec();
};