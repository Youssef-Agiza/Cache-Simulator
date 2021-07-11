#pragma once
#include "common.h"
#ifndef SET_H
#define SET_H

class Set
{
public:
    Set() = default;
    void Initialize(unsigned int lineSize, unsigned int numOfLines, const unsigned int numOfTagBits);

    bool IsInSet(unsigned int tag);
    void ReplaceLine(unsigned int address);
    ~Set();

private:
    unsigned int numOfTagBits,
        *tags,
        nextReplacementIndex,
        numOfLines,
        numberOfWords;

    unsigned short *validBits;
    enum Validity
    {
        NOT_VALID = 0,
        VALID = 1
    };
};

#endif //SET_H