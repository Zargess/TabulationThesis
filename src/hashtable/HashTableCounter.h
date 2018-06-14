#include <cstdint>
#include <vector>
#include "../hashfunction/HashFunction.h"
#pragma once
class HashTableCounter {
    public:
        virtual void clean() = 0;
        virtual std::vector<int>* getCounters() = 0;
        virtual void resetCounters() = 0;
        virtual void insertWithCounter(uint32_t value) = 0;
        virtual int searchWithCounter(uint32_t value) = 0;
		virtual std::vector<HashFunction*>* getHashFunctions() = 0;
        virtual HashTableCounter* clone() = 0;
        virtual void reinitHashFunction() = 0;
        virtual ~HashTableCounter();
};