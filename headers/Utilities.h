#pragma once

#include "PCH.h"
#include "Random.h"
#include "SetAssociativeCache.h"

#ifndef UTILITIES_H
#define UTILITIES_H

void HandleInput();
void Exp1();
void Exp2();
void ExecuteExp(int bytes, int ways, int expNumber);
void saveFiles(double  hitRatio[], int bytes, int ways, int expNumber);
unsigned int GetAddress(int j, Random& randGen1, Random& randGen2);
void error(void);


#endif



