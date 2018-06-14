#include <iostream>
#include <cstdlib>
#include "experiments/SimpleMixedTimeExperiment.h"
#include "experiments/SimpleMixedMomentExperiment.h"
#include "experiments/SimpleMixedMaxBucketSizeExperiment.h"
#include "experiments/Simple8vs16Experiment.h"
#include "experiments/DSTimingExperiment.h"
#include "experiments/DSWorstCaseExperiment.h"
#include "experiments/DSCompareExperiment.h"
#include "experiments/DSCacheExperiment.h"
#include "experiments/SingleDSCacheExperiment.h"
#include "randomnumbergenerator/CRandNumberGenerator.h"
#include "randomnumbergenerator/RandomNumberGenerator.h"
#include "hashfunction/HashFunction.h"
#include "hashfunction/StandardHashing.h"
#include "hashfunction/Tabulation.h"
#include "datagenerator/SyntheticDataGenerator.h"
#include "datagenerator/RealDataGenerator.h"
#include <bitset>
#include "cuckoo/Cuckoo.h"

using namespace std;

int main(int argc, char* argv[]) {
    RandomNumberGenerator* unirnd = new UniformDistMT();
    RandomNumberGenerator * rnd = new PolyHashRNG(unirnd, 20);

    DataGenerator* bow = new BagOfWordsDataGenerator(rnd);
    DataGenerator* bst18 = new BadSimpleTabulationDataGenerator(18,18,18,18);  
    DataGenerator* bst42 = new BadSimpleTabulationDataGenerator(42,42,42,42);   
    DataGenerator* bst64 = new BadSimpleTabulationDataGenerator(64,64,64,64); 
    DataGenerator* bst90 = new BadSimpleTabulationDataGenerator(90,90,90,90); 
    DataGenerator* oneToN = new OneToNDataGenerator();

    /* Useful for running a single experiment */
    /*
    int n = 104976;
    int numRepeats = 100;
    int numBuckets = 50000;
    //SimpleMixedMaxBucketSizeExperiment smmoment(n, numBuckets, numRepeats, rnd, bow, "bow_50k_poly2rnd");
    //SimpleMixedMomentExperiment smmoment(n, numBuckets, numRepeats, rnd, bow, "bow_50k_poly2rnd", 32);
    DSTimingExperiment smmoment(n, numRepeats, rnd, bow, "test_newpoly", "cuckoo", 30, 0);
    //DSWorstCaseExperiment smmoment(50000, rnd, bow, "test_worst_case", "probing", 50, 100);
    std::cout << "I am going to start experiment" << std::endl;
    smmoment.run();
    std::cout << "I am going to store" << std::endl;
    smmoment.store();
    std::cout << "I am done" << std::endl; 
    


    */
    /* Used when running multiple experiments  */
    std::cout << "I am initializing" << std::endl;
    std::vector<Experiment*> exps;

    // DataStructure
    int n = 104976; //104976; //pow(42, 4);
    int numRepeats = 10000;
    int numQueries = 10 * n;
    int numDS = 2000;

    int loadsize10 = 1049623;
    int loadsize20 = 524893;
    int loadsize30 = 349927;
    int loadsize40 = 262459;
    int loadsize80 = 131221;

    std::vector<double>* querySizes = new std::vector<double>();
    querySizes->push_back(3.0);
    querySizes->push_back(6.0);
    querySizes->push_back(9.0);
    querySizes->push_back(12.0);

    int size = atoi(argv[1]) == 40 ? loadsize40 : loadsize80;

    exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bow, "Cuckoo_BoW_40_rerun", "cuckoo", 40, numQueries));

    //exps.push_back(new SingleDSCacheExperiment(n, loadsize40, numRepeats, rnd, bow, "", argv[2]));
    //exps.push_back(new DSCacheExperiment(n, size, numRepeats, rnd, bow, "not important", argv[2]));

    //querySizes->push_back(10.0);
    /*
    exps.push_back(new DSCacheExperiment(n, numRepeats, rnd, bow, "not important", "probing", "simple", 40));
    exps.push_back(new DSCacheExperiment(n, numRepeats, rnd, bow, "not important", "probing", "mixed", 40));
    */
    //exps.push_back(new DSCacheExperiment(n, numRepeats, rnd, bow, "not important", "multshift", 40));
    /*
    exps.push_back(new DSCacheExperiment(n, numRepeats, rnd, bow, "not important", "probing", "poly2", 40));
    exps.push_back(new DSCacheExperiment(n, numRepeats, rnd, bow, "not important", "probing", "poly3", 40));

    exps.push_back(new DSCacheExperiment(n, numRepeats, rnd, bow, "not important", "probing", "simple", 80));
    exps.push_back(new DSCacheExperiment(n, numRepeats, rnd, bow, "not important", "probing", "mixed", 80));
    exps.push_back(new DSCacheExperiment(n, numRepeats, rnd, bow, "not important", "probing", "multshift", 80));
    exps.push_back(new DSCacheExperiment(n, numRepeats, rnd, bow, "not important", "probing", "poly2", 80));
    exps.push_back(new DSCacheExperiment(n, numRepeats, rnd, bow, "not important", "probing", "poly3", 80));

    
    exps.push_back(new DSCacheExperiment(n, loadsize40, numRepeats, rnd, bow, "not important", "double", "simple"));
    exps.push_back(new DSCacheExperiment(n, loadsize40, numRepeats, rnd, bow, "not important", "double", "mixed"));
    */
    //exps.push_back(new DSCacheExperiment(n, loadsize40, numRepeats, rnd, bow, "not important", "multshift"));
    /*
    exps.push_back(new DSCacheExperiment(n, loadsize40, numRepeats, rnd, bow, "not important", "double", "poly2"));
    exps.push_back(new DSCacheExperiment(n, loadsize40, numRepeats, rnd, bow, "not important", "double", "poly3"));

    exps.push_back(new DSCacheExperiment(n, loadsize80, numRepeats, rnd, bow, "not important", "double", "simple"));
    exps.push_back(new DSCacheExperiment(n, loadsize80, numRepeats, rnd, bow, "not important", "double", "mixed"));
    exps.push_back(new DSCacheExperiment(n, loadsize80, numRepeats, rnd, bow, "not important", "double", "multshift"));
    exps.push_back(new DSCacheExperiment(n, loadsize80, numRepeats, rnd, bow, "not important", "double", "poly2"));
    exps.push_back(new DSCacheExperiment(n, loadsize80, numRepeats, rnd, bow, "not important", "double", "poly3"));
    */
    //exps.push_back(new DSCompareExperiment(n, numRepeats, rnd, oneToN, "loadfactor40_50tho_oneToN", "simple", 40, querySizes));
    //exps.push_back(new DSCompareExperiment(n, numRepeats, rnd, bst64, "loadfactor40_50mil", "mixed", 40, querySizes));
    //exps.push_back(new DSCompareExperiment(n, numRepeats, rnd, bow, "loadfactor40__Bow", "multshift", 40, querySizes));
    //exps.push_back(new DSCompareExperiment(n, numRepeats, rnd, bst64, "loadfactor40_50mil", "poly2", 40, querySizes));
    //exps.push_back(new DSCompareExperiment(n, numRepeats, rnd, bst64, "loadfactor40_50mil", "poly3", 40, querySizes));


    /*
    exps.push_back(new DSWorstCaseExperiment(n, rnd, bow, "probing_bow_10", "probing", 10, numDS));
    exps.push_back(new DSWorstCaseExperiment(n, rnd, bow, "probing_bow_20", "probing", 20, numDS));
    exps.push_back(new DSWorstCaseExperiment(n, rnd, bow, "probing_bow_30", "probing", 30, numDS));
    exps.push_back(new DSWorstCaseExperiment(n, rnd, bow, "probing_bow_40", "probing", 40, numDS));
    
    exps.push_back(new DSWorstCaseExperiment(n, rnd, bst42, "probing_bst_10", "probing", 10, numDS));
    exps.push_back(new DSWorstCaseExperiment(n, rnd, bst18, "probing_bst_20", "probing", 20, numDS));
    exps.push_back(new DSWorstCaseExperiment(n, rnd, bst18, "probing_bst_30", "probing", 30, numDS));
    exps.push_back(new DSWorstCaseExperiment(n, rnd, bst42, "probing_bst_40", "probing", 40, numDS));

    exps.push_back(new DSWorstCaseExperiment(n, rnd, bow, "cuckoo_bow_10", "cuckoo", 10, numDS));
    exps.push_back(new DSWorstCaseExperiment(n, rnd, bow, "cuckoo_bow_20", "cuckoo", 20, numDS));
    exps.push_back(new DSWorstCaseExperiment(n, rnd, bow, "cuckoo_bow_30", "cuckoo", 30, numDS));
    exps.push_back(new DSWorstCaseExperiment(n, rnd, bow, "cuckoo_bow_40", "cuckoo", 40, numDS));
    
    exps.push_back(new DSWorstCaseExperiment(n, rnd, bst18, "cuckoo_bst_10", "cuckoo", 10, numDS));
    exps.push_back(new DSWorstCaseExperiment(n, rnd, bst18, "cuckoo_bst_20", "cuckoo", 20, numDS));
    exps.push_back(new DSWorstCaseExperiment(n, rnd, bst18, "cuckoo_bst_30", "cuckoo", 30, numDS));
    exps.push_back(new DSWorstCaseExperiment(n, rnd, bst18, "cuckoo_bst_40", "cuckoo", 40, numDS));
    
    
    exps.push_back(new DSTimingExperiment(n, loadsize10, numRepeats, rnd, bow, "Double_BoW_10", "double", numQueries));
    exps.push_back(new DSTimingExperiment(n, loadsize20, numRepeats, rnd, bow, "Double_BoW_20", "double", numQueries));
    exps.push_back(new DSTimingExperiment(n, loadsize30, numRepeats, rnd, bow, "Double_BoW_30", "double", numQueries));
    exps.push_back(new DSTimingExperiment(n, loadsize40, numRepeats, rnd, bow, "Double_BoW_40", "double", numQueries));

    exps.push_back(new DSTimingExperiment(n, loadsize10, numRepeats, rnd, bst18, "Double_BST_10", "double", numQueries));
    exps.push_back(new DSTimingExperiment(n, loadsize20, numRepeats, rnd, bst18, "Double_BST_20", "double", numQueries));
    exps.push_back(new DSTimingExperiment(n, loadsize30, numRepeats, rnd, bst18, "Double_BST_30", "double", numQueries));
    exps.push_back(new DSTimingExperiment(n, loadsize40, numRepeats, rnd, bst18, "Double_BST_40", "double", numQueries));
    
    
    exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bow, "Probing_BoW_80", "probing", 80, numQueries));
    exps.push_back(new DSTimingExperiment(n, loadsize80, numRepeats, rnd, bow, "Double_BoW_80", "double", numQueries));
    
    exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bst18, "Probing_BST_80", "probing", 80, numQueries));
    exps.push_back(new DSTimingExperiment(n, loadsize80, numRepeats, rnd, bst18, "Double_BST_80", "double", numQueries));
    /*exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bow, "Probing_BoW_20", "probing", 20, numQueries));
    exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bow, "Probing_BoW_30", "probing", 30, numQueries));
    exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bow, "Probing_BoW_40", "probing", 40, numQueries));

    exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bst18, "Probing_BST_10", "probing", 10, numQueries));
    exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bst18, "Probing_BST_20", "probing", 20, numQueries));
    exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bst18, "Probing_BST_30", "probing", 30, numQueries));
    exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bst18, "Probing_BST_40", "probing", 40, numQueries));
    
    exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bow, "Cuckoo_BoW_10", "cuckoo", 10, numQueries));
    exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bow, "Cuckoo_BoW_20", "cuckoo", 20, numQueries));
    exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bow, "Cuckoo_BoW_30", "cuckoo", 30, numQueries)); 
    exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bow, "Cuckoo_BoW_40", "cuckoo", 40, numQueries));

    exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bst18, "Cuckoo_BST_10", "cuckoo", 10, numQueries));
    exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bst18, "Cuckoo_BST_20", "cuckoo", 20, numQueries));
    exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bst18, "Cuckoo_BST_30", "cuckoo", 30, numQueries));
    exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bst18, "Cuckoo_BST_40", "cuckoo", 40, numQueries));
    */
