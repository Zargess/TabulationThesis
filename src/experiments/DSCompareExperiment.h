#include "Experiment.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"
#include "../datagenerator/DataGenerator.h"
#include "../hashfunction/Tabulation.h"
#include "../hashtable/HashTable.h"
#include "../linearprobing/LinearProbing.h"

#pragma once
class DSCompareExperiment : public Experiment {
    public:
        DSCompareExperiment();
        DSCompareExperiment(int n, int numRepeats, RandomNumberGenerator* rnd, DataGenerator* g, std::string filename, std::string hashName, int loadFactorPercentage, std::vector<double>* querySizes);
        ~DSCompareExperiment();
        void setup();
        void run();
        void store();
        void clean();
    private:
        void makeDS();
       
        uint64_t timeHashTableInsert(uint32_t* data, HashTable* table);
        void insertElements(uint32_t* data, HashTable* table);

        uint64_t timeHashTableQuery(std::vector<uint32_t>* data, HashTable* table);
        std::vector<uint32_t>* generateQueryData(uint32_t* data);
        int size;
        int numQueries;
        std::string hashName;
        RandomNumberGenerator * rnd;
        DataGenerator * gen;
        HashTable* cuckoo;
        HashTable* linearProbing;

        uint64_t* cuckooInsertTimes;
        uint64_t* lpInsertTimes;

        uint64_t* cuckooQueryTimes;
        uint64_t* lpQueryTimes;

		std::vector<double>* querySizes;
        
};