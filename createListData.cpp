#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "listOperation.h"


#define BUFFER_SIZE 		1024
#define NUMBER_OF_ELEMENTS	5000
#define MAX_WRITE			10


const char *SEQ_ORDERED_DATA_FILENAME = "ListContentCorrSort.txt";
const char *SEQ_BACKWARDS_ORDERED_FILENAME = "ListContentBackSort.txt";
const char *CORRECTNESS_FILENAME = "ListContentCorrectness.txt";
const char *RANDOM_DATA_FILENAME_80_10_10 = "ListContentRandom_80_10_10.txt";
const char *RANDOM_DATA_FILENAME_70_20_10 = "ListContentRandom_70_20_10.txt";
const char *RANDOM_DATA_FILENAME_60_20_20 = "ListContentRandom_60_20_20.txt";
const char *RANDOM_DATA_FILENAME_50_35_15 = "ListContentRandom_50_35_15.txt";
const char *RANDOM_DATA_FILENAME_40_30_30 = "ListContentRandom_40_30_30.txt";
const char *FRONT_HEAVY_DATA_FILENAME = "ListContentFront.txt";
const char *BACK_HEAVY_DATA_FILENAME = "ListContentBackHeavy.txt";


enum Operation SelectOp(int& containsOpCount, int& addOpCount, int& removeOpCount);
int WriteOp(std::ofstream& fileStream, struct ListOperation listOperation);


/**
 * @brief Creates a bunch of files containing list operations with one operation on each line with the following
 * format (example):
 *			...
 *			add 0
 *			add 5
 *			remove 5
 *			contains 0
 *			...
 */
