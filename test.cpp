#include <cstdlib>
#include <cstring>
#include <string>

#define MAX_THREADS					40
#define NUMBER_OF_INDIV_TESTRUNS	30
#define BUFFER_SIZE 				1024
#define MAX_INDEX					18

int main(int argc, char *argv[])
{
	char buffer[BUFFER_SIZE];
	unsigned int array[MAX_INDEX] = {1,2,3,4,5,6,7,8,10,12,14,16,20,24,28,32,36,40};

	//system(strcat(strcat(strcpy(buffer, "./listTest "), std::to_string(MAX_THREADS).c_str()), " ListContentCorrectness.txt correctness whole split full"));

	for(int z = 0; z < NUMBER_OF_INDIV_TESTRUNS; z++)
	{
		for(int index = 0; index < MAX_INDEX; index++)
		{
			//system(strcat(strcat(strcpy(buffer, "./listTest "), std::to_string(threads).c_str()), " ListContentCorrSort.txt performance split full"));
			//system(strcat(strcat(strcpy(buffer, "./listTest "), std::to_string(threads).c_str()), " ListContentBackSort.txt performance split full"));

			system(strcat(strcat(strcpy(buffer, "./listTest "), std::to_string(array[index]).c_str()), " ListContentRandom_80_10_10.txt performance split full"));
			system(strcat(strcat(strcpy(buffer, "./listTest "), std::to_string(array[index]).c_str()), " ListContentRandom_70_20_10.txt performance split full"));
			system(strcat(strcat(strcpy(buffer, "./listTest "), std::to_string(array[index]).c_str()), " ListContentRandom_60_20_20.txt performance split full"));
			system(strcat(strcat(strcpy(buffer, "./listTest "), std::to_string(array[index]).c_str()), " ListContentRandom_50_35_15.txt performance split full"));
			system(strcat(strcat(strcpy(buffer, "./listTest "), std::to_string(array[index]).c_str()), " ListContentRandom_40_30_30.txt performance split full"));
		}
	}

	return EXIT_SUCCESS;
}
