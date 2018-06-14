#include "DSTimingExperiment.h"
#include "../randomnumbergenerator/CRandNumberGenerator.h"
#include "../datagenerator/SyntheticDataGenerator.h"
#include "../hashfunction/Tabulation.h"
#include "../linearprobing/LinearProbing.h"
#include "../linearprobing/LinearProbingWithCounter.h"
#include "../linearprobing/LinearProbingListInput.h"
#include "../cuckoo/Cuckoo.h"
#include "../cuckoo/CuckooWithCounter.h"
#include "../cuckoo/CuckooListInput.h"
#include "../doublehashing/DoubleHashing.h"
#include "../doublehashing/DoubleHashingWithCounter.h"
#include "../doublehashing/DoubleHashingListInput.h"
#include "../hashtable/HashTable.h"
#include <ctime>
#include <chrono>
#include <fstream>
#include <string>
#include <iostream>
#include <cmath>
#include <iterator>
#include <algorithm>

DSTimingExperiment::DSTimingExperiment() {
    int n = 1000;
    int numRepeats = 3;
    std::string filename = "DSTimingExperiment";
	RandomNumberGenerator* unirnd = new UniformDistMT();
    RandomNumberGenerator * rnd = new PolyHashRNG(unirnd, 20);
    DataGenerator* gen = new BadSimpleTabulationDataGenerator(1,1,2,2); 
	std::string ds = "cuckoo";
	int loadFactorPercentage = 50;
	int numQueries = 100000;
	DSTimingExperiment(n, numRepeats, rnd, gen, filename, ds, loadFactorPercentage, numQueries);
}

DSTimingExperiment::DSTimingExperiment(int n, int numRepeats, RandomNumberGenerator* rnd, DataGenerator* g, std::string filename, std::string ds, int loadFactorPercentage, int numQueries) {
    this->rnd = rnd;
    this->gen = g;
	this->size = (n / (loadFactorPercentage / 100.0));
	this->dsString = ds;
	this->numQueries = numQueries;
    init(n, numRepeats, filename);
}

DSTimingExperiment::DSTimingExperiment(int n, int size, int numRepeats, RandomNumberGenerator* rnd, DataGenerator* g, std::string filename, std::string ds, int numQueries) {
    this->rnd = rnd;
    this->gen = g;
	this->size = size;
	this->dsString = ds;
	this->numQueries = numQueries;
    init(n, numRepeats, filename);
}

void DSTimingExperiment::clean() {

}

void sortUint64Array(uint64_t* array, int size) {
	std::vector<uint64_t> temp;
	for (int i = 0; i < size; i++) {
		temp.push_back(array[i]);
	}

	std::sort(temp.begin(), temp.end());


	for (int i = 0; i < size; i++) {
		array[i] = temp[i];
	}
}

DSTimingExperiment::~DSTimingExperiment() {
	clean();
}

void DSTimingExperiment::makeDS() {
	if(dsString == "cuckoo") {
	    simple = new Cuckoo(size, "simple", "simple", rnd, 32, gen->getNumberNotInData());
    	mixed = new Cuckoo(size, "mixed", "mixed", rnd, 32, gen->getNumberNotInData());
    	multShift = new Cuckoo(size, "multshift", "multshift", rnd, 32, gen->getNumberNotInData());
    	polyHash2 = new Cuckoo(size, "poly2", "poly2", rnd, 32, gen->getNumberNotInData());
     	polyHash3 = new Cuckoo(size, "poly3", "poly3", rnd, 32, gen->getNumberNotInData());
	} else if(dsString == "probing") {
		simple = new LinearProbing(size, "simple", rnd, gen->getNumberNotInData(), 32);
		mixed = new LinearProbing(size, "mixed", rnd, gen->getNumberNotInData(), 32);
		multShift = new LinearProbing(size, "multshift", rnd, gen->getNumberNotInData(), 32);
		polyHash2 = new LinearProbing(size, "poly2", rnd, gen->getNumberNotInData(), 32);
		polyHash3 = new LinearProbing(size, "poly3", rnd, gen->getNumberNotInData(), 32);
	} else if (dsString == "double") {
		simple = new DoubleHashing(size, "simple", "simple", rnd, gen->getNumberNotInData(), 32);
		mixed = new DoubleHashing(size, "mixed", "mixed", rnd, gen->getNumberNotInData(), 32);
		multShift = new DoubleHashing(size, "multshift", "multshift", rnd, gen->getNumberNotInData(), 32);
		polyHash2 = new DoubleHashing(size, "poly2", "poly2", rnd, gen->getNumberNotInData(), 32);
		polyHash3 = new DoubleHashing(size, "poly3", "poly3", rnd, gen->getNumberNotInData(), 32);
	}
}

