#include <cstdint>
#include <string>
#pragma once
class Experiment {
    public:
        virtual ~Experiment();
        virtual void setup() = 0;
        virtual void run() = 0;
        virtual void store() = 0;
        void init(int n, int numRepeats, std::string filename);
        virtual void clean() = 0;
    protected:
        Experiment();
        int n;
        int numRepeats;
        std::string filename;
};