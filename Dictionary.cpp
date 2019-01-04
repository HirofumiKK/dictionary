// Dictionary.cpp

// the only C++ library container you
// are allowed to use are vector, list, stack, and queue (and string); if you
// want anything fancier, implement it yourself).

#include "Dictionary.h"
#include <string>
#include <utility>  // for swap
using namespace std;

void removeNonLetters(string& s);
unsigned int intOf(const string &word);
unsigned int hashFunc(const string &word);
const int NumBucket = 50000;
const int primeNum[26] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101};


// This class does the real work of the implementation.
class DictionaryImpl
{
public:
    DictionaryImpl();
    ~DictionaryImpl() {}
    void insert(string word);
    void lookup(string letters, void callback(string)) const;
private:
    struct Node
    {
        string   m_word;
        Node*    m_head;
        Node*    m_next;
    };
    Node m_dictionary[NumBucket];
};

DictionaryImpl::DictionaryImpl()
{
    for(int i = 0; i < NumBucket; i++){
        m_dictionary[i].m_head = nullptr;
    }
}

void DictionaryImpl::insert(string word)
{
    removeNonLetters(word);
    if (!word.empty()){
        unsigned int index = hashFunc(word);
        if(m_dictionary[index].m_head == nullptr){
            Node* newp = new Node;
            newp->m_word = word;
            m_dictionary[index].m_head = newp;
            m_dictionary[index].m_head->m_next = nullptr;
        }else{
            Node* newp = new Node;
            newp->m_word = word;
            newp->m_next = m_dictionary[index].m_head;
            m_dictionary[index].m_head = newp;
        }
    }
}

void DictionaryImpl::lookup(string letters, void callback(string)) const
{
    if (callback == nullptr)
        return;
    
    removeNonLetters(letters);
    if (letters.empty())
        return;

    unsigned int index = hashFunc(letters);
    for(Node* ptr = m_dictionary[index].m_head; ptr != nullptr; ptr = ptr->m_next){
        // checking if the word is an anagram of the string letters
        if(intOf(letters) == intOf(ptr->m_word))
            callback(ptr->m_word);
    }
}

void removeNonLetters(string& s)
{
    string::iterator to = s.begin();
    for (string::const_iterator from = s.begin(); from != s.end(); from++)
    {
        if (isalpha(*from))
        {
            *to = tolower(*from);
            to++;
        }
    }
    s.erase(to, s.end());  // chop everything off from "to" to end.
}

unsigned int intOf(const string &word)
{
    unsigned int total = 1;
    for(int i = 0; i < word.length(); i++)
        total *= primeNum[word[i] - 'a']; // a - a = 0, b - a = 1, and so on
    return total;
}

unsigned int hashFunc(const string &word)
{
    return intOf(word) % NumBucket;
}

//******************** Dictionary functions ********************************

// These functions simply delegate to DictionaryImpl's functions.
// You probably don't want to change any of this code

Dictionary::Dictionary()
{
    m_impl = new DictionaryImpl;
}

Dictionary::~Dictionary()
{
    delete m_impl;
}

void Dictionary::insert(string word)
{
    m_impl->insert(word);
}

void Dictionary::lookup(string letters, void callback(string)) const
{
    m_impl->lookup(letters,callback);
}
