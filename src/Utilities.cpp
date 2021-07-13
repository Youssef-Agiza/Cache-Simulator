#include "../headers/Utilities.h"

#define NO_OF_Iterations 1000000
void HandleInput()
{
	std::string answer = "";
	//Function pointer array that contains pointers to the different experiments that can be made
	void (*expFunctions[])(ReplacementPolicy) = {Exp1, Exp2};
	//calculauting the number of functions in the array
	uint32_t numOfFunctions = sizeof(expFunctions) / sizeof(void (*)(ReplacementPolicy));
	while (answer == "")
	{
		std::cout << "Enter the experiment number\n";
		std::cout << "Experiment #1: measuring and graphing the hit ratios for each line size\n";
		std::cout << "Experiment #2: measuring and graphing the hit ratios for different number of ways\n";
		std::cout << "Enter one digit indicating the required experiment number (1 or 2)\n";
		std::cin >> answer;

		if (!isdigit(answer[0]) || answer[0] > (numOfFunctions + '0'))
		{
			answer = "";
			continue;
		}
		int32_t expNumber = answer[0] - '0' - 1;
		ReplacementPolicy policy = ReplacementPolicy::Random;
		answer = "";
		while (answer == "")
		{
			std::cout << "Enter the replacement policy you want the test to work with:\n";
			std::cout << "1- Random\n2- LFU (Least Frequently Used)\n3- LRU (Least Recently Used)\n";
			std::cin >> answer;
			if (!isdigit(answer[0]) || answer[0] > '3')
			{
				std::cout << "Entered Wrong value\n";
				answer = "";
				continue;
			}
		}
		policy = (ReplacementPolicy)(answer[0] - '0' - 1);
		//Calling the correct experiment function
		expFunctions[expNumber](policy);
		// switch (expNumber)
		// {
		// case 1:
		// 	Exp1(policy);
		// 	break;
		// case 2:
		// 	Exp2(policy);
		// 	break;
		// default:
		// 	std::cout << "Invalid input!\n";
		// 	answer = "";
		// 	break;
		// }
	}
}

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

void ExecuteExp(int lineSize, int ways, int expNumber, ReplacementPolicy policy)
{
	SetAssociativeCache *caches[6];
	unsigned int hits[6];
	double hitRatio[6];
	for (uint32_t i = 0; i < 6; i++)
	{
		caches[i] = new SetAssociativeCache(ways, lineSize, policy, CACHE_SIZE);
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

	SaveFiles(hitRatio, lineSize, ways, expNumber, policy);
	for (uint32_t i = 0; i < 6; i++)
		delete caches[i];
}

void SaveFiles(double hitRatio[], int lineSize, int ways, int expNumber, ReplacementPolicy p)
{
	std::ofstream outFile("Outputs/Exp#" + std::to_string(expNumber) + " & " + ((expNumber == 1) ? std::to_string(ways) : "32") + " ways - " + g_RepPoliciesStrings[(int)p] + ".csv", std::ios::app | std::ios::ate);
	int length = outFile.tellp();
	if (length == 0)
		outFile << ",A,B,C,D,E,F\n";
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

/************************Tests*********************************/

void Test1()
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