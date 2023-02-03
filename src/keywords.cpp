#include "keywords.h"

std::vector<std::map<std::string, float>> Keywords::TFIDF_Extraction(std::vector<std::map<std::string, int>> docs) {
    /*
        Count the frequency of every word in documents
    */
    std::map<std::string, int> docFreq;
    for (auto doc : docs) { 
        for (auto word : doc) {
            ++docFreq[word.first];
        }
    }

    std::vector<std::map<std::string, float>> files;
    for (auto doc : docs) {
        /*
            Count total words in each document
        */
        int totalWords = 0;
        for (auto word : doc) {
            totalWords += word.second;
        }

        /*
            Calculate term frequency multiples inverse document frequency
            of each word in each document
        */
        std::map<std::string, float> words;
        for (auto word : doc) {
            float _tfidf = ((float) word.second / (float) totalWords) * (log((float) docs.size() / (float) docFreq[word.first]));
            if (_tfidf >= _kwTFIDF) {
                words[word.first] = _tfidf;
            }
        }
        files.push_back(words);
    }
    return files;
}