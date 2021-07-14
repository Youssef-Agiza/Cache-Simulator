#pragma once

#include "PCH.h"
#include "Random.h"
#include "SetAssociativeCache.h"

#ifndef UTILITIES_H
#define UTILITIES_H


void HandleInput();

void Exp1(ReplacementPolicy policy);
void Exp2(ReplacementPolicy policy);

void getHitRatio(double hitRatio[], SetAssociativeCache* caches[], unsigned int hits[]);
void initalizeVariables(SetAssociativeCache* caches[], unsigned int hits[], double hitRatio[], int ways, int lineSize, ReplacementPolicy policy);
void freePointers(SetAssociativeCache* caches[]);
void ExecuteExp(int bytes, int ways, int expNumber, ReplacementPolicy policy);
void SaveFiles(double hitRatio[], int bytes, int ways, int expNumber, ReplacementPolicy p);
unsigned int GetAddress(int j, Random &randGen1, Random &randGen2);
void error(void);

/*this should be moved to a sepearte file along with other tests*/

/*
 * Test 1: performs a test on 2-way cache of size 128 bytes and line size 32 bytes with RLU as replacement policy
 *         addresses are fixed used from an online reference. 
 * Result: The test performs two iterations on the addresses chosen and the hit ratio is supposed to be 44% in the first
 *          and 77% second time.
 * YOUTUBE LINK: https://www.youtube.com/watch?v=quZe1ehz-EQ&t=201s
 * 
 * Test 4: performs a replacement test on 1-way cache set assiociative cache to see if it has a direct cache behaviour
 * Result: Expected HIT rate = 0
 * 
 */
void Test1();
void Test2();
void Test3();
void Test4();

//The patternA: Creates addresses of intervals of 32
//This is used for testing 32
uint32_t *GetPatternA(uint32_t &size);
uint32_t *GetPatternB(uint32_t &size);
//The patternC: Creates alternating addresses of 130 and 2
uint32_t *GetPatternC(uint32_t &size);
#endif