void DSTimingExperiment::makeCounterDS() {
	if(dsString == "cuckoo") {
	    simpleCounter = new CuckooWithCounter(size, "simple", "simple", rnd, 32, gen->getNumberNotInData());
    	mixedCounter = new CuckooWithCounter(size, "mixed", "mixed", rnd, 32, gen->getNumberNotInData());
    	multShiftCounter = new CuckooWithCounter(size, "multshift", "multshift", rnd, 32, gen->getNumberNotInData());
    	polyHash2Counter = new CuckooWithCounter(size, "poly2", "poly2", rnd, 32, gen->getNumberNotInData());
     	polyHash3Counter = new CuckooWithCounter(size, "poly3", "poly3", rnd, 32, gen->getNumberNotInData());
	} else if(dsString == "probing") {
		simpleCounter = new LinearProbingWithCounter(size, "simple", rnd, gen->getNumberNotInData(), 32);
		mixedCounter = new LinearProbingWithCounter(size, "mixed", rnd, gen->getNumberNotInData(), 32);
		multShiftCounter = new LinearProbingWithCounter(size, "multshift", rnd, gen->getNumberNotInData(), 32);
		polyHash2Counter = new LinearProbingWithCounter(size, "poly2", rnd, gen->getNumberNotInData(), 32);
		polyHash3Counter = new LinearProbingWithCounter(size, "poly3", rnd, gen->getNumberNotInData(), 32);
	} else if (dsString == "double") {
		
		simpleCounter = new DoubleHashingWithCounter(size, "simple", "simple", rnd, gen->getNumberNotInData(), 32);
		mixedCounter = new DoubleHashingWithCounter(size, "mixed", "mixed", rnd, gen->getNumberNotInData(), 32);
		multShiftCounter = new DoubleHashingWithCounter(size, "multshift", "multshift", rnd, gen->getNumberNotInData(), 32);
		polyHash2Counter = new DoubleHashingWithCounter(size, "poly2", "poly2", rnd, gen->getNumberNotInData(), 32);
		polyHash3Counter = new DoubleHashingWithCounter(size, "poly3", "poly3", rnd, gen->getNumberNotInData(), 32);
	}
}

void DSTimingExperiment::makeListInputDS(std::vector<HashFunction*>* simple, std::vector<HashFunction*>* mixed, std::vector<HashFunction*>* ms, std::vector<HashFunction*>* p2, std::vector<HashFunction*>* p3) {
	if(dsString == "cuckoo") {
	    simpleListInput = new CuckooListInput(size, gen->getNumberNotInData(), simple);
    	mixedListInput = new CuckooListInput(size, gen->getNumberNotInData(), mixed);
    	multShiftListInput = new CuckooListInput(size, gen->getNumberNotInData(), ms);
    	polyHash2ListInput = new CuckooListInput(size, gen->getNumberNotInData(), p2);
     	polyHash3ListInput = new CuckooListInput(size, gen->getNumberNotInData(), p3);
	} else if(dsString == "probing") {
		simpleListInput = new LinearProbingListInput(size, gen->getNumberNotInData(), simple);
		mixedListInput = new LinearProbingListInput(size, gen->getNumberNotInData(), mixed);
		multShiftListInput = new LinearProbingListInput(size, gen->getNumberNotInData(), ms);
		polyHash2ListInput = new LinearProbingListInput(size, gen->getNumberNotInData(), p2);
		polyHash3ListInput = new LinearProbingListInput(size, gen->getNumberNotInData(), p3);
	}  else if(dsString == "double") {
		simpleListInput = new DoubleHashingListInput(size, gen->getNumberNotInData(), simple);
		mixedListInput = new DoubleHashingListInput(size, gen->getNumberNotInData(), mixed);
		multShiftListInput = new DoubleHashingListInput(size, gen->getNumberNotInData(), ms);
		polyHash2ListInput = new DoubleHashingListInput(size, gen->getNumberNotInData(), p2);
		polyHash3ListInput = new DoubleHashingListInput(size, gen->getNumberNotInData(), p3);
	}
}

