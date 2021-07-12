#include "../headers/SetAssociativeCache.h"

SetAssociativeCache::SetAssociativeCache(unsigned int numberOfWays, unsigned int lineSize, ReplacmentPolicy policy, unsigned int cacheSize)
    : m_NumberOfWays(numberOfWays), m_LineSize(lineSize), m_ReplacmentPolicy(policy)
{
    m_NumberOfSets = (unsigned int)(cacheSize) / (m_LineSize * m_NumberOfWays);
    // 32 bit address - the number of bits we have for the words
    m_NumberOfOffsetBits = (uint32_t)(log2(m_LineSize));
    m_NumberOfIndexBits = (uint32_t)log2(m_NumberOfSets);
    m_NumberOfTagBits = 32 - (m_NumberOfOffsetBits + m_NumberOfIndexBits);
    InitalizeSets(m_LineSize);
#ifdef _DEBUG
    std::cout << "Tag bits: " << m_NumberOfTagBits << std::endl;
    std::cout << "Number of Sets: " << m_NumberOfSets << std::endl;
#endif
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

inline unsigned int SetAssociativeCache::GetSetIndex(unsigned int address)
{
    unsigned int mask = 0xFFFFFFFF >> ((unsigned int)(32 - m_NumberOfIndexBits));
    std::cout << "Mask: " << mask << " address: " << std::hex << address << "\n";

    return (address >> m_NumberOfOffsetBits) & mask;
}

inline unsigned int SetAssociativeCache::GetTag(unsigned int address)
{
    return address >> (m_NumberOfIndexBits + m_NumberOfOffsetBits);
}

bool SetAssociativeCache::IsInSet(unsigned int address)
{
    unsigned int setIndex = GetSetIndex(address);
    unsigned int tag = GetTag(address);

    // Referecne variable to ease the use of the array
    Set &set = m_Sets[setIndex];
    for (uint32_t i = 0; i < m_NumberOfWays; i++)
        if (set.validBits[i] == VALID && set.tags[i] == tag)
        {
            set.frequency[i]++;
            for (int j = 0; j < m_NumberOfWays; j++)
            {
                if (i != j)
                    set.leastUsed[j]++;
            }
            return true;
        }
        else
            set.leastUsed[i]++;

    return false;
}

cacheResType SetAssociativeCache::TestCache(unsigned int address)
{
    if (IsInSet(address))
        return HIT;

    UpdateSet(address);
    return MISS;
}

void SetAssociativeCache::UpdateSet(unsigned int address)
{
    unsigned int setIndex = GetSetIndex(address);
    unsigned int tag = GetTag(address);
    Set &set = m_Sets[setIndex];

    uint32_t replacementIndex = FindNextReplacemntIndex(setIndex);
#ifdef _DEBUG
    std::cout << "setIndex: " << setIndex << std::endl;
    std::cout << "EXISTING TAG: " << set.tags[replacementIndex] << std::endl;
    std::cout << "New Tag: " << tag << std::endl;
    std::cout << "Replacing at way number: " << replacementIndex << std::endl;
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

        return index;
    }
}
uint32_t SetAssociativeCache::FindNextReplacemntIndex(uint32_t setNumber)
{
    uint32_t replacementIndex = 0;
    switch (m_ReplacmentPolicy)
    {
    case ReplacmentPolicy::Random:
    {
        replacementIndex = rand() % m_NumberOfWays;
        break;
    }
    case ReplacmentPolicy::LFU:
    {
        replacementIndex = FindLeastFrequent(setNumber);
        m_Sets[setNumber].frequency[replacementIndex] = 0;
        break;
    }
    case ReplacmentPolicy::LRU:
    {
        replacementIndex = FindLeastRecentlyUsed(setNumber);
        for (int j = 0; j < m_NumberOfWays; j++)
        {
            if (replacementIndex != j)
                m_Sets[setNumber].leastUsed[j]++;
        }
        m_Sets[setNumber].leastUsed[replacementIndex] = 1;
        break;
    }
    }
    return replacementIndex;
}

inline uint32_t SetAssociativeCache::GetTag(uint32_t address)
{
    return address >> (m_NumberOfIndexBits + m_NumberOfOffsetBits);
}
inline uint32_t SetAssociativeCache::GetSetIndex(uint32_t address)
{
    unsigned int mask = 0xFFFFFFFF >> (32 - m_NumberOfIndexBits);
    return ((address >> m_NumberOfOffsetBits) & mask);
}