#include "../headers/Utilities.h"
#include <climits>
#include <random>
#define NO_OF_Iterations 1000000
void HandleInput()
{
	std::string answer = "";
	while (answer == "")
	{
		std::cout << "Enter the experiment number\n";
		std::cout << "Experiment #1: measuring and graphing the hit ratios for each line size\n";
		std::cout << "Experiment #2: measuring and graphing the hit ratios for different number of ways\n";
		std::cout << "Enter one digit indicating the required experiment number (1 or 2)\n";
		std::cin >> answer;
		switch (stoi(answer))
		{
		case 1:
			Exp1();
			break;
		case 2:
			Exp2();
			break;
		default:
			std::cout << "Invalid input!\n";
			answer = "";
			break;
		}
	}
}

void Exp1()
{
	ExecuteExp(16, 1, 1);
	ExecuteExp(32, 1, 1);
	ExecuteExp(64, 1, 1);
	ExecuteExp(128, 1, 1);
	std::cout << "\nSaved PHASE1: Outputs/Exp#1 & 1 ways.csv\n";
	ExecuteExp(16, 8, 1);
	ExecuteExp(32, 8, 1);
	ExecuteExp(64, 8, 1);
	ExecuteExp(128, 8, 1);
	std::cout << "\nSaved PHASE2: Outputs/Exp#1 & 8 ways.csv\n";
}

void Exp2()
{
	ExecuteExp(32, 1, 2);
	ExecuteExp(32, 2, 2);
	ExecuteExp(32, 4, 2);
	ExecuteExp(32, 8, 2);
	ExecuteExp(32, 16, 2);
	ExecuteExp(32, 32, 2);
	std::cout << "\nSaved PHASE1: Outputs/Exp#2 & 32 ways.csv\n";
}

void ExecuteExp(int lineSize, int ways, int expNumber)
{
	SetAssociativeCache *caches[6];
	unsigned int hits[6];
	double hitRatio[6];
	for (uint32_t i = 0; i < 6; i++)
	{
		caches[i] = new SetAssociativeCache(ways, lineSize, ReplacmentPolicy::LRU, CACHE_SIZE);
		hits[i] = 0;
		hitRatio[i] = 0.0;
	}

	Random randGen1;
	Random randGen2;
	for (uint32_t i = 0; i < NO_OF_Iterations; i++)
		for (uint32_t j = 0; j < 6; j++)
		{
			unsigned int address = GetAddress(j, randGen1, randGen2);
			if (caches[j]->TestCache(address) == HIT)
				hits[j]++;
		}
	for (int i = 0; i < 6; i++)
		hitRatio[i] = (100.0 * (double)(hits[i]) / (double)NO_OF_Iterations);

	saveFiles(hitRatio, lineSize, ways, expNumber);
	for (uint32_t i = 0; i < 6; i++)
		delete caches[i];
}

void saveFiles(double hitRatio[], int lineSize, int ways, int expNumber)
{
	std::ofstream outFile("Outputs/Exp#" + std::to_string(expNumber) + " & " + ((expNumber == 1) ? std::to_string(ways) : "32") + " ways" + ".csv", std::ios::app | std::ios::ate);
	int length = outFile.tellp();
	if (length == 0)
		outFile << " , A,B,C,D,E,F\n";
	outFile << ((expNumber == 1) ? lineSize : ways) << "," << hitRatio[0] << "," << hitRatio[1] << "," << hitRatio[2] << "," << hitRatio[3] << "," << hitRatio[4] << "," << hitRatio[5] << std::endl;
	outFile.close();
}

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

void error(void)
{
	std::cout << "Exception handled while getting address.\n";
	std::cout << "Exiting program\n";
	exit(0);
}

/********************** Tests*********************************/

void Test1()
{
	int answer;
	std::cout << "Replacement policy:\n1-random\n2-LFU\n3-LRU\n";
	std::cin >> answer;
	ReplacmentPolicy p;
	switch (answer)
	{
	case 1:
		p = ReplacmentPolicy::Random;
		break;
	case 2:
		p = ReplacmentPolicy::LFU;
		break;
	case 3:
		p = ReplacmentPolicy::LRU;
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