int main(int argc, char *argv[])
{
	//char buffer[BUFFER_SIZE];
	std::ofstream sequOrderedDataFile;
	std::ofstream sequBackwardsOrderedDataFile;
	std::ofstream correctnessDataFile;
	std::ofstream randomDataFile_80_10_10;
	std::ofstream randomDataFile_70_20_10;
	std::ofstream randomDataFile_60_20_20;
	std::ofstream randomDataFile_50_35_15;
	std::ofstream randomDataFile_40_30_30;
	std::ofstream frontHeavyDataFile;
	std::ofstream backHeavyDataFile;

	int containsOpCount;
	int addOpCount;
	int removeOpCount;
	struct ListOperation listOperation;


	srand(time(NULL));

	try
	{
		sequOrderedDataFile.open(SEQ_ORDERED_DATA_FILENAME, std::fstream::out);
		sequBackwardsOrderedDataFile.open(SEQ_BACKWARDS_ORDERED_FILENAME, std::fstream::out);
		correctnessDataFile.open(CORRECTNESS_FILENAME, std::fstream::out);
		randomDataFile_80_10_10.open(RANDOM_DATA_FILENAME_80_10_10, std::fstream::out);
		randomDataFile_70_20_10.open(RANDOM_DATA_FILENAME_70_20_10, std::fstream::out);
		randomDataFile_60_20_20.open(RANDOM_DATA_FILENAME_60_20_20, std::fstream::out);
		randomDataFile_50_35_15.open(RANDOM_DATA_FILENAME_50_35_15, std::fstream::out);
		randomDataFile_40_30_30.open(RANDOM_DATA_FILENAME_40_30_30, std::fstream::out);
		frontHeavyDataFile.open(FRONT_HEAVY_DATA_FILENAME, std::fstream::out);
		backHeavyDataFile.open(BACK_HEAVY_DATA_FILENAME, std::fstream::out);
	}
	catch(std::exception ex)
	{
		//do nothing, error handling is done below
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
	if((correctnessDataFile.rdstate() & std::fstream::failbit) != 0)
	{
		std::cout << "Error when opening file: " << CORRECTNESS_FILENAME << std::endl;
		exit(EXIT_FAILURE);
	}
	if((randomDataFile_80_10_10.rdstate() & std::fstream::failbit) != 0)
	{
		std::cout << "Error when opening file: " << RANDOM_DATA_FILENAME_80_10_10 << std::endl;
		exit(EXIT_FAILURE);
	}
	if((randomDataFile_70_20_10.rdstate() & std::fstream::failbit) != 0)
	{
		std::cout << "Error when opening file: " << RANDOM_DATA_FILENAME_70_20_10 << std::endl;
		exit(EXIT_FAILURE);
	}
	if((randomDataFile_60_20_20.rdstate() & std::fstream::failbit) != 0)
	{
		std::cout << "Error when opening file: " << RANDOM_DATA_FILENAME_60_20_20 << std::endl;
		exit(EXIT_FAILURE);
	}
	if((randomDataFile_50_35_15.rdstate() & std::fstream::failbit) != 0)
	{
		std::cout << "Error when opening file: " << RANDOM_DATA_FILENAME_50_35_15 << std::endl;
		exit(EXIT_FAILURE);
	}
	if((randomDataFile_40_30_30.rdstate() & std::fstream::failbit) != 0)
	{
		std::cout << "Error when opening file: " << RANDOM_DATA_FILENAME_40_30_30 << std::endl;
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

	//generate the ascending sorted list data with ADD operations only
	for(int z = 0; z < NUMBER_OF_ELEMENTS; z++)
	{
		listOperation.operation = ADD; listOperation.data = z;
		WriteOp(sequOrderedDataFile, listOperation);
	}
	sequOrderedDataFile.close();

	//generate the descending sorted list data with ADD operations only
	for(int z = NUMBER_OF_ELEMENTS-1; z >= 0; z--)
	{
		listOperation.operation = ADD; listOperation.data = z;
		WriteOp(sequBackwardsOrderedDataFile, listOperation);
	}
	sequBackwardsOrderedDataFile.close();

	//generate the list operations for the parallel correctness test
	for(int z = 0; z < NUMBER_OF_ELEMENTS; z++)
	{
		listOperation.operation = ADD; listOperation.data = z;
		WriteOp(correctnessDataFile, listOperation);
	}
	for(int z = 0; z < NUMBER_OF_ELEMENTS; z++)
	{
		listOperation.operation = REMOVE; listOperation.data = z;
		WriteOp(correctnessDataFile, listOperation);
	}
	correctnessDataFile.close();

	//generate a pseudo random list of data with a random order of operations,
	//consisting of 80% CONTAINS, 10% ADD and 10% REMOVE operations
	containsOpCount = static_cast<int>(NUMBER_OF_ELEMENTS * 0.8);
	addOpCount = static_cast<int>(NUMBER_OF_ELEMENTS * 0.1);
	removeOpCount = static_cast<int>(NUMBER_OF_ELEMENTS * 0.1);
	for(int z = 0; z < NUMBER_OF_ELEMENTS; z++)
	{
		listOperation.operation = SelectOp(containsOpCount, addOpCount, removeOpCount);
		listOperation.data = rand() % NUMBER_OF_ELEMENTS;
		if(WriteOp(randomDataFile_80_10_10, listOperation) == EXIT_FAILURE)
		{
			std::cout << "Error in WriteOp() (most likely listOperation.operation == FAIL)" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	randomDataFile_80_10_10.close();

	//generate a pseudo random list of data with a random order of operations,
	//consisting of 70% CONTAINS, 20% ADD and 10% REMOVE operations
	containsOpCount = static_cast<int>(NUMBER_OF_ELEMENTS * 0.7);
	addOpCount = static_cast<int>(NUMBER_OF_ELEMENTS * 0.2);
	removeOpCount = static_cast<int>(NUMBER_OF_ELEMENTS * 0.1);
	for(int z = 0; z < NUMBER_OF_ELEMENTS; z++)
	{
		listOperation.operation = SelectOp(containsOpCount, addOpCount, removeOpCount);
		listOperation.data = rand() % NUMBER_OF_ELEMENTS;
		if(WriteOp(randomDataFile_70_20_10, listOperation) == EXIT_FAILURE)
		{
			std::cout << "Error in WriteOp() (most likely listOperation.operation == FAIL)" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	randomDataFile_70_20_10.close();

	//generate a pseudo random list of data with a random order of operations,
	//consisting of 60% CONTAINS, 20% ADD and 20% REMOVE operations
	containsOpCount = static_cast<int>(NUMBER_OF_ELEMENTS * 0.6);
	addOpCount = static_cast<int>(NUMBER_OF_ELEMENTS * 0.2);
	removeOpCount = static_cast<int>(NUMBER_OF_ELEMENTS * 0.2);
	for(int z = 0; z < NUMBER_OF_ELEMENTS; z++)
	{
		listOperation.operation = SelectOp(containsOpCount, addOpCount, removeOpCount);
		listOperation.data = rand() % NUMBER_OF_ELEMENTS;
		if(WriteOp(randomDataFile_60_20_20, listOperation) == EXIT_FAILURE)
		{
			std::cout << "Error in WriteOp() (most likely listOperation.operation == FAIL)" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	randomDataFile_60_20_20.close();

	//generate a pseudo random list of data with a random order of operations,
	//consisting of 50% CONTAINS, 35% ADD and 15% REMOVE operations
	containsOpCount = static_cast<int>(NUMBER_OF_ELEMENTS * 0.5);
	addOpCount = static_cast<int>(NUMBER_OF_ELEMENTS * 0.35);
	removeOpCount = static_cast<int>(NUMBER_OF_ELEMENTS * 0.15);
	for(int z = 0; z < NUMBER_OF_ELEMENTS; z++)
	{
		listOperation.operation = SelectOp(containsOpCount, addOpCount, removeOpCount);
		listOperation.data = rand() % NUMBER_OF_ELEMENTS;
		if(WriteOp(randomDataFile_50_35_15, listOperation) == EXIT_FAILURE)
		{
			std::cout << "Error in WriteOp() (most likely listOperation.operation == FAIL)" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	randomDataFile_50_35_15.close();

	//generate a pseudo random list of data with a random order of operations,
	//consisting of 40% CONTAINS, 30% ADD and 30% REMOVE operations
	containsOpCount = static_cast<int>(NUMBER_OF_ELEMENTS * 0.4);
	addOpCount = static_cast<int>(NUMBER_OF_ELEMENTS * 0.3);
	removeOpCount = static_cast<int>(NUMBER_OF_ELEMENTS * 0.3);
	for(int z = 0; z < NUMBER_OF_ELEMENTS; z++)
	{
		listOperation.operation = SelectOp(containsOpCount, addOpCount, removeOpCount);
		listOperation.data = rand() % NUMBER_OF_ELEMENTS;
		if(WriteOp(randomDataFile_40_30_30, listOperation) == EXIT_FAILURE)
		{
			std::cout << "Error in WriteOp() (most likely listOperation.operation == FAIL)" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	randomDataFile_40_30_30.close();

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

/**
 * @brief Randomly selects an operation (add, remove or contains) and decreases the  appropriate count variable
 * @details Once a parameter with a value less or equal then 0 is entered this operation cannot be selected;
 *			if all three parameters are less or equal 0, then as a result the returned value is FAIL
 *			The probability of choosing an operation "xxx" is: xxxOpCount/<sum of all op counts>
 *			(number of favourable outcomes divided by possible outcomes)
 * @return A random operation or FAIL if all parameters are <= 0
 * @param containsOpCount Reference to the (backwards) counter for the contains operations;
 *		  if its value is greater than 0 and a contains operations is selected, then its value is decremented
 * @param addOpCount Reference to the (backwards) counter for the add operations;
 		  analogous to the "containsOpCount" parameter
 * @param removeOpCount Reference to the (backwards) counter for the remove operations;
 		  analogous to the "containsOpCount" parameter
 */
enum Operation SelectOp(int& containsOpCount, int& addOpCount, int& removeOpCount)
{
	int totalRemainingOpCount = containsOpCount + addOpCount + removeOpCount;
	int randomNumber = rand() % totalRemainingOpCount;


	if(containsOpCount <= 0 && addOpCount <= 0 &&removeOpCount <= 0)
		return FAIL;
	else
	{
		if(randomNumber < containsOpCount)
		{
			containsOpCount--;
			return CONTAINS;
		}
		else if(randomNumber >= containsOpCount && randomNumber < containsOpCount + addOpCount)
		{
			addOpCount--;
			return ADD;
		}
		else if(randomNumber >= containsOpCount + addOpCount && randomNumber < totalRemainingOpCount)
		{
			removeOpCount--;
			return REMOVE;
		}
	}

	//control should never reach this part
	return FAIL;
}

/**
 * @brief Writes a list operation "description" to a file
 * @returns EXIT_SUCCESS if the operation was successfully written to the specified file or
 * 			EXIT_FAILURE otherwise
 * @param fileStream File stream to which the operation should be written
 * @param listOperation A structure object containing the list operation information
 */
int WriteOp(std::ofstream& fileStream, struct ListOperation listOperation)
{
	char buffer[BUFFER_SIZE];


	switch(listOperation.operation)
	{
	case CONTAINS:
		strcat(strcpy(buffer, "CONTAINS "), std::to_string(listOperation.data).c_str());
		break;
	case ADD:
		strcat(strcpy(buffer, "ADD "), std::to_string(listOperation.data).c_str());
		break;
	case REMOVE:
		strcat(strcpy(buffer, "REMOVE "), std::to_string(listOperation.data).c_str());
		break;
	case FAIL:
		std::cout << "Error in WriteOp(), listOperation.operation == FAIL" << std::endl;
		return EXIT_FAILURE;
	default:
		std::cout << "Error in WriteOp(), program flow should never reach the default statement" << std::endl;
		return EXIT_FAILURE;
	}

	fileStream.write(buffer, strlen(buffer));
	fileStream.put('\n');

	return EXIT_SUCCESS;
}
