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

enum class ReplacmentPolicy
{
    Random,
    LFU,
    LRU
};

class SetAssociativeCache
{
public:
    SetAssociativeCache(unsigned int numberOfWays, unsigned int lineSize, ReplacmentPolicy policy = ReplacmentPolicy::Random);
    ~SetAssociativeCache();
    cacheResType TestCache(unsigned int address);

private:
    bool IsInSet(unsigned int address);
    void InitalizeSets(unsigned int lineSize);
    void UpdateSet(unsigned int address);
    uint32_t FindLeastFrequent(uint32_t setNumber);
    uint32_t FindNextReplacemntIndex(uint32_t setNumber);
    uint32_t FindLeastRecentlyUsed(uint32_t setNumber);
    struct Set
    {
        unsigned int *tags;
        unsigned short *validBits;
        unsigned int *frequency;
        unsigned int *recentlyUsage;
    };

    unsigned int m_NumberOfWays;
    unsigned int m_LineSize;
    Set *m_Sets;
    ReplacmentPolicy m_ReplacmentPolicy;
    unsigned int m_NumberOfSets;
    unsigned int m_NumberOfTagBits;
};

#endif