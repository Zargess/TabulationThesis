#include "Experiment.h"
#include <iostream>
#include <string>

int* createListInputSizes(int n, double increaseFactor, int size) {
    int* resList = new int[size];
    int tempN = n;
    for (int i = 0; i < size; i++) {
        resList[i] = tempN;
        tempN = tempN * increaseFactor;
    }
    return resList;
};

Experiment::Experiment() {}

Experiment::~Experiment() {}

void Experiment::init(int n, int numRepeats, std::string filename) {
    this->n = n;
    this->numRepeats = numRepeats;
    this->filename = filename;
    setup();
}