#include "../hashtable/HashTable.h"
#include "../hashfunction/HashFunction.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"
#include <string>

#pragma once
class Cuckoo : public HashTable {
    public:
        Cuckoo(int tableSize, std::string h1, std::string h2, RandomNumberGenerator* rnd, int outputBits, uint32_t notInData);
        Cuckoo(uint32_t* table, int tableSize, std::string h1, std::string h2, RandomNumberGenerator* rnd, int outputBits, uint32_t notInData, HashFunction* h1_func, HashFunction* h2_func);
        ~Cuckoo();
        void insert(uint32_t value);
        int search(uint32_t value);
        void clean();
		void reinitHashFunction();
        HashTable* clone();
    private:
        HashFunction * initHash(std::string hash);
        void rehash();
        bool rehashInsert(uint32_t value);

        uint32_t* table;
        int size;
        HashFunction * h1;
        HashFunction * h2;
        RandomNumberGenerator * rnd;

        int outputBits;
        std::string h1_name;
        std::string h2_name;

        uint32_t empty;
};