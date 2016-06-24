#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "listOperation.h"


#define BUFFER_SIZE 		1024
#define NUMBER_OF_ELEMENTS	50000
#define MAX_WRITE			20


const char *SEQ_ORDERED_DATA_FILENAME = "ListContentCorrSort.txt";
const char *SEQ_BACKWARDS_ORDERED_FILENAME = "ListContentBackSort.txt";
const char *RANDOM_DATA_FILENAME = "ListContentRandom.txt";
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

	//generate the ascending sorted list data with ADD operations only
	for(int z = 0; z < NUMBER_OF_ELEMENTS; z++)
	{
		sequOrderedDataFile.write("ADD ", strlen("ADD "));
		strcpy(buffer, std::to_string(z).c_str());
		sequOrderedDataFile.write(buffer, strlen(buffer));
		sequOrderedDataFile.put('\n');
	}
	sequOrderedDataFile.close();

	//generate the descending sorted list data with ADD operations only
	for(int z = NUMBER_OF_ELEMENTS-1; z >= 0; z--)
	{
		sequBackwardsOrderedDataFile.write("ADD ", strlen("ADD "));
		strcpy(buffer, std::to_string(z).c_str());
		sequBackwardsOrderedDataFile.write(buffer, strlen(buffer));
		sequBackwardsOrderedDataFile.put('\n');
	}
	sequBackwardsOrderedDataFile.close();

	//generate a pseudo random list of data with a random order of operations,
	//consisting of 80% CONTAINS, 10% ADD and 10% REMOVE operations
	int containsOpCount = static_cast<int>(NUMBER_OF_ELEMENTS * 0.8);
	int addOpCount = static_cast<int>(NUMBER_OF_ELEMENTS * 0.1);
	int removeOpCount = static_cast<int>(NUMBER_OF_ELEMENTS * 0.1);
	struct ListOperation listOperation;
	srand(time(NULL));
	for(int z = 0; z < NUMBER_OF_ELEMENTS; z++)
	{
		listOperation.operation = SelectOp(containsOpCount, addOpCount, removeOpCount);
		listOperation.data = rand() % NUMBER_OF_ELEMENTS;
		if(WriteOp(randomDataFile, listOperation) == EXIT_FAILURE)
		{
			std::cout << "Error in WriteOp() (most likely listOperation.operation == FAIL)" << std::endl;
			exit(EXIT_FAILURE);
		}
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

/**
 * @brief Randomly selects an operation (add, remove or contains) and decreases the  appropriate count variable
 * @details Once a parameter with a value less or equal then 0 is entered this operation cannot be selected;
 *			if all three parameters are less or equal 0, then as a result the returned value is FAIL
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
	int randomNumber = rand();

	//if all operations are available
	if(containsOpCount > 0 && addOpCount > 0 && removeOpCount > 0)
	{
		//select contains
		if((randomNumber % 3) == 0)
		{
			containsOpCount--;
			return CONTAINS;
		}
		//select add
		if((randomNumber % 3) == 1)
		{
			addOpCount--;
			return ADD;
		}
		//select remove
		if((randomNumber % 3) == 2)
		{
			removeOpCount--;
			return REMOVE;
		}
	}
	//if the contains operation is not available
	else if(containsOpCount <= 0 && addOpCount > 0 && removeOpCount > 0)
	{
		//select add
		if((randomNumber % 2) == 0)
		{
			addOpCount--;
			return ADD;
		}
		//select remove
		if((randomNumber % 2) == 1)
		{
			removeOpCount--;
			return REMOVE;
		}
	}
	//if the add operation is not available
	else if(containsOpCount > 0 && addOpCount <= 0 && removeOpCount > 0)
	{
		//select contains
		if((randomNumber % 2) == 0)
		{
			containsOpCount--;
			return CONTAINS;
		}
		//select remove
		if((randomNumber % 2) == 1)
		{
			removeOpCount--;
			return REMOVE;
		}
	}
	//if the remove operation is not available
	else if(containsOpCount > 0 && addOpCount > 0 && removeOpCount <= 0)
	{
		//select contains
		if((randomNumber % 2) == 0)
		{
			containsOpCount--;
			return CONTAINS;
		}
		//select add
		if((randomNumber % 2) == 1)
		{
			addOpCount--;
			return ADD;
		}
	}
	//if the contains and add operation are not available
	else if(containsOpCount <= 0 && addOpCount <= 0 && removeOpCount > 0)
	{
		//select remove
		removeOpCount--;
		return REMOVE;
	}
	//if the contains and remove operation are not available
	else if(containsOpCount <= 0 && addOpCount > 0 && removeOpCount <= 0)
	{
		//select add
		addOpCount--;
		return ADD;
	}
	//if the add and remove operation arare not available
	else if(containsOpCount > 0 && addOpCount <= 0 && removeOpCount <= 0)
	{
		//select contains
		containsOpCount--;
		return CONTAINS;
	}
	//if no operation is available
	else
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
