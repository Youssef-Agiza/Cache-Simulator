#pragma once

#ifndef COMMON_H
#define COMMON_H

#define DEBUG_LEVEL 1
#if (DEBUG_LEVEL == 1)
#define NORM_DEBUG
#elif (DEBUG_LEVEL == 2)
#define EXTREME_DEBUG
#endif

#define DBG 1
#define DRAM_SIZE 64 * 1024 * 1024
#define CACHE_SIZE 64 * 1024

#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <cmath>
#include <fstream>
#include <bitset>

#endif //COMMON_H