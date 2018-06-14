#include "SimpleMixedMomentExperiment.h"
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

SimpleMixedMomentExperiment::SimpleMixedMomentExperiment() {
    RandomNumberGenerator* unirnd = new UniformDistMT();
    RandomNumberGenerator* rnd = new PolyHashRNG(unirnd, 32);
    DataGenerator* gen = new BadSimpleTabulationDataGenerator(1,1,2,2);
    int numBuckets = 2;
    int n = 104976;
    int numRepeats = 1000;
    int momentDegree = 32;
    std::string filename = "MomentDefault";
    SimpleMixedMomentExperiment(n, numBuckets, numRepeats, rnd, gen, filename, momentDegree);
}

SimpleMixedMomentExperiment::SimpleMixedMomentExperiment(int n, int numBuckets, int numRepeats, RandomNumberGenerator* rnd, DataGenerator* g, std::string filename, int momentDegree) {
    this->numBuckets = numBuckets;
    this->rnd = rnd;
    this->gen = g;
    this->momentDegree = momentDegree;
    init(n, numRepeats, filename);
}

SimpleMixedMomentExperiment::~SimpleMixedMomentExperiment() {
    clean();
}

void SimpleMixedMomentExperiment::clean() {
    delete[] simpleBucketSizes;
    delete[] mixedBucketSizes;
    delete[] multiplyShiftBucketSizes;
    delete[] polyHash3BucketSizes;
    delete[] polyHash2BucketSizes;

    delete[] simpleMomentSizes;
    delete[] mixedMomentSizes;
    delete[] multiplyShiftSizes;
    delete[] polyHash3Sizes;
    delete[] polyHash2Sizes;    
}

void SimpleMixedMomentExperiment::setup() {
    simpleBucketSizes = new double[numBuckets];
    mixedBucketSizes = new double[numBuckets];
    multiplyShiftBucketSizes = new double[numBuckets];
    polyHash3BucketSizes = new double[numBuckets];
    polyHash2BucketSizes = new double[numBuckets];

    simpleMomentSizes = new double[numRepeats]{0};
    mixedMomentSizes = new double[numRepeats]{0};
    multiplyShiftSizes = new double[numRepeats]{0};
    polyHash3Sizes = new double[numRepeats]{0};
    polyHash2Sizes = new double[numRepeats]{0};
}

void SimpleMixedMomentExperiment::emptyBuckets() {
    for (int i = 0; i < numBuckets; i++) {
        simpleBucketSizes[i] = 0;
        mixedBucketSizes[i] = 0;
        multiplyShiftBucketSizes[i] = 0;
        polyHash3BucketSizes[i] = 0;
        polyHash2BucketSizes[i] = 0;
    };
}

void SimpleMixedMomentExperiment::run() {

    uint32_t res;
    int simpleMaxBucket = 0;
    int mixedMaxBucket = 0;
    uint32_t * data;
    for(int j = 0; j < numRepeats; j++) {
        emptyBuckets();
        data = gen->createDataSet(n);
        SimpleTabulation simple(rnd);
        MixedTabulation mixed(rnd);
        MultiplyShift multshift(rnd, ceil(log2(numBuckets)));
        PolyHash2_Mersenne poly2hash(rnd);
        PolyHash3_Mersenne poly3hash(rnd);
        for(int k = 0; k < n; k++) {
            res = simple.hash(data[k]) % numBuckets;
            simpleBucketSizes[res] += 1;
            
            res = mixed.hash(data[k]) % numBuckets;
            mixedBucketSizes[res] += 1;

            res = multshift.hash(data[k]) % numBuckets;
            multiplyShiftBucketSizes[res] += 1;

            res = poly2hash.hash(data[k]) % numBuckets;
            polyHash2BucketSizes[res] += 1;

            res = poly3hash.hash(data[k]) % numBuckets;
            polyHash3BucketSizes[res] += 1;
        }

        for(int k = 0; k < numBuckets; k++) {
            simpleMomentSizes[j] += pow((double) simpleBucketSizes[k], (double) momentDegree);
            mixedMomentSizes[j] += pow((double) mixedBucketSizes[k], (double)  momentDegree);
            multiplyShiftSizes[j] += pow((double) multiplyShiftBucketSizes[k], (double)  momentDegree);
            polyHash2Sizes[j] += pow((double) polyHash2BucketSizes[k], (double)  momentDegree);
            polyHash3Sizes[j] += pow((double) polyHash3BucketSizes[k], (double)  momentDegree);
        }

        delete[] data;
    }

    
}

void SimpleMixedMomentExperiment::store() {

    std::ofstream myfile;
    std::string path = "data/Moment/" + filename + ".txt";
    myfile.open(path);
    myfile << "32nd moment \n" << "--- \n" << "Simple Tab \n";

    double total = 0;
    for (int j = 0; j < numRepeats; j++) {
        total += pow(simpleMomentSizes[j], 1.0/momentDegree);
    }

    myfile << numBuckets << "," << (total / numRepeats) << "\n";

    myfile << "--- \n" << "Mixed Tab \n";

    total = 0;
    for (int j = 0; j < numRepeats; j++) {
        total += pow(mixedMomentSizes[j], 1.0/momentDegree);
    }

    myfile << numBuckets << "," << (total / numRepeats) << "\n";

    myfile << "--- \n" << "Mult Shift \n";

    total = 0;
    for (int j = 0; j < numRepeats; j++) {
        total += pow(multiplyShiftSizes[j], 1.0/momentDegree);
    }

    myfile << numBuckets << "," << (total / numRepeats) << "\n";

    
    myfile << "--- \n" << "PolyHash 2 \n";

    total = 0;
    for (int j = 0; j < numRepeats; j++) {
        total += pow(polyHash2Sizes[j], 1.0/momentDegree);
    }

    myfile << numBuckets << "," << (total / numRepeats) << "\n";
    
    
    myfile << "--- \n" << "PolyHash 3 \n";

    total = 0;
    for (int j = 0; j < numRepeats; j++) {
        total += pow(polyHash3Sizes[j], 1.0/momentDegree);
    }

    myfile << numBuckets << "," << (total / numRepeats) << "\n";


    myfile.close();
}