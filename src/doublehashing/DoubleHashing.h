#include "../hashtable/HashTable.h"
#include "../hashfunction/HashFunction.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"

#pragma once
class DoubleHashing : public HashTable {
    public:
        DoubleHashing(int tableSize, std::string hash_name1, std::string hash_name2, RandomNumberGenerator* rnd, uint32_t notInData, int outputBits);
        DoubleHashing(uint32_t* table, int tableSize, std::string hash_name, std::string hash_name2, RandomNumberGenerator* rnd, HashFunction* hashFunction1, HashFunction* hashFunction2, uint32_t notInData, int outputBits);
        ~DoubleHashing();
        void insert(uint32_t value);
        int search(uint32_t value);
        void clean();
		void reinitHashFunction();
        HashTable* clone();
    private:
		HashFunction* initHash(std::string hash);
        uint32_t* table;
        int size;
        HashFunction* hashFunction1;
        HashFunction* hashFunction2;
        uint32_t empty;
		std::string hash_name1;
		std::string hash_name2;
		RandomNumberGenerator* rnd;
		int outputBits;
};