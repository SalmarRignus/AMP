#include <iostream>
#include <fstream>
#include <cstring>

#define BUFFER_SIZE 1024

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


	return EXIT_SUCCESS;
}
