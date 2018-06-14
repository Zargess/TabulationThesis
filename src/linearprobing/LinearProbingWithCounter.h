#include "../hashtable/HashTableCounter.h"
#include "../hashfunction/HashFunction.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"

#pragma once
class LinearProbingWithCounter : public HashTableCounter {
    public:
        LinearProbingWithCounter(int tableSize, std::string hash_name, RandomNumberGenerator* rnd, uint32_t notInData, int outputBits);
        LinearProbingWithCounter(uint32_t* table, int tableSize, std::string hash_name, RandomNumberGenerator* rnd, HashFunction* hashFunction, uint32_t notInData, int outputBits, int probeCount, int searchProbeCount, std::vector<HashFunction*>* sequenceOfHashFunctions);
        ~LinearProbingWithCounter();
        void clean();
        void cleanHashFunctions();
        std::vector<int>* getCounters();
        void resetCounters();
        void insertWithCounter(uint32_t value);
        int searchWithCounter(uint32_t value);
		std::vector<HashFunction*>* getHashFunctions();
		void reinitHashFunction();
        HashTableCounter* clone();
    private:
		HashFunction* initHash(std::string hash);

        uint32_t* table;
        int size;
        HashFunction* hashFunction;
        int probeCount;
        int searchProbeCount;
		int outputBits;
        uint32_t empty;
		std::string hash_name;
		std::vector<HashFunction*>* sequenceOfHashFunctions;
		RandomNumberGenerator* rnd;
};