void DSTimingExperiment::setup() {
	simpleInsertTimes = new uint64_t[numRepeats];
    mixedInsertTimes = new uint64_t[numRepeats];
    msInsertTimes = new uint64_t[numRepeats];
    p2InsertTimes = new uint64_t[numRepeats];
    p3InsertTimes = new uint64_t[numRepeats];

	simpleQueryTimes = new uint64_t[numRepeats];
    mixedQueryTimes = new uint64_t[numRepeats];
    msQueryTimes = new uint64_t[numRepeats];
    p2QueryTimes = new uint64_t[numRepeats];
    p3QueryTimes = new uint64_t[numRepeats];

	simpleProbeCounts = new uint64_t[numRepeats];
    mixedProbeCounts = new uint64_t[numRepeats];
    msProbeCounts = new uint64_t[numRepeats];
    p2ProbeCounts = new uint64_t[numRepeats];
    p3ProbeCounts = new uint64_t[numRepeats];

	simpleSearchProbeCounts = new uint64_t[numRepeats];
    mixedSearchProbeCounts = new uint64_t[numRepeats];
    msSearchProbeCounts = new uint64_t[numRepeats];
    p2SearchProbeCounts = new uint64_t[numRepeats];
    p3SearchProbeCounts = new uint64_t[numRepeats];

}

uint64_t DSTimingExperiment::timeHashTableInsert(uint32_t* data, HashTableListInput* table) {
	uint64_t result = 0;
	auto begin = std::chrono::high_resolution_clock::now();        
			
	for(int i = 0; i < n; i++) {
		table->insert(data[i]);
	} 
	auto endTime = std::chrono::high_resolution_clock::now(); 
	auto elapsed_millisecs = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - begin).count();
	result += elapsed_millisecs;
	return result;
}

uint64_t DSTimingExperiment::insertWithCounter(uint32_t* data, HashTableCounter* table) {   
	for(int i = 0; i < n; i++) {       
		table->insertWithCounter(data[i]);
	}
	return (*table->getCounters())[0];
}

uint64_t DSTimingExperiment::searchWithCounter(std::vector<uint32_t>* queryData, HashTableCounter* table) {   
	for(int i = 0; i < numQueries; i++) {       
		table->searchWithCounter((*queryData)[i]);
	}
	return (*table->getCounters())[1];
}

void DSTimingExperiment::insertElements(uint32_t* data, HashTable* table) {
	for(int i = 0; i < n; i++) {       
		table->insert(data[i]);
	}
}

uint64_t DSTimingExperiment::timeHashTableQuery(std::vector<uint32_t>* queryData, HashTableListInput* table) {
	uint64_t result = 0;
	auto begin = std::chrono::high_resolution_clock::now();        
	for(int i = 0; i < numQueries; i++) {       
		table->search((*queryData)[i]);
	} 
	auto endTime = std::chrono::high_resolution_clock::now(); 
	auto elapsed_millisecs = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - begin).count();
	result += elapsed_millisecs;
	return result;
}

std::vector<uint32_t>* DSTimingExperiment::generateQueryData(uint32_t* data) {
	std::vector<uint32_t>* result = new std::vector<uint32_t>();
	for(int i = 0; i < numQueries; i++) {
		int index = rnd->getNextNumber() % n;
		result->push_back(data[index]);
	}
	return result;
}


