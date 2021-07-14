#include "../headers/Utilities.h"

#define NO_OF_Iterations 1000000
void HandleInput()
{
	std::string answer = "";
	//Function pointer array that contains pointers to the different experiments/tests that can be made
	void (*expFunctions[])(ReplacementPolicy) = {Exp1, Exp2};
	void (*testFunctions[])() = {TestA, TestB, TestC, TestD};
	//calculauting the number of functions in the array
	uint32_t numOfFunctions = sizeof(expFunctions) / sizeof(void (*)(ReplacementPolicy));
	//Taking the user input to decide which experiement to be excuted and graphed
	while (answer == "")
	{
		std::cout << "Enter the a valid request number\n";
		std::cout << "1-Experiment #1: measuring and graphing the hit ratios for each line size\n";
		std::cout << "2-Experiment #2: measuring and graphing the hit ratios for different number of ways\n";
		std::cout << "3-Test #a\n";
		std::cout << "4-Test #b\n";
		std::cout << "5-Test #c\n";
		std::cout << "6-Test #d\n";
		std::cout << "Enter one digit indicating the required experiment number (1 or 2), or a test number (3 (#a), 4 (#b), 5 (#c), or 6 (#d))\n";
		std::cin >> answer;

		if (!isdigit(answer[0]) || answer[0] > (numOfFunctions + '0' + '4'))
		{
			answer = "";
			continue;
		}
		int32_t expNumber = answer[0] - '0' - 1;

		//Calling the correct experiment function
		if (answer == "1" || answer == "2")
			for (int i = 0; i < 3; i++)
				expFunctions[expNumber](ReplacementPolicy(i));
		else
			testFunctions[expNumber - 3]();
	}
}
//A function to excute different parts of first experiment
//Takes the replacement policy for the cache as a parameter
void Exp1(ReplacementPolicy policy)
{
	ExecuteExp(16, 1, 1, policy);
	ExecuteExp(32, 1, 1, policy);
	ExecuteExp(64, 1, 1, policy);
	ExecuteExp(128, 1, 1, policy);
	std::cout << "\nSaved Part1 in ./Outputs/\n";
	ExecuteExp(16, 8, 1, policy);
	ExecuteExp(32, 8, 1, policy);
	ExecuteExp(64, 8, 1, policy);
	ExecuteExp(128, 8, 1, policy);
	std::cout << "\nSaved Part2 in ./Outputs/";
}

void Exp2(ReplacementPolicy policy)
{
	ExecuteExp(32, 1, 2, policy);
	ExecuteExp(32, 2, 2, policy);
	ExecuteExp(32, 4, 2, policy);
	ExecuteExp(32, 8, 2, policy);
	ExecuteExp(32, 16, 2, policy);
	ExecuteExp(32, 32, 2, policy);
	std::cout << "\nSaved Exp2\n";
}

//A function to excute different parts of experiments 1 and 2
//Takes the line size, ways, and replacementpolicy of the n-way associative cache, also takes the experiment number in which the cache is being excuted.
void ExecuteExp(int lineSize, int ways, int expNumber, ReplacementPolicy policy)
{
	//Declaring 6 n-way associative caches to test the different 6 random addresses generators functions
	SetAssociativeCache *caches[6];

	//An array to hold the number of hits of each random addresses generator function for that cache
	unsigned int hits[6];

	//An array to hold the final ratio of hits of each random addresses generator function for that cache
	double hitRatio[6];

	initalizeVariables(caches, hits, hitRatio, ways, lineSize, policy);

	getHitRatio(hitRatio, caches, hits);

	//Calling a function to save the results in CSV files
	SaveFiles(hitRatio, lineSize, ways, expNumber, policy);

	freePointers(caches);
}

//A function to get the hitRatio for each cache
void getHitRatio(double hitRatio[], SetAssociativeCache *caches[], unsigned int hits[])
{
	Random randGen1;
	Random randGen2;
	//Looping the required number of iterations times to get an address from each memory generator and test it on the cache (whether it is a hit or a miss)
	for (uint32_t i = 0; i < NO_OF_Iterations; i++)
		for (uint32_t j = 0; j < 6; j++)
		{
			unsigned int address = GetAddress(j, randGen1, randGen2);
			if (caches[j]->TestCache(address) == HIT)
				hits[j]++;
		}
	//Calculating the hit ratio for each of the 6 caches
	for (int i = 0; i < 6; i++)
		hitRatio[i] = (100.0 * (double)(hits[i]) / (double)NO_OF_Iterations);
}

//A function to initalize the values for the caches, and the number of hits and the hitRatios
void initalizeVariables(SetAssociativeCache *caches[], unsigned int hits[], double hitRatio[], int ways, int lineSize, ReplacementPolicy policy)
{
	for (uint32_t i = 0; i < 6; i++)
	{
		caches[i] = new SetAssociativeCache(ways, lineSize, policy, CACHE_SIZE);
		hits[i] = 0;
		hitRatio[i] = 0.0;
	}
}

//A function to free the pointers.
void freePointers(SetAssociativeCache *caches[])
{
	for (uint32_t i = 0; i < 6; i++)
		delete caches[i];
}

