#include "Experiment.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"
#include "../datagenerator/DataGenerator.h"
#include "../hashfunction/Tabulation.h"

#pragma once
class SimpleMixedMomentExperiment : public Experiment {
    public:
        SimpleMixedMomentExperiment();
        SimpleMixedMomentExperiment(int n, int numBuckets, int numRepeats, RandomNumberGenerator* rnd, DataGenerator* g, std::string filename, int momentDegree);
        ~SimpleMixedMomentExperiment();
        void setup();
        void run();
        void store();
        void clean();
    private:
        void emptyBuckets();
        int momentDegree;
        int numBuckets;
        RandomNumberGenerator * rnd;
        DataGenerator * gen;
        
        double * simpleBucketSizes;
        double * mixedBucketSizes;
        double * multiplyShiftBucketSizes;
        double * polyHash3BucketSizes;
        double * polyHash2BucketSizes;

        double * simpleMomentSizes;
        double * mixedMomentSizes;
        double * multiplyShiftSizes;
        double * polyHash3Sizes;
        double * polyHash2Sizes;
};