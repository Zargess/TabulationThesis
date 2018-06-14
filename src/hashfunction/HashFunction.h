#include <cstdint>

#ifndef HASHFUNCTION_H
#define HASHFUNCTION_H
class HashFunction {
    public:
    virtual uint32_t hash(uint32_t n) = 0;
    virtual HashFunction* clone() = 0;
};
#endif