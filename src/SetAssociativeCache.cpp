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
    m_NumberOfSets = (unsigned int)(cacheSize) / (m_LineSize * m_NumberOfWays);
    m_NumberOfOffsetBits = (uint32_t)(log2(m_LineSize));
    m_NumberOfIndexBits = (uint32_t)log2(m_NumberOfSets);
    m_NumberOfTagBits = 32 - (m_NumberOfOffsetBits + m_NumberOfIndexBits);
}

SetAssociativeCache::~SetAssociativeCache()
{

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
    m_Sets = new Set[m_NumberOfSets];
    for (uint32_t i = 0; i < m_NumberOfSets; i++)
    {
        m_Sets[i].tags = new unsigned int[m_NumberOfWays];        // in words
        m_Sets[i].validBits = new unsigned short[m_NumberOfWays]; // in words
        m_Sets[i].frequency = new unsigned int[m_NumberOfWays];
        m_Sets[i].leastUsed = new unsigned int[m_NumberOfWays];
        for (int j = 0; j < m_NumberOfWays; j++)
        {
            m_Sets[i].leastUsed[j] = 0;
            m_Sets[i].frequency[j] = 0;
            m_Sets[i].tags[j] = 0;
            m_Sets[i].validBits[j] = 0;
        }
    }
}

inline uint32_t SetAssociativeCache::GetTag(uint32_t address)
{
    return address >> (m_NumberOfIndexBits + m_NumberOfOffsetBits);
}
inline uint32_t SetAssociativeCache::GetSetIndex(uint32_t address)
{
    uint32_t mask = 0xFFFFFFFF >> (32 - m_NumberOfIndexBits);
    return ((address >> m_NumberOfOffsetBits) & mask);
}

bool SetAssociativeCache::IsInSet(unsigned int address)
{
    unsigned int setIndex = GetSetIndex(address);
    unsigned int tag = GetTag(address);

    // Referecne variable to ease the use of the array
    Set &set = m_Sets[setIndex];
#ifdef EXTREME_DEBUG
    LogSetInfo(setIndex);
#endif
    for (uint32_t i = 0; i < m_NumberOfWays; i++)
        if (set.validBits[i] == VALID && set.tags[i] == tag)
        {
            set.frequency[i]++;
            for (int j = 0; j < m_NumberOfWays; j++)
                if (i != j)
                    set.leastUsed[j]++;

            set.leastUsed[i] = 0; //reset current access index least used value
            return true;
        }

    return false;
}

cacheResType SetAssociativeCache::TestCache(unsigned int address)
{
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
    UpdateSet(address);
    return MISS;
}

void SetAssociativeCache::UpdateSet(unsigned int address)
{
    unsigned int setIndex = GetSetIndex(address);
    unsigned int tag = GetTag(address);
    Set &set = m_Sets[setIndex];
#ifdef EXTREME_DEBUG
    LogSetInfo(setIndex);
#endif

    uint32_t replacementIndex = FindNextReplacemntIndex(setIndex);
#if defined(NORM_DEBUG) || defined(EXTREME_DEBUG)
    LogUpdateInfo(setIndex, replacementIndex, tag, address);
#endif
    set.tags[replacementIndex] = tag;
    set.validBits[replacementIndex] = VALID;
}

uint32_t SetAssociativeCache::FindLeastFrequent(uint32_t setNumber)
{
    uint32_t leastIndex = 0;
    Set &currentSet = m_Sets[setNumber];
    uint32_t least = currentSet.frequency[0];
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
    uint32_t most = m_Sets[setNumber].leastUsed[0];
    uint32_t index = 0;
    for (uint32_t i = 0; i < m_NumberOfWays; i++)
    {
        if (m_Sets[setNumber].leastUsed[i] > most)
        {
            most = m_Sets[setNumber].leastUsed[i];
            index = i;
        }
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
        m_Sets[setNumber].frequency[replacementIndex] = 0;
        break;
    }
    case ReplacementPolicy::LRU:
    {
        replacementIndex = FindLeastRecentlyUsed(setNumber);
        for (int j = 0; j < m_NumberOfWays; j++)
        {
            if (replacementIndex != j)
                m_Sets[setNumber].leastUsed[j]++;
        }
        m_Sets[setNumber].leastUsed[replacementIndex] = 0;
        break;
    }
    }
    return replacementIndex;
}

#if defined(NORM_DEBUG) || defined(EXTREME_DEBUG)
void SetAssociativeCache::LogSetInfo(unsigned int setIndex)
{
    std::cout << "\n set index:" << setIndex << "\n";
    for (int i = 0; i < m_NumberOfWays; i++)
        std::cout << i << " - tag:" << m_Sets[setIndex].tags[i] << std::endl;
    std::cout << "\n\n";
}

void SetAssociativeCache::LogCacheInfo()
{
    std::cout << "Replacement Policy: " << g_RepPoliciesStrings[(int)m_ReplacmentPolicy] << std::endl;
    std::cout << "line size: " << m_LineSize << std::endl;
    std::cout << "number of ways : " << m_NumberOfWays << std::endl;
    std::cout << "Tag bits: " << m_NumberOfTagBits << std::endl;
    std::cout << "index bits: " << m_NumberOfIndexBits << std::endl;
    std::cout << "offset bits: " << m_NumberOfOffsetBits << std::endl;
    std::cout << "Number of Sets: " << m_NumberOfSets << std::endl;
}

void SetAssociativeCache::LogUpdateInfo(unsigned int setIndex, unsigned int replacementIndex, unsigned int tag, unsigned int address)
{
    std::cout << "+-----------------------------------+";
    std::cout << "ADDRESS: " << address << std::endl;
    std::cout << "setIndex: " << setIndex << std::endl;
    std::cout << "EXISTING TAG: " << m_Sets[setIndex].tags[replacementIndex] << std::endl;
    std::cout << "New Tag: " << tag << std::endl;
    std::cout << "Replacing at way number: " << replacementIndex << std::endl;
}
#endif
