#include <cstdlib>
#include <cstring>
#include <string>

#define MAX_THREADS					30
#define NUMBER_OF_INDIV_TESTRUNS	50
#define BUFFER_SIZE 				1024

int main(int argc, char *argv[])
{
	char buffer[BUFFER_SIZE];

	system(strcat(strcat(strcpy(buffer, "./listTest "), std::to_string(MAX_THREADS).c_str()), " ListContentCorrectness.txt correctness"));

	for(int threads = 0; threads < MAX_THREADS; threads++)
	{
		for(int z = 0; z < NUMBER_OF_INDIV_TESTRUNS; z++)
		{
			system(strcat(strcat(strcpy(buffer, "./listTest "), std::to_string(threads).c_str()), " ListContentCorrSort.txt correctness"));
			system(strcat(strcat(strcpy(buffer, "./listTest "), std::to_string(threads).c_str()), " ListContentBackSort.txt correctness"));

			system(strcat(strcat(strcpy(buffer, "./listTest "), std::to_string(threads).c_str()), " ListContentRandom_80_10_10.txt correctness"));
			system(strcat(strcat(strcpy(buffer, "./listTest "), std::to_string(threads).c_str()), " ListContentRandom_70_20_10.txt correctness"));
			system(strcat(strcat(strcpy(buffer, "./listTest "), std::to_string(threads).c_str()), " ListContentRandom_60_20_20.txt correctness"));
			system(strcat(strcat(strcpy(buffer, "./listTest "), std::to_string(threads).c_str()), " ListContentRandom_50_35_15.txt correctness"));
			system(strcat(strcat(strcpy(buffer, "./listTest "), std::to_string(threads).c_str()), " ListContentRandom_40_30_30.txt correctness"));
		}
	}

	return EXIT_SUCCESS;
}
