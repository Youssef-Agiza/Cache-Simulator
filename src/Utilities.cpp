#include "../headers/Utilities.h"
#define NO_OF_Iterations 10 
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

	ExecuteExp(16, 8, 1);
	ExecuteExp(32, 8, 1);
	ExecuteExp(64, 8, 1);
	ExecuteExp(128, 8, 1);
}


void Exp2()
{
	ExecuteExp(32, 1, 2);
	ExecuteExp(32, 2, 2);
	ExecuteExp(32, 4, 2);
	ExecuteExp(32, 8, 2);
	ExecuteExp(32, 16, 2);
	ExecuteExp(32, 32, 2);
}

void ExecuteExp(int lineSize, int ways, int expNumber)
{
    SetAssociativeCache* caches [6];
    unsigned int hits[6];
	unsigned int hitRatio[6];
    memset(hits, 0, 6);
	memset(hitRatio, 0, 6);
    for(int i = 0; i < 6; i++)
        caches[i] = new SetAssociativeCache(ways, lineSize);

    Random randGen1;
    Random randGen2;
    for(int i = 0; i < NO_OF_Iterations; i++)
        for(int j = 0; j < 6; j++)
        {
            unsigned int address = GetAddress(j, randGen1, randGen2);
            if(caches[i]->TestCache(address))
                hits[j]++;
        }
    for (int i = 0; i < 6; i++)
		hitRatio[i] = (100 * hits[i] / NO_OF_Iterations);
	
	saveFiles(hitRatio, lineSize, ways, expNumber);

	for(uint32_t i=0;i<6;i++)
		delete caches[i];
}

void saveFiles(unsigned int  hitRatio[], int lineSize, int ways, int expNumber)
{
	std::ofstream outFile("Exp#"+ std::to_string(expNumber) + " & " + std::to_string(ways) + " ways" +".csv");
	outFile << lineSize << "," << hitRatio[0] << "," << hitRatio[1] << "," << hitRatio[2] << "," << hitRatio[3] << "," << hitRatio[4] << "," << hitRatio[5] << std::endl;
}

unsigned int GetAddress(int j, Random& randGen1, Random& randGen2)
{
	std::cout << "Entered GetAddress function" << j << std::endl;
    switch(j)
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





