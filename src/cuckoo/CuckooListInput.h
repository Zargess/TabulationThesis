#include "../hashtable/HashTableListInput.h"
#include "../hashfunction/HashFunction.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"
#include <string>

#pragma once
class CuckooListInput : public HashTableListInput {
    public:
        CuckooListInput(int tableSize, uint32_t notInData, std::vector<HashFunction*>* sequenceOfHashFunctions);
        CuckooListInput(uint32_t* table, int tableSize, uint32_t notInData, HashFunction* h1, HashFunction* h2, std::vector<HashFunction*>* sequenceOfHashFunctions, int hashFunctionIndex);
        ~CuckooListInput();
        void insert(uint32_t value);
        int search(uint32_t value);
        void clean();
		void reinitHashFunction();
        HashTableListInput* clone();
    private:
        void rehash();
        bool rehashInsert(uint32_t value);

        uint32_t* table;
        int size;
        HashFunction * h1;
        HashFunction * h2;
		int hashFunctionIndex;

        uint32_t empty;
		std::vector<HashFunction*>* sequenceOfHashFunctions;
};