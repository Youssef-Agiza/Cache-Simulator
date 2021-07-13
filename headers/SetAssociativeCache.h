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

enum class ReplacementPolicy
{
    Random = 0,
    LFU,
    LRU
};

// Global string array that has the string values of the enum
const std::string g_RepPoliciesStrings[] = {"Random", "LFU", "LRU"};

class SetAssociativeCache
{
public:
    SetAssociativeCache(unsigned int numberOfWays,
                        unsigned int lineSize,
                        ReplacementPolicy policy = ReplacementPolicy::Random,
                        unsigned int cacheSize = CACHE_SIZE);
    ~SetAssociativeCache();
    cacheResType TestCache(unsigned int address);

private:
    struct Set
    {
        unsigned short *validBits;
        unsigned int *tags;
        unsigned int *frequency;
        unsigned int *leastUsed;
    };

    /*returns true if the address if HIT, false if MISS*/
    bool IsInSet(unsigned int address);
    void InitalizeSets(unsigned int lineSize);

    /*updates the cache on MISS*/
    void UpdateSet(unsigned int address);

    /*
     * Utilities
     * /

    /* precondition: address is not more than 32 bits
       postcondition: return the tag in the address*/
    inline uint32_t GetTag(uint32_t address);

    //precondition: address to be accesssed  is not more than 32 bits
    //postcondition: return the index of the set to be searched
    inline uint32_t GetSetIndex(uint32_t address);

    //precondition: cacheSize is given, m_NumberOfWays and m_NumberOfSets are initalized correctly
    void InitializeBitNumbers(uint32_t cacheSize);

    /*Replacement Policy helpers*/
    uint32_t FindLeastFrequent(uint32_t setNumber);
    uint32_t FindNextReplacemntIndex(uint32_t setNumber);
    uint32_t FindLeastRecentlyUsed(uint32_t setNumber);

#if defined(NORM_DEBUG) || defined(EXTREME_DEBUG)
    /*for debugging*/
    void LogSetInfo(unsigned int setIndex);
    void LogCacheInfo();
    void LogUpdateInfo(unsigned int setIndex, unsigned int replacementIndex, unsigned int tag, unsigned int address);
#endif

    unsigned int m_LineSize;
    Set *m_Sets;
    ReplacementPolicy m_ReplacmentPolicy;

    //number of bits/ways
    uint32_t m_NumberOfSets;
    uint32_t m_NumberOfWays;
    uint32_t m_NumberOfIndexBits;
    uint32_t m_NumberOfOffsetBits;
};

#endif