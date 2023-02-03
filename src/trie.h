#pragma once

#include <iostream>
#include <queue>

#define EngDict 26

struct TrieNode
{
    TrieNode* child[EngDict] = {nullptr};
    int cnt = 0; //Words count
};

class Trie {
    private:
        TrieNode* root;

    public:
        Trie();
        ~Trie();
    
    public:
        bool insert(const std::string& word);
        bool contain(const std::string& word);
};