#include "../headers/Set.h"

void Set::Initialize(unsigned int lineSize, unsigned int numOfLines, const unsigned int numOfTagBits)
{
    numberOfWords = (lineSize / 4 == 0) ? 1 : lineSize / 4;

    tags = new unsigned int[numOfLines]; // in words
    memset(tags, 0, numOfLines);

    validBits = new unsigned short[numOfLines]; // in words
    memset(validBits, 0, numOfLines);           //All are invalid

    blockIndex = 0;
    this->numOfLines = numOfLines;
}

bool Set::IsInSet(unsigned int tag)
{
    for (uint32_t i = 0; i < numOfLines; i++)
        if (validBits[i] == VALID && tags[i] == tag)
            return true;

    return false;
}
void Set::ReplaceLine(unsigned int address)
{
    unsigned int tag = address >> (32 - numOfTagBits);
    tags[blockIndex] = tag;
    validBits[blockIndex] = VALID;
    blockIndex = (blockIndex + 1) % numOfLines;
}
Set::~Set()
{
    delete[] tags;
    delete[] validBits;
}
