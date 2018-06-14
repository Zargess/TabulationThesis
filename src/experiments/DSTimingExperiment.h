#include "Experiment.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"
#include "../datagenerator/DataGenerator.h"
#include "../hashfunction/Tabulation.h"
#include "../hashtable/HashTable.h"
#include "../hashtable/HashTableCounter.h"
#include "../hashtable/HashTableListInput.h"
#include "../linearprobing/LinearProbing.h"

#pragma once
class DSTimingExperiment : public Experiment {
    public:
        DSTimingExperiment();
        DSTimingExperiment(int n, int numRepeats, RandomNumberGenerator* rnd, DataGenerator* g, std::string filename, std::string ds, int loadFactorPercentage, int numQueries);
        DSTimingExperiment(int n, int size, int numRepeats, RandomNumberGenerator* rnd, DataGenerator* g, std::string filename, std::string ds, int numQueries);
        ~DSTimingExperiment();
        void setup();
        void run();
        void store();
        void clean();
    private:
        void makeDS();
        void makeListInputDS(std::vector<HashFunction*>* simple, std::vector<HashFunction*>* mixed, std::vector<HashFunction*>* ms, std::vector<HashFunction*>* p2, std::vector<HashFunction*>* p3);
        void makeCounterDS();
        uint64_t timeHashTableInsert(uint32_t* data, HashTableListInput* table);
        uint64_t insertWithCounter(uint32_t* data, HashTableCounter* table);
        void insertElements(uint32_t* data, HashTable* table);

        uint64_t timeHashTableQuery(std::vector<uint32_t>* data, HashTableListInput* table);
        uint64_t searchWithCounter(std::vector<uint32_t>* data, HashTableCounter* table);
        std::vector<uint32_t>* generateQueryData(uint32_t* data);
        int size;
        int numQueries;
        std::string dsString;
        RandomNumberGenerator * rnd;
        DataGenerator * gen;
        HashTable* simple;
        HashTable* mixed;
        HashTable* multShift;
        HashTable* polyHash2;
        HashTable* polyHash3;

        HashTableCounter* simpleCounter;
        HashTableCounter* mixedCounter;
        HashTableCounter* multShiftCounter;
        HashTableCounter* polyHash2Counter;
        HashTableCounter* polyHash3Counter;

        HashTableListInput* simpleListInput;
        HashTableListInput* mixedListInput;
        HashTableListInput* multShiftListInput;
        HashTableListInput* polyHash2ListInput;
        HashTableListInput* polyHash3ListInput;

        uint64_t* simpleInsertTimes;
        uint64_t* mixedInsertTimes;
        uint64_t* msInsertTimes;
        uint64_t* p2InsertTimes;
        uint64_t* p3InsertTimes;

        uint64_t* simpleQueryTimes;
        uint64_t* mixedQueryTimes;
        uint64_t* msQueryTimes;
        uint64_t* p2QueryTimes;
        uint64_t* p3QueryTimes;

        uint64_t* simpleProbeCounts;
        uint64_t* mixedProbeCounts;
        uint64_t* msProbeCounts;
        uint64_t* p2ProbeCounts;
        uint64_t* p3ProbeCounts;

        uint64_t* simpleSearchProbeCounts;
        uint64_t* mixedSearchProbeCounts;
        uint64_t* msSearchProbeCounts;
        uint64_t* p2SearchProbeCounts;
        uint64_t* p3SearchProbeCounts;
        
        
};