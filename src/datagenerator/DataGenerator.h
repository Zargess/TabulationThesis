#include <cstdint>
#pragma once

class DataGenerator {

    public:
        virtual uint32_t* createDataSet(int n) = 0;
        virtual uint32_t getNumberNotInData() = 0;
};

