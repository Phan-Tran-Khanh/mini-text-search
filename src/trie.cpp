#include "trie.h"

Trie::Trie() {
    this->root = new TrieNode;
}

bool Trie::insert(const std::string& word) {
    TrieNode* p = this->root;
    bool isNew = false;
    int z = 0;
    for(int i = 0; i < word.length(); ++i) {
        if (word[i] != ' ') {
            int letter = word[i] - 'a';
            if (!p->child[letter]) {
                p->child[letter] = new TrieNode;
                ++z;
                isNew = true;
            }
            p = p->child[letter];
        }
    }
    ++p->cnt;
    return isNew;
}

bool Trie::contain(const std::string& word) {
    TrieNode* p = this->root;
    for (int i = 0; i < word.length(); ++i) {
        int letter = word[i] - 'a';
        if (letter < 0 || !p->child[letter])
            return false;
        p = p->child[letter];
    }
    return p->cnt > 0;
}

Trie::~Trie() {
    if (this->root) {
        std::queue<TrieNode*> q;
        q.push(this->root);
        while (!q.empty())
        {
            TrieNode* pnt = q.front();
            for (int i = 0; i < EngDict; ++i) {
                if (pnt->child[i]) {
                    q.push(pnt->child[i]);
                }
            }
            q.pop();
            delete pnt; pnt = nullptr;
        }
    }
}