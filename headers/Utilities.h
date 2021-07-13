#pragma once

#include "PCH.h"
#include "Random.h"
#include "SetAssociativeCache.h"

#ifndef UTILITIES_H
#define UTILITIES_H

/*
    add a function to take replacement policy from the user and use the input to initialize the caches.
*/
void HandleInput();

void Exp1(ReplacementPolicy policy);
void Exp2(ReplacementPolicy policy);

/*
    possible restructure of the ExectureExp() for better readability:
    1- Create function to initalize the variables at the begining
    2- Create a function to do the loops and the calculations
    3- Create a function to free the pointers at the end
    4- Move the logic from ExecuteExp to those functions and call them
*/
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
 */
void Test1();

#endif
