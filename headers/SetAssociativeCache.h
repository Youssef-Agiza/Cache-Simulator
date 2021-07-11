#pragma once

#ifndef SET_ASSOCIATIVE_CACHE_H
#define SET_ASSOCIATIVE_CACHE_H

#include "Set.h"

#define CACHE_SIZE (64 * 1024)
enum cacheResType
{
    MISS = 0,
    HIT = 1
};

class SetAssociativeCache
{
public:
    SetAssociativeCache(unsigned int numberOfWays, unsigned int blockSize);
    ~SetAssociativeCache();
    cacheResType TestCache(unsigned int address);

private:
    void Initialize();

    Set *m_Sets;
    unsigned int m_NumberOfWays;
    unsigned int m_BlockSize;
};

#endif