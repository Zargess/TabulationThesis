#include "Experiment.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"
#include "../datagenerator/DataGenerator.h"
#include "../hashfunction/Tabulation.h"
#include <string>

class SimpleMixedMaxBucketSizeExperiment : public Experiment {
	public:
	    SimpleMixedMaxBucketSizeExperiment();
		SimpleMixedMaxBucketSizeExperiment(int n, int numBuckets, int numRepeats, RandomNumberGenerator* rnd, DataGenerator* g, std::string filename);
        ~SimpleMixedMaxBucketSizeExperiment();
        void setup();
        void run();
        void store();
        void clean();
	private:
		int numBuckets;

        RandomNumberGenerator * rnd;
        DataGenerator * gen;

		double * simpleMaxBucketSizes;
        double * mixedMaxBucketSizes;
        double * multiplyShiftMaxBucketSizes;
        double * polyHash3MaxBucketSizes;
        double * polyHash2MaxBucketSizes;
};