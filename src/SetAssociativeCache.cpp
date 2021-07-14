#include "../headers/SetAssociativeCache.h"

SetAssociativeCache::SetAssociativeCache(unsigned int numberOfWays,
                                         unsigned int lineSize,
                                         ReplacementPolicy policy,
                                         unsigned int cacheSize) : m_NumberOfWays(numberOfWays),
                                                                   m_LineSize(lineSize),
                                                                   m_ReplacmentPolicy(policy)
{
    InitializeBitNumbers(cacheSize);
    InitalizeSets(m_LineSize);

#if defined(NORM_DEBUG) || defined(EXTREME_DEBUG)
    LogCacheInfo();
#endif
}

void SetAssociativeCache::InitializeBitNumbers(uint32_t cacheSize)
{
    // Initializing several parameters needed
    m_NumberOfSets = (unsigned int)(cacheSize) / (m_LineSize * m_NumberOfWays);
    m_NumberOfOffsetBits = (uint32_t)(log2(m_LineSize));
    m_NumberOfIndexBits = (uint32_t)log2(m_NumberOfSets);
}

SetAssociativeCache::~SetAssociativeCache()
{
    //Deleting the memory we used in the class
    for (uint32_t i = 0; i < m_NumberOfSets; i++)
    {
        delete[] m_Sets[i].tags;
        delete[] m_Sets[i].validBits;
        delete[] m_Sets[i].leastUsed;
        delete[] m_Sets[i].frequency;
    }

    delete[] m_Sets;
}

void SetAssociativeCache::InitalizeSets(unsigned int lineSize)
{
    // Initalizing sets
    m_Sets = new Set[m_NumberOfSets];
    for (uint32_t i = 0; i < m_NumberOfSets; i++)
    {
        m_Sets[i].tags = new unsigned int[m_NumberOfWays];
        m_Sets[i].validBits = new unsigned short[m_NumberOfWays];
        m_Sets[i].frequency = new unsigned int[m_NumberOfWays];
        m_Sets[i].leastUsed = new unsigned int[m_NumberOfWays];
        for (int j = 0; j < m_NumberOfWays; j++)
        {
            //Every set starts with all the ways frequency = 0 and
            //the number of times it was used = 0
            m_Sets[i].leastUsed[j] = 0;
            m_Sets[i].frequency[j] = 0;
            m_Sets[i].tags[j] = 0;
            m_Sets[i].validBits[j] = 0;
        }
    }
}

inline uint32_t SetAssociativeCache::GetTag(uint32_t address)
{
    //The tag is the address excluding the offsetbits and the index bits
    return address >> (m_NumberOfIndexBits + m_NumberOfOffsetBits);
}

inline uint32_t SetAssociativeCache::GetSetIndex(uint32_t address)
{
    //mask with 1's equal to the number of index bits (inspired by RVIC32IC Disassembler)
    uint32_t mask = 0xFFFFFFFF >> (32 - m_NumberOfIndexBits);
    return ((address >> m_NumberOfOffsetBits) & mask);
}

bool SetAssociativeCache::IsInSet(unsigned int address)
{
    unsigned int setIndex = GetSetIndex(address);
    unsigned int tag = GetTag(address);

    // Referecne variable to ease the use of the array
    Set &set = m_Sets[setIndex];

    //For debugging purposes only
#ifdef EXTREME_DEBUG
    LogSetInfo(setIndex);
#endif

    //We go through all the ways in the set we found and we check
    //If the tag in the address is in the set, then we return it is & we update
    //the rest of the set parameters
    for (uint32_t i = 0; i < m_NumberOfWays; i++)
        if (set.validBits[i] == VALID && set.tags[i] == tag)
        {
            //increase the frequency of line being accessed
            set.frequency[i]++;
            for (int j = 0; j < m_NumberOfWays; j++)
                if (i != j)
                    //increase least used for the other blocks in the set(read the comments for LeastRecentlyUsed function)
                    set.leastUsed[j]++;

            //reset current access index least used value
            set.leastUsed[i] = 0;
            return true;
        }

    return false;
}

cacheResType SetAssociativeCache::TestCache(unsigned int address)
{
    //checking if the address is in the test and if so we return it is a hit
    if (IsInSet(address))
    {
#if defined(NORM_DEBUG) || defined(EXTREME_DEBUG)
        std::cout << "===>Hits Address: " << address << std::endl;
#endif
        return HIT;
    }

#if defined(NORM_DEBUG) || defined(EXTREME_DEBUG)
    std::cout << "<===Misses Address: " << address << std::endl;
#endif
    // Else we update the set with that address and return MISS
    UpdateSet(address);
    return MISS;
}

