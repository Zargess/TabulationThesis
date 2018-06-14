#include "Experiment.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"
#include "../datagenerator/DataGenerator.h"
#include "../hashfunction/Tabulation.h"
#include "../hashtable/HashTable.h"

#pragma once
class SingleDSCacheExperiment : public Experiment {
	public:
		SingleDSCacheExperiment(int n, int size, int numRepeats, RandomNumberGenerator* rnd, DataGenerator* g, std::string filename, std::string dsName);
		~SingleDSCacheExperiment();
		void setup();
		void run();
		void store();
		void clean();
	private:
		HashTable* getNextDS(int counter);
		int size;
		std::string dsName;
        RandomNumberGenerator * rnd;
        DataGenerator * gen;
		HashTable* table;
};