void DSTimingExperiment::run() {
   
	for(int i = 0; i < numRepeats; i++) {

		makeCounterDS();
		if(i % 100 == 0) {
			std::cout << "Iteration: " << i+1 << "/" << numRepeats  << std::endl;
		}
		
        uint32_t* data = gen->createDataSet(n);
		

		simpleProbeCounts[i] = insertWithCounter(data, simpleCounter);
		mixedProbeCounts[i] = insertWithCounter(data, mixedCounter);
		msProbeCounts[i] = insertWithCounter(data, multShiftCounter);
		p2ProbeCounts[i] = insertWithCounter(data, polyHash2Counter);
		p3ProbeCounts[i] = insertWithCounter(data, polyHash3Counter);

		std::vector<HashFunction*>* simpleHashFunctions = simpleCounter->getHashFunctions();
		std::vector<HashFunction*>* mixedHashFunctions = mixedCounter->getHashFunctions();
		std::vector<HashFunction*>* multShiftHashFunctions = multShiftCounter->getHashFunctions();
		std::vector<HashFunction*>* polyHash2Functions = polyHash2Counter->getHashFunctions();
		std::vector<HashFunction*>* polyHash3Functions = polyHash3Counter->getHashFunctions();

		std::vector<uint32_t>* queryData = generateQueryData(data);

		simpleSearchProbeCounts[i] = searchWithCounter(queryData, simpleCounter);
		mixedSearchProbeCounts[i] = searchWithCounter(queryData, mixedCounter);
		msSearchProbeCounts[i] = searchWithCounter(queryData, multShiftCounter);
		p2SearchProbeCounts[i] = searchWithCounter(queryData, polyHash2Counter);
		p3SearchProbeCounts[i] = searchWithCounter(queryData, polyHash3Counter);

		delete simpleCounter;
		delete mixedCounter;
		delete multShiftCounter;
		delete polyHash2Counter;
		delete polyHash3Counter; 

		makeListInputDS(simpleHashFunctions, mixedHashFunctions, multShiftHashFunctions, polyHash2Functions, polyHash3Functions);
		
		simpleInsertTimes[i] = timeHashTableInsert(data, simpleListInput);
		mixedInsertTimes[i] = timeHashTableInsert(data, mixedListInput);
		msInsertTimes[i] = timeHashTableInsert(data, multShiftListInput);
		p2InsertTimes[i] = timeHashTableInsert(data, polyHash2ListInput);
		p3InsertTimes[i] = timeHashTableInsert(data, polyHash3ListInput);

		simpleQueryTimes[i] =  timeHashTableQuery(queryData, simpleListInput);
		mixedQueryTimes[i] =  timeHashTableQuery(queryData, mixedListInput);
		msQueryTimes[i] =  timeHashTableQuery(queryData, multShiftListInput);
		p2QueryTimes[i] =  timeHashTableQuery(queryData, polyHash2ListInput);
		p3QueryTimes[i] =  timeHashTableQuery(queryData, polyHash3ListInput);

		delete simpleListInput;
		delete mixedListInput;
		delete multShiftListInput;
		delete polyHash2ListInput;
		delete polyHash3ListInput; 

		delete[] data;
		delete queryData;
	}
	sortUint64Array(simpleInsertTimes, numRepeats);
	sortUint64Array(mixedInsertTimes, numRepeats);
	sortUint64Array(msInsertTimes, numRepeats);
	sortUint64Array(p2InsertTimes, numRepeats);	
	sortUint64Array(p3InsertTimes, numRepeats);

	sortUint64Array(simpleQueryTimes, numRepeats);
	sortUint64Array(mixedQueryTimes, numRepeats);
	sortUint64Array(msQueryTimes, numRepeats);
	sortUint64Array(p2QueryTimes, numRepeats);	
	sortUint64Array(p3QueryTimes, numRepeats);

	sortUint64Array(simpleProbeCounts, numRepeats);
	sortUint64Array(mixedProbeCounts, numRepeats);
	sortUint64Array(msProbeCounts, numRepeats);
	sortUint64Array(p2ProbeCounts, numRepeats);	
	sortUint64Array(p3ProbeCounts, numRepeats);

	sortUint64Array(simpleSearchProbeCounts, numRepeats);
	sortUint64Array(mixedSearchProbeCounts, numRepeats);
	sortUint64Array(msSearchProbeCounts, numRepeats);
	sortUint64Array(p2SearchProbeCounts, numRepeats);	
	sortUint64Array(p3SearchProbeCounts, numRepeats);
	
    
}



