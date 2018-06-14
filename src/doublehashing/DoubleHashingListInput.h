#include "../hashtable/HashTableListInput.h"
#include "../hashfunction/HashFunction.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"

#pragma once
class DoubleHashingListInput : public HashTableListInput {
    public:
        DoubleHashingListInput(int tableSize, uint32_t notInData, std::vector<HashFunction*>* sequenceOfHashFunctions);
        DoubleHashingListInput(uint32_t* table, int tableSize, HashFunction* hashFunction1, HashFunction* hashFunction2, uint32_t notInData, std::vector<HashFunction*>* sequenceOfHashFunctions, int hashFunctionIndex);
        ~DoubleHashingListInput();
        void insert(uint32_t value);
        int search(uint32_t value);
        void clean();
		void reinitHashFunction();
        HashTableListInput* clone();
    private:
        uint32_t* table;
        int size;
        HashFunction* hashFunction1;
        HashFunction* hashFunction2;
        uint32_t empty;
		int hashFunctionIndex;
		std::vector<HashFunction*>* sequenceOfHashFunctions;
};