//A function to output the results of each experiment in their proper CSV (comma seperated) files
void SaveFiles(double hitRatio[], int lineSize, int ways, int expNumber, ReplacementPolicy p)
{
	std::ofstream outFile("outputs/Exp#" + std::to_string(expNumber) + "_" + ((expNumber == 1) ? std::to_string(ways) : "32") + "ways_" + g_RepPoliciesStrings[(int)p] + ".csv", std::ios::app | std::ios::ate);
	int length = outFile.tellp();
	if (length == 0)
		outFile << ((expNumber == 1) ? "line sizes" : "ways") << ",A,B,C,D,E,F\n";
	outFile << ((expNumber == 1) ? lineSize : ways) << "," << hitRatio[0] << "," << hitRatio[1] << "," << hitRatio[2] << "," << hitRatio[3] << "," << hitRatio[4] << "," << hitRatio[5] << std::endl;
	outFile.close();
}

//A function to get an address from the different random addresses generator functions
//Takes the index of the required memGen function to operate on, and 2 different random generators (because of the different random seeds in memGen A and D)
unsigned int GetAddress(int j, Random &randGen1, Random &randGen2)
{
	switch (j)
	{
	case 0:
		return randGen1.memGenA();
	case 1:
		return randGen1.memGenB();
	case 2:
		return randGen1.memGenC();
	case 3:
		return randGen2.memGenD();
	case 4:
		return randGen1.memGenE();
	case 5:
		return randGen1.memGenF();
	default:
		error();
	}

	return 0;
}

//A function to handle exception and exit the program
void error(void)
{
	std::cout << "Exception handled while getting address.\n";
	std::cout << "Exiting program\n";
	exit(0);
}

/************************Tests*********************************/

void TestA()
{
	int answer;
	std::cout << "Replacement policy:\n1-random\n2-LFU\n3-LRU\n";
	std::cin >> answer;
	ReplacementPolicy p;
	switch (answer)
	{
	case 1:
		p = ReplacementPolicy::Random;
		break;
	case 2:
		p = ReplacementPolicy::LFU;
		break;
	case 3:
		p = ReplacementPolicy::LRU;
		break;
	}
	SetAssociativeCache cache(2u, 32u, p, 128u);
	uint32_t addresses[] = {0x070, 0x080, 0x068, 0x190, 0x084, 0x178, 0x08C, 0xF00, 0x064};
	uint32_t arrSize = sizeof(addresses) / sizeof(uint32_t);
	uint32_t hitNumber = 0;
	double hitRatio;
	int times = 0;
there:
	for (uint32_t i = 0; i < arrSize; i++)
	{
		std::cout << "+---------------------+\n NEW TEST CACHE\n";
		if (cache.TestCache(addresses[i]) == HIT)
		{
			std::cout << "The address: 0x" << std::hex << addresses[i] << " performs HIT\n";
			hitNumber++;
		}
		else
			std::cout << "The address: 0x" << std::hex << addresses[i] << " performs MISS\n";
	}
	times++;
	hitRatio = (100.0 * hitNumber) / (arrSize);
	std::cout << "Hit ratio is " << hitRatio << std::endl;
	hitNumber = 0;
	if (times == 1)
	{
		std::cout << "\n +-------------+\nGoing There\n";
		goto there;
	}
	std::cout << "End of the simulation\n";
}

void TestB()
{
	uint32_t size;
	uint32_t *pattern = GetPatternB(size);
	SetAssociativeCache cache(4, 8, ReplacementPolicy::LRU, 128);
	uint32_t hitNumber = 0;
	for (uint32_t i = 0; i < size; i++)
	{
		if (cache.TestCache(pattern[i]))
			hitNumber++;
	}

	std::cout << (100.0 * hitNumber / size);
	delete[] pattern;
}

void TestC()
{
	uint32_t size;
	uint32_t *pattern = GetPatternA(size);
	SetAssociativeCache cache(4, 8, ReplacementPolicy::LRU, 1024);
	uint32_t hitNumber = 0;
	for (uint32_t i = 0; i < size; i++)
	{
		if (cache.TestCache(pattern[i]))
			hitNumber++;
	}

	std::cout << (100.0 * hitNumber / size);
	delete[] pattern;
}

void TestD()
{
	uint32_t size;
	uint32_t *pattern = GetPatternC(size);
	SetAssociativeCache cache(1, 4, ReplacementPolicy::LRU, 128);
	uint32_t hitNumber = 0;
	for (uint32_t i = 0; i < size; i++)
	{
		if (cache.TestCache(pattern[i]))
			hitNumber++;
	}

	std::cout << (100.0 * hitNumber / size);
	delete[] pattern;
}

uint32_t *GetPatternA(uint32_t &size)
{
	size = 100;
	uint32_t *array = new uint32_t[size];
	for (uint32_t i = 0; i < size; i++)
	{
		array[i] = i * 32;
	}
	return array;
}

//To test Cache ways
//8 bytes line
//4-way set associative
//128B CacheSize
uint32_t *GetPatternB(uint32_t &size)
{
	size = 10;
	uint32_t *array = new uint32_t[size];
	uint32_t startingValue = 0b00100;
	for (uint32_t i = 0; i < size; i++)
		array[i] = (i << 5) | startingValue;
	return array;
}

//Testing replacement for 1-way SetAssociativeCache (direct cache behaviour)
//Expected HIT rate = 0
uint32_t *GetPatternC(uint32_t &size)
{
	size = 16;
	uint32_t *array = new uint32_t[size];
	array[0] = 2;

	for (uint32_t i = 1; i < size; i++)
	{
		//2 = 0b00000010
		//130 = 0b10000010
		//Same set but different TAG
		if (array[i - 1] == 2)
			array[i] = 130;
		else
			array[i] = 2;
	}
	return array;
}