void DSTimingExperiment::store() {
	std::ofstream myfile;
    std::string path = "data/DS/insert/" + filename + ".txt";
    myfile.open(path);
    
	myfile << "Insert \n";
	myfile << "--- \n" << "Simple Tab \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile << i << "," << simpleInsertTimes[i]  / 1000000.0 << "\n";
	}
	myfile << "--- \n" << "Mixed Tab \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile << i << "," << mixedInsertTimes[i]  / 1000000.0 << "\n";
	}
	myfile << "--- \n" << "MultShift \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile << i << "," << msInsertTimes[i]  / 1000000.0 << "\n";
	}
	myfile << "--- \n" << "PolyHash2 \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile << i << "," << p2InsertTimes[i]  / 1000000.0 << "\n";
	}
	myfile << "--- \n" << "PolyHash3 \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile << i << "," << p3InsertTimes[i]  / 1000000.0 << "\n";
	}

	myfile.close();

	std::ofstream myfile2;
    path = "data/DS/query/" + filename + ".txt";
    myfile2.open(path);
    
	myfile2 << "Query \n";
	myfile2 << "--- \n" << "Simple Tab \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile2 << i << "," << simpleQueryTimes[i]  / 1000000.0 << "\n";
	}
	myfile2 << "--- \n" << "Mixed Tab \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile2 << i << "," << mixedQueryTimes[i]  / 1000000.0 << "\n";
	}
	myfile2 << "--- \n" << "MultShift \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile2 << i << "," << msQueryTimes[i]  / 1000000.0 << "\n";
	}
	myfile2 << "--- \n" << "PolyHash2 \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile2 << i << "," << p2QueryTimes[i]  / 1000000.0 << "\n";
	}
	myfile2 << "--- \n" << "PolyHash3 \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile2 << i << "," << p3QueryTimes[i]  / 1000000.0 << "\n";
	}

	myfile2.close();


	std::ofstream myfile3;
    path = "data/DS/probe/" + filename + ".txt";
    myfile3.open(path);

	myfile3 << "Probe \n";
	myfile3 << "--- \n" << "Simple Tab \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile3 << i << "," << simpleProbeCounts[i]  << "\n";
	}
	myfile3 << "--- \n" << "Mixed Tab \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile3 << i << "," << mixedProbeCounts[i]  << "\n";
	}
	myfile3 << "--- \n" << "MultShift \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile3 << i << "," << msProbeCounts[i]  << "\n";
	}
	myfile3 << "--- \n" << "PolyHash2 \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile3 << i << "," << p2ProbeCounts[i]  << "\n";
	}
	myfile3 << "--- \n" << "PolyHash3 \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile3 << i << "," << p3ProbeCounts[i]  << "\n";
	}
	
	myfile3.close();


	std::ofstream myfile4;
    path = "data/DS/searchprobe/" + filename + ".txt";
    myfile4.open(path);

	myfile4 << "SearchProbe \n";
	myfile4 << "--- \n" << "Simple Tab \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile4 << i << "," << simpleSearchProbeCounts[i]  << "\n";
	}
	myfile4 << "--- \n" << "Mixed Tab \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile4 << i << "," << mixedSearchProbeCounts[i]  << "\n";
	}
	myfile4 << "--- \n" << "MultShift \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile4 << i << "," << msSearchProbeCounts[i]  << "\n";
	}
	myfile4 << "--- \n" << "PolyHash2 \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile4 << i << "," << p2SearchProbeCounts[i]  << "\n";
	}
	myfile4 << "--- \n" << "PolyHash3 \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile4 << i << "," << p3SearchProbeCounts[i]  << "\n";
	}
	
	myfile4.close();
}