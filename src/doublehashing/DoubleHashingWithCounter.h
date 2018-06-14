#include "../hashtable/HashTableCounter.h"
#include "../hashfunction/HashFunction.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"

#pragma once
class DoubleHashingWithCounter : public HashTableCounter {
    public:
        DoubleHashingWithCounter(int tableSize, std::string hash_name1, std::string hash_name2, RandomNumberGenerator* rnd, uint32_t notInData, int outputBits);
        DoubleHashingWithCounter(uint32_t* table, int tableSize, std::string hash_name1, std::string hash_name2, RandomNumberGenerator* rnd, HashFunction* hashFunction1, HashFunction* hashFunction2, uint32_t notInData, int outputBits, int probeCount, int searchProbeCount, std::vector<HashFunction*>* sequenceOfHashFunctions);
        ~DoubleHashingWithCounter();
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
        HashFunction* hashFunction1;
        HashFunction* hashFunction2;
        int probeCount;
        int searchProbeCount;
		int outputBits;
        uint32_t empty;
		std::string hash_name1;
		std::string hash_name2;
		std::vector<HashFunction*>* sequenceOfHashFunctions;
		RandomNumberGenerator* rnd;
};