/*
    exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bow, "Probing_BoW_10_Valgrind", "probing", 10, numQueries));
    exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bow, "Probing_BoW_10_Valgrind", "probing", 10, numQueries));
    exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bow, "Probing_BoW_10_Valgrind", "double", 10, numQueries));
    exps.push_back(new DSTimingExperiment(n, numRepeats, rnd, bow, "Probing_BoW_10_Valgrind", "double", 10, numQueries));
*/
/*
    // Moment
    int numBuckets = 2;
    exps.push_back(new SimpleMixedMomentExperiment (n, numBuckets, numRepeats, rnd, bow, "BoW_2b", 32));
    exps.push_back(new SimpleMixedMomentExperiment (n, numBuckets, numRepeats, rnd, bst18, "BST_2b", 32));
    
    numBuckets = 50;
    exps.push_back(new SimpleMixedMomentExperiment (n, numBuckets, numRepeats, rnd, bow, "BoW_50b", 32));
    exps.push_back(new SimpleMixedMomentExperiment (n, numBuckets, numRepeats, rnd, bst18, "BST_50b", 32));

    numBuckets = 50000;
    exps.push_back(new SimpleMixedMomentExperiment (n, numBuckets, numRepeats, rnd, bow, "BoW_50000b", 32));
    exps.push_back(new SimpleMixedMomentExperiment (n, numBuckets, numRepeats, rnd, bst18, "BST_50000b", 32));

    numBuckets = 200000;
    exps.push_back(new SimpleMixedMomentExperiment (n, numBuckets, numRepeats, rnd, bow, "BoW_200000b", 32));
    exps.push_back(new SimpleMixedMomentExperiment (n, numBuckets, numRepeats, rnd, bst18, "BST_200000b", 32));
    
    // MaxBucket
    numRepeats = 100000;
    numBuckets = 2;
    exps.push_back(new SimpleMixedMaxBucketSizeExperiment (n, numBuckets, numRepeats, rnd, bow, "BoW_2b"));
    exps.push_back(new SimpleMixedMaxBucketSizeExperiment (n, numBuckets, numRepeats, rnd, bst18, "BST_2b"));
    
    numBuckets = 50;
    exps.push_back(new SimpleMixedMaxBucketSizeExperiment (n, numBuckets, numRepeats, rnd, bow, "BoW_50b"));
    exps.push_back(new SimpleMixedMaxBucketSizeExperiment (n, numBuckets, numRepeats, rnd, bst18, "BST_50b"));

    numBuckets = 50000;
    exps.push_back(new SimpleMixedMaxBucketSizeExperiment (n, numBuckets, numRepeats, rnd, bow, "BoW_50000b"));
    exps.push_back(new SimpleMixedMaxBucketSizeExperiment (n, numBuckets, numRepeats, rnd, bst18, "BST_50000b"));

    numBuckets = 200000;
    exps.push_back(new SimpleMixedMaxBucketSizeExperiment (n, numBuckets, numRepeats, rnd, bow, "BoW_200000b"));
    exps.push_back(new SimpleMixedMaxBucketSizeExperiment (n, numBuckets, numRepeats, rnd, bst18, "BST_200000b"));

    */
    // Timing
    //numRepeats = 100000;
    //exps.push_back(new SimpleMixedTimeExperiment(10000000, numRepeats, rnd,  oneToN, "TimeAll_cache"));
    
    int i = 0;
    int totalExps = exps.size();
    for(Experiment* e : exps) {
        i++;
        std::cout << "I am going to start experiment: " << i << "/" << totalExps << std::endl;
        e->run();
        std::cout << "I am going to store" << std::endl;
        e->store();
        e->clean();
        std::cout << "I am done with experiment: " << i << "/" << totalExps <<  std::endl;
    }
    
    delete unirnd;
    delete rnd;
    delete bow;
    delete bst18;
    delete oneToN;
}