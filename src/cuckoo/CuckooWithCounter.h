#include "../hashtable/HashTableCounter.h"
#include "../hashfunction/HashFunction.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"
#include <string>

#pragma once
class CuckooWithCounter : public HashTableCounter {
    public:
        CuckooWithCounter(int tableSize, std::string h1, std::string h2, RandomNumberGenerator* rnd, int outputBits, uint32_t notInData);
        CuckooWithCounter(uint32_t* table, int tableSize, std::string h1, std::string h2, RandomNumberGenerator* rnd, int outputBits, uint32_t notInData, HashFunction* h1_func, HashFunction* h2_func, int probeCount, int searchProbeCount, std::vector<HashFunction*>* sequenceOfHashFunctions);
        ~CuckooWithCounter();
        void insertWithCounter(uint32_t value);
        int searchWithCounter(uint32_t value);
        void clean();
        void cleanHashFunctions();
        std::vector<int>* getCounters();
		std::vector<HashFunction*>* getHashFunctions();
        void resetCounters();
		void reinitHashFunction();
        HashTableCounter* clone();
    private:
        HashFunction * initHash(std::string hash);
        void rehashWithCounter();
        bool rehashInsertWithCounter(uint32_t value);

        uint32_t* table;
        int size;
        HashFunction * h1;
        HashFunction * h2;
        RandomNumberGenerator * rnd;

        int outputBits;
        std::string h1_name;
        std::string h2_name;

        int probeCount;
        int searchProbeCount;

        uint32_t empty;

		std::vector<HashFunction*>* sequenceOfHashFunctions;
};

