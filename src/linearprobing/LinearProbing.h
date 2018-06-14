#include "../hashtable/HashTable.h"
#include "../hashfunction/HashFunction.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"

#pragma once
class LinearProbing : public HashTable {
    public:
        LinearProbing(int tableSize, std::string hash_name, RandomNumberGenerator* rnd, uint32_t notInData, int outputBits);
        LinearProbing(uint32_t* table, int tableSize, std::string hash_name, RandomNumberGenerator* rnd, HashFunction* hashFunction, uint32_t notInData, int outputBits);
        ~LinearProbing();
        void insert(uint32_t value);
        int search(uint32_t value);
        void clean();
		void reinitHashFunction();
        HashTable* clone();
    private:
		HashFunction* initHash(std::string hash);
        uint32_t* table;
        int size;
        HashFunction* hashFunction;
        uint32_t empty;
		std::string hash_name;
		RandomNumberGenerator* rnd;
		int outputBits;
};