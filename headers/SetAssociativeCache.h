#pragma once

#ifndef SET_ASSOCIATIVE_CACHE_H
#define SET_ASSOCIATIVE_CACHE_H

#include "PCH.h"
enum cacheResType
{
    MISS = 0,
    HIT = 1
};

enum Validity
{
    NOT_VALID = 0,
    VALID = 1
};

class SetAssociativeCache
{
public:
    SetAssociativeCache(unsigned int numberOfWays, unsigned int blockSize);
    ~SetAssociativeCache();
    cacheResType TestCache(unsigned int address);

private:
    bool IsInSet(unsigned int address);
    void InitalizeSets(unsigned int lineSize);
    void UpdateSet(unsigned int address);

    struct Set
    {
        unsigned int nextReplacementIndex;
        unsigned int *tags;
        unsigned short *validBits;
    };

    Set *m_Sets;
    unsigned int m_NumberOfWays;
    unsigned int m_LineSize;
    unsigned int m_NumberOfSets;
    unsigned int m_NumberOfTagBits;
};

#endif