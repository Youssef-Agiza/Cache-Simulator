#include "../headers/SetAssociativeCache.h"

SetAssociativeCache::SetAssociativeCache(unsigned int numberOfWays, unsigned int lineSize)
    : m_NumberOfWays(numberOfWays), m_LineSize(lineSize)
{
    m_NumberOfSets = (unsigned int)(CACHE_SIZE) / (m_LineSize * m_NumberOfWays);
    // 32 bit address - the number of bits we have for the words
    m_NumberOfTagBits = 32 - (unsigned int)(log2(m_LineSize) + log2(m_NumberOfSets));
    InitalizeSets(m_LineSize);
}

SetAssociativeCache::~SetAssociativeCache()
{
    //Deleting used memory
    delete[] m_Sets;
}

bool SetAssociativeCache::IsInSet(unsigned int address)
{
    unsigned int setIndex = (address << m_NumberOfTagBits) >> (m_NumberOfTagBits + (unsigned int)(log2(m_LineSize)));

    // Referecne variable to ease the use of the array
    Set &set = m_Sets[setIndex];
    unsigned int tag = address >> (32 - m_NumberOfTagBits);
    for (uint32_t i = 0; i < m_NumberOfWays; i++)
        if (set.validBits[i] == VALID && set.tags[i] == tag){
            return true;
        }
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
    unsigned int setIndex = (address << m_NumberOfTagBits) >> (m_NumberOfTagBits + (unsigned int)(log2(m_LineSize)));
    unsigned int tag = address >> (32 - m_NumberOfTagBits);
    Set &set = m_Sets[setIndex];

    set.tags[set.nextReplacementIndex] = tag;
    set.validBits[set.nextReplacementIndex] = VALID;
    set.nextReplacementIndex = rand() % m_NumberOfWays;
}

void SetAssociativeCache::InitalizeSets(unsigned int lineSize)
{
    m_Sets = new Set[m_NumberOfSets];

    for (uint32_t i = 0; i < m_NumberOfSets; i++)
    {
        m_Sets[i].nextReplacementIndex = 0;

        m_Sets[i].tags = new unsigned int[m_NumberOfWays]; // in words
        m_Sets[i].validBits = new unsigned short[m_NumberOfWays]; // in words
        for(int j = 0; j < m_NumberOfWays; j++)
        {
            m_Sets[i].tags[j] = 0;
            m_Sets[i].validBits[j] = 0;
        }
    }
}