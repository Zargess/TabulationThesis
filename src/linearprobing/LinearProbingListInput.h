#include "../hashtable/HashTableListInput.h"
#include "../hashfunction/HashFunction.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"

#pragma once
class LinearProbingListInput : public HashTableListInput {
    public:
        LinearProbingListInput(int tableSize, uint32_t notInData, std::vector<HashFunction*>* sequenceOfHashFunctions);
        LinearProbingListInput(uint32_t* table, int tableSize, HashFunction* hashFunction, uint32_t notInData, std::vector<HashFunction*>* sequenceOfHashFunctions, int hashFunctionIndex);
        ~LinearProbingListInput();
        void insert(uint32_t value);
        int search(uint32_t value);
        void clean();
		void reinitHashFunction();
        HashTableListInput* clone();
    private:
        uint32_t* table;
        int size;
        HashFunction* hashFunction;
        uint32_t empty;
		int hashFunctionIndex;
		std::vector<HashFunction*>* sequenceOfHashFunctions;
};