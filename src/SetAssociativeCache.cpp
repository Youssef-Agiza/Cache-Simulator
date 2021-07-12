#include "../headers/SetAssociativeCache.h"

SetAssociativeCache::SetAssociativeCache(unsigned int numberOfWays, unsigned int lineSize, ReplacmentPolicy policy, unsigned int cacheSize)
    : m_NumberOfWays(numberOfWays), m_LineSize(lineSize), m_ReplacmentPolicy(policy)
{
    m_NumberOfSets = (unsigned int)(CACHE_SIZE) / (m_LineSize * m_NumberOfWays);
    // 32 bit address - the number of bits we have for the words
    m_NumberOfOffsetBits = (unsigned int)(log2(m_LineSize));
    m_NumberOfIndexBits = (unsigned int)(log2(m_NumberOfSets));
    m_NumberOfTagBits = 32 - (m_NumberOfIndexBits + m_NumberOfOffsetBits);

    InitalizeSets(m_LineSize);
    std::cout << "Tag bits: " << m_NumberOfTagBits << std::endl;
    std::cout << "Number of Sets: " << m_NumberOfSets << std::endl;
}

SetAssociativeCache::~SetAssociativeCache()
{

    for (uint32_t i = 0; i < m_NumberOfSets; i++)
    {
        delete[] m_Sets[i].tags;
        delete[] m_Sets[i].validBits;
        delete[] m_Sets[i].recentlyUsage;
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
        m_Sets[i].recentlyUsage = new unsigned int[m_NumberOfWays];
        for (int j = 0; j < m_NumberOfWays; j++)
        {
            m_Sets[i].recentlyUsage[j] = 0;
            m_Sets[i].frequency[j] = 0;
            m_Sets[i].tags[j] = 0;
            m_Sets[i].validBits[j] = 0;
        }
    }
}

inline unsigned int SetAssociativeCache::getSetIndex(unsigned int address)
{
    unsigned int mask = 0xFFFFFFFF >> ((unsigned int)(32 - m_NumberOfIndexBits));
    std::cout << "Mask: " << mask << " address: " << std::hex << address << "\n";

    return (address >> m_NumberOfOffsetBits) & mask;
}

inline unsigned int SetAssociativeCache::getTag(unsigned int address)
{
    return address >> (m_NumberOfIndexBits + m_NumberOfOffsetBits);
}

bool SetAssociativeCache::IsInSet(unsigned int address)
{
    unsigned int setIndex = getSetIndex(address);
    unsigned int tag = getTag(address);

    // Referecne variable to ease the use of the array
    Set &set = m_Sets[setIndex];

    bool found = false;

    //shift to the right with the number of byteoffset + index bits
    for (uint32_t i = 0; i < m_NumberOfWays; i++)
        if (set.validBits[i] == VALID && set.tags[i] == tag)
        {
            set.frequency[i]++;
            found = true;
        }
        else
            set.recentlyUsage[i]++;

    return found;
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
    unsigned int setIndex = getSetIndex(address);
    unsigned int tag = getTag(address);

    Set &set = m_Sets[setIndex];

    uint32_t replacementIndex = FindNextReplacemntIndex(setIndex);

    std::cout << "setIndex: " << setIndex << std::endl;
    std::cout << "EXISTING TAG: " << set.tags[replacementIndex] << std::endl;
    std::cout << "New Tag: " << tag << std::endl;
    std::cout << "Replacing at way number: " << replacementIndex << std::endl;

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
uint32_t SetAssociativeCache::FindLeastRecentlyUsed(uint32_t setIndex)
{
    uint32_t most = m_Sets[setIndex].recentlyUsage[0];
    uint32_t index = 0;
    for (uint32_t i = 0; i < m_NumberOfWays; i++)
        if (m_Sets[setIndex].recentlyUsage[i] > most)
        {
            most = m_Sets[setIndex].recentlyUsage[i];
            index = i;
        }

    return index;
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
        m_Sets[setNumber].recentlyUsage[replacementIndex] = 0;
        break;
    }
    }
    return replacementIndex;
}
