#pragma once

#include "PCH.h"
#include "Random.h"
#include "SetAssociativeCache.h"

#ifndef UTILITIES_H
#define UTILITIES_H

void HandleInput();

void Exp1(ReplacementPolicy policy);
void Exp2(ReplacementPolicy policy);

void getHitRatio(double hitRatio[], SetAssociativeCache *caches[], unsigned int hits[]);
void initalizeVariables(SetAssociativeCache *caches[], unsigned int hits[], double hitRatio[], int ways, int lineSize, ReplacementPolicy policy);
void freePointers(SetAssociativeCache *caches[]);
void ExecuteExp(int bytes, int ways, int expNumber, ReplacementPolicy policy);
void SaveFiles(double hitRatio[], int bytes, int ways, int expNumber, ReplacementPolicy p);
unsigned int GetAddress(int j, Random &randGen1, Random &randGen2);
void error(void);

//The patternA1: Creates alternating addresses of 130 and 2
uint32_t *GetPatternA(uint32_t &size);
//The patternB: Creates addresses of intervals of 4
uint32_t *GetPatternB(uint32_t &size);

/*
 * Test A: performs a replacement test on 1-way cache set assiociative cache to see if it has a direct cache behaviour
 * Result: Expected HIT rate = 0
 * 
 * Test B:Testing whether the cache responds to adding differnet addresses of the same set 
 * but with different tags to the same set (way-storing test).
 * Result: 0% hit rate
 * 
 * Test C: performs a test on 2-way cache of size 128 bytes and line size 32 bytes with RLU as replacement policy
 *         addresses are fixed used from an online reference. 
 * Result: The test performs two iterations on the addresses chosen and the hit ratio is supposed to be 44% in the first
 *          and 77% second time.
 * YOUTUBE LINK: https://www.youtube.com/watch?v=quZe1ehz-EQ&t=201s
 * 
 */
void TestA();
void TestB();
void TestC();

#endif
