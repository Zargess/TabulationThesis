#include "Experiment.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"
#include "../datagenerator/DataGenerator.h"
#include "../hashfunction/Tabulation.h"
#include "../hashtable/HashTable.h"
#include "../linearprobing/LinearProbing.h"

#pragma once
class DSWorstCaseExperiment : public Experiment {
	public:
		DSWorstCaseExperiment();
		DSWorstCaseExperiment(int n, RandomNumberGenerator* rnd, DataGenerator* gen, std::string filename, std::string ds, int loadFactorPercentage, int numDS);
		~DSWorstCaseExperiment();
        void setup();
        void run();
        void store();
		void clean();
	private:
		void makeDS();
		std::vector<HashTable*>* createCopies(HashTable* table);
		uint64_t timeWorstCase(uint32_t* data, HashTable* table);
		void insertAllElements(uint32_t* data, HashTable* table);
		int size;
		int numDS;
		std::string dsString;
		RandomNumberGenerator* rnd;
		DataGenerator* gen;

        HashTable * simple;
        HashTable * mixed;
        HashTable* multShift;
        HashTable* polyHash2;
        HashTable* polyHash3;
		
		uint64_t simpleWorstCase;
        uint64_t mixedWorstCase;
        uint64_t msWorstCase;
        uint64_t p2WorstCase;
        uint64_t p3WorstCase;
};