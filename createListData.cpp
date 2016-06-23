#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>

#define BUFFER_SIZE 		1024
#define NUMBER_OF_ELEMENTS	50000
#define MAX_WRITE			20

const char *SEQ_ORDERED_DATA_FILENAME = "ListContentCorrSort.txt";
const char *SEQ_BACKWARDS_ORDERED_FILENAME = "ListContentBackSort.txt";
const char *RANDOM_DATA_FILENAME = "ListContentRandom.txt";
const char *FRONT_HEAVY_DATA_FILENAME = "ListContentFront.txt";
const char *BACK_HEAVY_DATA_FILENAME = "ListContentBackHeavy.txt";

/**
 * @brief Call: createListData <fileName>
 */
int main(int argc, char *argv[])
{
	char buffer[BUFFER_SIZE];
	std::ofstream sequOrderedDataFile;
	std::ofstream sequBackwardsOrderedDataFile;
	std::ofstream randomDataFile;
	std::ofstream frontHeavyDataFile;
	std::ofstream backHeavyDataFile;

	try
	{
		sequOrderedDataFile.open(SEQ_ORDERED_DATA_FILENAME, std::fstream::out);
		sequBackwardsOrderedDataFile.open(SEQ_BACKWARDS_ORDERED_FILENAME, std::fstream::out);
		randomDataFile.open(RANDOM_DATA_FILENAME, std::fstream::out);
		frontHeavyDataFile.open(FRONT_HEAVY_DATA_FILENAME, std::fstream::out);
		backHeavyDataFile.open(BACK_HEAVY_DATA_FILENAME, std::fstream::out);
	}
	catch(std::exception ex)
	{
		//do nothing, error handling below
	}
	if((sequOrderedDataFile.rdstate() & std::fstream::failbit) != 0)
	{
		std::cout << "Error when opening file: " << SEQ_ORDERED_DATA_FILENAME << std::endl;
		exit(EXIT_FAILURE);
	}
	if((sequBackwardsOrderedDataFile.rdstate() & std::fstream::failbit) != 0)
	{
		std::cout << "Error when opening file: " << SEQ_BACKWARDS_ORDERED_FILENAME << std::endl;
		exit(EXIT_FAILURE);
	}
	if((randomDataFile.rdstate() & std::fstream::failbit) != 0)
	{
		std::cout << "Error when opening file: " << RANDOM_DATA_FILENAME << std::endl;
		exit(EXIT_FAILURE);
	}
	if((frontHeavyDataFile.rdstate() & std::fstream::failbit) != 0)
	{
		std::cout << "Error when opening file: " << FRONT_HEAVY_DATA_FILENAME << std::endl;
		exit(EXIT_FAILURE);
	}
	if((backHeavyDataFile.rdstate() & std::fstream::failbit) != 0)
	{
		std::cout << "Error when opening file: " << BACK_HEAVY_DATA_FILENAME << std::endl;
		exit(EXIT_FAILURE);
	}

	//generate the ascending sorted list data
	for(unsigned int z = 0; z < NUMBER_OF_ELEMENTS; z++)
	{
		strcpy(buffer, std::to_string(z).c_str());
		sequOrderedDataFile.write(buffer, strlen(buffer));
		sequOrderedDataFile.put('\n');
	}
	sequOrderedDataFile.close();

	//generate the descending sorted list data
	for(int z = NUMBER_OF_ELEMENTS-1; z >= 0; z--)
	{
		strcpy(buffer, std::to_string(z).c_str());
		sequBackwardsOrderedDataFile.write(buffer, strlen(buffer));
		sequBackwardsOrderedDataFile.put('\n');
	}
	sequBackwardsOrderedDataFile.close();

	//generate a pseudo random list of data
	srand(time(NULL));
	for(unsigned int z = 0; z < NUMBER_OF_ELEMENTS; z++)
	{
		strcpy(buffer, std::to_string(rand()).c_str());
		randomDataFile.write(buffer, strlen(buffer));
		randomDataFile.put('\n');
	}
	randomDataFile.close();

	/*XXX not sure how to fill the following two files or how to actually use them ... */
	//generate the "front heavy" list data
	//for(unsigned int z = 0; z < NUMBER_OF_ELEMENTS; z++)
	//{
	//	strcpy(buffer, std::to_string(z).c_str());
	//	frontHeavyDataFile.write(buffer, strlen(buffer));
	//	frontHeavyDataFile.put('\n');
	//}
	frontHeavyDataFile.close();

	////generate the "back heavy" list data
	//for(unsigned int z = 0; z < NUMBER_OF_ELEMENTS; z++)
	//{
	//	strcpy(buffer, std::to_string(z).c_str());
	//	backHeavyDataFile.write(buffer, strlen(buffer));
	//	backHeavyDataFile.put('\n');
	//}
	backHeavyDataFile.close();

	return EXIT_SUCCESS;
}
