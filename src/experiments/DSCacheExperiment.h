#include "Experiment.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"
#include "../datagenerator/DataGenerator.h"
#include "../hashfunction/Tabulation.h"
#include "../hashtable/HashTable.h"

#pragma once
class DSCacheExperiment : public Experiment {
	public:
		DSCacheExperiment(int n, int size, int numRepeats, RandomNumberGenerator* rnd, DataGenerator* g, std::string filename, std::string hashName);
		~DSCacheExperiment();
		void setup();
		void run();
		void store();
		void clean();
	private:
		int size;
		std::string hashName;
        RandomNumberGenerator * rnd;
        DataGenerator * gen;
		HashTable* table;
};