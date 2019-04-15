#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>
using namespace std;

template<typename ValueType>
struct TrieNode;

template<typename ValueType>
struct TrieNode
{
    vector<TrieNode<ValueType>*> m_children;
    vector<char> m_keys;
    vector<ValueType> m_values;
};

template<typename ValueType>
class Trie
{
public:
    Trie();
    ~Trie();
    void reset();
    void insert(const std::string& key, const ValueType& value);
    std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const;
      // C++11 syntax for preventing copying and assignment
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
private:
    TrieNode<ValueType>* root;
    void insertHelper(const std::string& key, const ValueType& value,
                      TrieNode<ValueType> *curr);
    void resetHelper(TrieNode<ValueType> *curr);
    vector<ValueType> findHelper(const std::string& key, bool exactMatchOnly,
                                 TrieNode<ValueType> *curr) const;
    
};

template<typename ValueType>
Trie<ValueType>::Trie(){
    root = new TrieNode<ValueType>;
}

template<typename ValueType>
Trie<ValueType>::~Trie(){
    delete root;
}

template<typename ValueType>
void Trie<ValueType>::reset(){
    resetHelper(root);
    delete root;
    root = new TrieNode<ValueType>;
}

template<typename ValueType>
void Trie<ValueType>::resetHelper(TrieNode<ValueType> *curr){
    for (int i = 0; i < curr->m_children.size(); i++){
        resetHelper(curr->m_children[i]);
        delete curr->m_children[i];
    }
}

template<typename ValueType>
void Trie<ValueType>::insert(const std::string& key, const ValueType& value){
    insertHelper(key, value, root);
}

template<typename ValueType>
void Trie<ValueType>::insertHelper(const std::string& key, const ValueType& value,
                                            TrieNode<ValueType> *curr){
    if (key.empty()){
        curr->m_values.push_back(value);
        return;
    }
    int i;
    for (i = 0; i < curr->m_keys.size(); i++)
        if (key[0] == curr->m_keys[i]){
            insertHelper(key.substr(1), value, curr->m_children[i]);
            return;
        }
    curr->m_keys.push_back(key[0]);
    curr->m_children.push_back(new TrieNode<ValueType>);
    insertHelper(key.substr(1), value, curr->m_children[i]);
}

template<typename ValueType>
vector<ValueType> Trie<ValueType>::find(const std::string& key, bool exactMatchOnly) const{
    TrieNode<ValueType>* curr = root;
    vector<ValueType> res;
    for (int i = 0; i < curr->m_keys.size(); i++){
        if (key[0] == curr->m_keys[i]){
            res = findHelper(key.substr(1), exactMatchOnly, curr->m_children[i]);
        }
    }

    return res;
}

template<typename ValueType>
vector<ValueType> Trie<ValueType>::findHelper(const std::string &key, bool exactMatchOnly, TrieNode<ValueType> *curr) const{
    vector<ValueType> result;
    if (key.empty()){
        result.insert(result.end(), curr->m_values.begin(), curr->m_values.end());
        return result;
    }
    
    for (int i = 0; i < curr->m_keys.size(); i++){
        if (exactMatchOnly && key[0] == curr->m_keys[i]){
            return findHelper(key.substr(1), true, curr->m_children[i]);
        }else if(!exactMatchOnly){
            vector<ValueType> tmp;
            if (key[0] != curr->m_keys[i]){
                tmp = findHelper(key.substr(1), true, curr->m_children[i]);
            }else{
                tmp = findHelper(key.substr(1), false, curr->m_children[i]);
            }
            result.insert(result.end(), tmp.begin(), tmp.end());
        }
    }
    return result;
}


#endif // TRIE_INCLUDED
