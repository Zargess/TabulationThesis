#include "SimpleMixedMaxBucketSizeExperiment.h"
#include "../randomnumbergenerator/CRandNumberGenerator.h"
#include "../datagenerator/SyntheticDataGenerator.h"
#include "../datagenerator/RealDataGenerator.h"
#include "../hashfunction/Tabulation.h"
#include <ctime>
#include <chrono>
#include <fstream>
#include <string>
#include <iostream>
#include <cmath>
#include <math.h>
#include <bitset>
#include <algorithm>
#include <iterator>

void sortDoubleArray(double* array, int size) {
	std::vector<double> temp;
	for (int i = 0; i < size; i++) {
		temp.push_back(array[i]);
	}

	std::sort(temp.begin(), temp.end());

	for (int i = 0; i < size; i++) {
		array[i] = temp[i];
	}
}

SimpleMixedMaxBucketSizeExperiment::SimpleMixedMaxBucketSizeExperiment(int n, int numBuckets, int numRepeats, RandomNumberGenerator* rnd, DataGenerator* g, std::string filename) {
	this->numBuckets = numBuckets;
	this->rnd = rnd;
	this->gen = g;
	init(n, numRepeats, filename);
}

SimpleMixedMaxBucketSizeExperiment::SimpleMixedMaxBucketSizeExperiment() {
	RandomNumberGenerator* unirnd = new UniformDistMT();
    RandomNumberGenerator* rnd = new PolyHashRNG(unirnd, 32);
	DataGenerator* gen = new BagOfWordsDataGenerator(rnd); 
    int n = 104976;   
    int numRepeats = 1000;
    int numBuckets = 200000;
	std::string filename = "MaxBucketDefault";
	SimpleMixedMaxBucketSizeExperiment(n, numBuckets, numRepeats, rnd, gen, filename); 
}

void SimpleMixedMaxBucketSizeExperiment::clean() {
	delete[] simpleMaxBucketSizes;
    delete[] mixedMaxBucketSizes;
	delete[] multiplyShiftMaxBucketSizes;
    delete[] polyHash3MaxBucketSizes;
    delete[] polyHash2MaxBucketSizes;
}


SimpleMixedMaxBucketSizeExperiment::~SimpleMixedMaxBucketSizeExperiment() {
	clean(); 
}

void SimpleMixedMaxBucketSizeExperiment::setup() { 
	simpleMaxBucketSizes = new double[numRepeats]{0};
    mixedMaxBucketSizes = new double[numRepeats]{0};
    multiplyShiftMaxBucketSizes = new double[numRepeats]{0};
    polyHash3MaxBucketSizes = new double[numRepeats]{0};
    polyHash2MaxBucketSizes = new double[numRepeats]{0};
}

void SimpleMixedMaxBucketSizeExperiment::run() {
	double* simplebucket = new double[numBuckets]{0};
	double* mixedbucket = new double[numBuckets]{0};
	double* multiplyShiftbucket = new double[numBuckets]{0};
	double* polyHash3bucket = new double[numBuckets]{0};
	double* polyHash2bucket = new double[numBuckets]{0};
	
	uint32_t res;
	uint32_t * data; 

	for (int i = 0; i < numRepeats; i++) {
		for (int j = 0; j < numBuckets; j++) {
			simplebucket[j] = 0;
			mixedbucket[j] = 0;
			multiplyShiftbucket[j] = 0;
			polyHash3bucket[j] = 0;
			polyHash2bucket[j] = 0;
		}

		data = gen->createDataSet(n);		
		SimpleTabulation simple(rnd);
        MixedTabulation mixed(rnd);
        MultiplyShift multshift(rnd, ceil(log2(numBuckets)));
        PolyHash2_Mersenne poly2hash(rnd);
        PolyHash3_Mersenne poly3hash(rnd);
		
		for (int k = 0; k < n; k++) {
			res = simple.hash(data[k]) % numBuckets;
            simplebucket[res] += 1;
            
            res = mixed.hash(data[k]) % numBuckets;
            mixedbucket[res] += 1;

            res = multshift.hash(data[k]) % numBuckets;
            multiplyShiftbucket[res] += 1;

            res = poly2hash.hash(data[k]) % numBuckets;
            polyHash2bucket[res] += 1;

            res = poly3hash.hash(data[k]) % numBuckets;
            polyHash3bucket[res] += 1;
		}

		for (int k = 0; k < numBuckets; k++) {
			if (simplebucket[k] > simpleMaxBucketSizes[i]) {
				simpleMaxBucketSizes[i] = simplebucket[k];
			}

			if (mixedbucket[k] > mixedMaxBucketSizes[i]) {
				mixedMaxBucketSizes[i] = mixedbucket[k];
			}

			if (multiplyShiftbucket[k] > multiplyShiftMaxBucketSizes[i]) {
				multiplyShiftMaxBucketSizes[i] = multiplyShiftbucket[k];
			}

			if (polyHash2bucket[k] > polyHash2MaxBucketSizes[i]) {
				polyHash2MaxBucketSizes[i] = polyHash2bucket[k];
			}

			if (polyHash3bucket[k] > polyHash3MaxBucketSizes[i]) {
				polyHash3MaxBucketSizes[i] = polyHash3bucket[k];
			}
		}

		delete[] data;
	}
	sortDoubleArray(simpleMaxBucketSizes, numRepeats);
	sortDoubleArray(mixedMaxBucketSizes, numRepeats);
	sortDoubleArray(multiplyShiftMaxBucketSizes, numRepeats);
	sortDoubleArray(polyHash2MaxBucketSizes, numRepeats);	
	sortDoubleArray(polyHash3MaxBucketSizes, numRepeats);
}

void SimpleMixedMaxBucketSizeExperiment::store() {
	std::ofstream myfile;
    std::string filename = this->filename;
    std::string path = "data/MaxBucket/" + filename + ".txt";
    myfile.open(path);
    
	myfile << "Max Bucket \n";
	myfile << "--- \n" << "Simple Tab \n";
	
	for (int i = 0; i < numRepeats; i++) {
		myfile << i << "," << simpleMaxBucketSizes[i] << "\n";
	}

	myfile << "--- \n" << "Mixed Tab \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile << i << "," << mixedMaxBucketSizes[i] << "\n";
	}

	myfile << "--- \n" << "Multiply Shift \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile << i << "," << multiplyShiftMaxBucketSizes[i] << "\n";
	}
	
	myfile << "--- \n" << "PolyHash 2 \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile << i << "," << polyHash2MaxBucketSizes[i] << "\n";
	}
	
	myfile << "--- \n" << "PolyHash 3 \n";
	for (int i = 0; i < numRepeats; i++) {
		myfile << i << "," << polyHash3MaxBucketSizes[i] << "\n";
	}
	
	myfile.close();
}