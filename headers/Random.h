#pragma once
#include "PCH.h"

class Random
{
public:
    Random();
    unsigned int rand_();
    unsigned int memGenA();
    unsigned int memGenB();
    unsigned int memGenC();
    unsigned int memGenD();
    unsigned int memGenE();
    unsigned int memGenF();
private:
    unsigned int m_w;
    unsigned int m_z;
};