void SetAssociativeCache::UpdateSet(unsigned int address)
{
    unsigned int setIndex = GetSetIndex(address);
    unsigned int tag = GetTag(address);
    Set &set = m_Sets[setIndex];

    //We find which line we want to replace in the set
    uint32_t replacementIndex = FindNextReplacemntIndex(setIndex);
    // and we replace it
    set.tags[replacementIndex] = tag;
    set.validBits[replacementIndex] = VALID;
#if defined(NORM_DEBUG) || defined(EXTREME_DEBUG)
    LogUpdateInfo(setIndex, replacementIndex, tag, address);
#endif
}

uint32_t SetAssociativeCache::FindLeastFrequent(uint32_t setNumber)
{
    Set &currentSet = m_Sets[setNumber];

    //loop over all the blocks in the set chosen and
    //... find the one with the least frequency to
    //replace it
    uint32_t least = currentSet.frequency[0];
    uint32_t leastIndex = 0;
    for (uint32_t i = 0; i < m_NumberOfWays; i++)
        if (currentSet.frequency[i] < least)
        {
            least = currentSet.frequency[i];
            leastIndex = i;
        }
    return leastIndex;
}
uint32_t SetAssociativeCache::FindLeastRecentlyUsed(uint32_t setNumber)
{
    Set &currentSet = m_Sets[setNumber];

    uint32_t most = currentSet.leastUsed[0];
    uint32_t index = 0;

    //loop over all the blocks in the chosen set and
    //... find the one with largest "leastUsed"  value
    // i.e. the block which was least recently accessed
    for (uint32_t i = 0; i < m_NumberOfWays; i++)
        if (currentSet.leastUsed[i] > most)
        {
            most = currentSet.leastUsed[i];
            index = i;
        }
    return index;
}
uint32_t SetAssociativeCache::FindNextReplacemntIndex(uint32_t setNumber)
{
    uint32_t replacementIndex = 0;
    switch (m_ReplacmentPolicy)
    {
    case ReplacementPolicy::Random:
    {
        replacementIndex = rand() % m_NumberOfWays;
        break;
    }
    case ReplacementPolicy::LFU:
    {
        replacementIndex = FindLeastFrequent(setNumber);
        //reset the frequency of the line that will be replaced to 1(since it accessed only 1 time now)
        m_Sets[setNumber].frequency[replacementIndex] = 1;
        break;
    }
    case ReplacementPolicy::LRU:
    {
        replacementIndex = FindLeastRecentlyUsed(setNumber);

        /*
        increase leastUsed for all the lines in the set except the one we just replaced
        because they weren't used in this memory access (i.e. there weren't "recently used" in this access)
        */
        for (int j = 0; j < m_NumberOfWays; j++)
            if (replacementIndex != j)
                m_Sets[setNumber].leastUsed[j]++;

        //reset lesatUsed for the line being accessed to 0, which means it is the most recently used line
        m_Sets[setNumber].leastUsed[replacementIndex] = 0;
        break;
    }
    }
    return replacementIndex;
}

#if defined(NORM_DEBUG) || defined(EXTREME_DEBUG)
void SetAssociativeCache::LogSetInfo(unsigned int setIndex)
{
    std::cout << "******LOGGING SET INFO******\n";
    std::cout << "+-----------------------------------+\n";
    std::cout << "\nset index:" << setIndex << "\n";
    for (int i = 0; i < m_NumberOfWays; i++)
        std::cout << i << " - tag:" << m_Sets[setIndex].tags[i] << " VALID: " << m_Sets[setIndex].validBits[i] << std::endl;
    std::cout << "+-----------------------------------+\n";
}

void SetAssociativeCache::LogCacheInfo()
{
    std::cout << "******LOGGING CACHE INFO******\n";
    std::cout << "+-----------------------------------+\n";
    std::cout << "Replacement Policy: " << g_RepPoliciesStrings[(int)m_ReplacmentPolicy] << std::endl;
    std::cout << "line size: " << m_LineSize << std::endl;
    std::cout << "number of ways : " << m_NumberOfWays << std::endl;
    std::cout << "index bits: " << m_NumberOfIndexBits << std::endl;
    std::cout << "offset bits: " << m_NumberOfOffsetBits << std::endl;
    std::cout << "Number of Sets: " << m_NumberOfSets << std::endl;
    std::cout << "+-----------------------------------+\n";
}

void SetAssociativeCache::LogUpdateInfo(unsigned int setIndex, unsigned int replacementIndex, unsigned int tag, unsigned int address)
{

    std::cout << "******LOGGING UPDATE INFO******\n";
    std::cout << "+-----------------------------------+";
    std::cout << "ADDRESS: " << address << std::endl;
    std::cout << "setIndex: " << setIndex << std::endl;
    std::cout << "EXISTING TAG: " << m_Sets[setIndex].tags[replacementIndex] << std::endl;
    std::cout << "New Tag: " << tag << std::endl;
    std::cout << "Replacing at way number: " << replacementIndex << std::endl;
    std::cout << "+-----------------------------------+\n";
    std::cout << "THE NEW SET INFO\n";
    LogSetInfo(setIndex);
}
#endif
