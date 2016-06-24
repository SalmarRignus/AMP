#include "fineList.h"
#include "optList.h"
#include "lazyList.h"
#include "lFreeList.h"
#include "coarseList.h"
#include "listOperation.h"

#include <iostream>
#include <fstream>
#include <thread>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sstream>
#include <chrono>
#include <list>


/****************************************Prototypes****************************************/
void PrintUsage(void);
void TestLists(void);
void TestList(List *list);
int SeqTestList(List *list);
int ParTestList(List *list);
int ParTestListIndividual(List *list);


/****************************************Custom Types****************************************/
enum TestMode {FULL, CORRECTNESS, PERFORMANCE};


/****************************************Constants****************************************/
#define BUFFER_SIZE								1024

const char *OUTPUT_FILE_NAME = 					"TestResults.txt";
const char *DEFAULT_FILE_NAME = 				"ListContent.txt";
const unsigned int DEFAULT_NUMBER_OF_THREADS = 	50;
const enum	TestMode DEFAULT_TEST_MODE =		FULL;


/****************************************global variables****************************************/
//after their first definition they should be treated like constants
unsigned int numberOfThreads;
enum TestMode testMode;
char inputFileName[BUFFER_SIZE];
std::ifstream inputFile;
unsigned int numberOfListElements;	//the number of elements from the input file 

/**
 * @brief	Performs a couple of tests considering the correctness and performance of different
 *			concurrent list implementations
 * @details Call: listTest <numberOfThreads> <listFileName> <testMode>
 *		numberOfThreads:	the number of threads that are spawned
 *		listFileName:		the name of the file, which contains the list data to use for the tests
 *		testMode:			can be either string "full", "correctness", "performance"
 *							-) "full" indicates that a complete test should be performed using all implemented tests (correctness and performance)
 *							-) "correctness" indicates that only the correctness tests should be performed
 *							-) "performance" indicates that only the performance tests should be performed
 */
int main(int argc, char *argv[])
{
	char buffer[BUFFER_SIZE];
	std::list<unsigned int> inputList;


	//parse the command line
	switch(argc)
	{
	case 1:
		numberOfThreads = 	DEFAULT_NUMBER_OF_THREADS;
		strcpy(inputFileName, DEFAULT_FILE_NAME);
		testMode = 			DEFAULT_TEST_MODE;
		break;
	case 4:
		try
		{
			numberOfThreads = 	std::stoul(std::string(argv[1]), NULL, 10);
		}
		catch(std::invalid_argument ex)
		{
			PrintUsage();
			std::cout << "Invalid argument for numberOfThreads" << std::endl;
			return EXIT_FAILURE;
		}
		strncpy(inputFileName, argv[2], BUFFER_SIZE);
		inputFileName[BUFFER_SIZE-1] = '\0';
		if(strcmp(argv[3], "full") == 0)
			testMode = FULL;
		else if(strcmp(argv[3], "correctness") == 0)
			testMode = CORRECTNESS;
		else if(strcmp(argv[3], "performance") == 0)
			testMode = PERFORMANCE;
		else
		{
			PrintUsage();
			std::cout << "Invalid argument for <testMode>" << std::endl;
			return EXIT_FAILURE;
		}
		break;
	default:
		PrintUsage();
		return EXIT_FAILURE;
	}

	//open the input file
	strcat(strcpy(buffer, "./"), inputFileName);
	try
	{
		inputFile.open(buffer, std::fstream::in);
	}
	catch(std::exception ex)
	{
		//ignore the exception, error handling regarding failure to open file follows below
	}
	if((inputFile.rdstate() & std::fstream::failbit) != 0)
	{
		std::cout << "Error when opening file: " << inputFileName << std::endl;
		exit(EXIT_FAILURE);
	}

	//write the file contents into a list
	while(inputFile.eof())
	{
		inputFile.getline(buffer, BUFFER_SIZE-1, '\n');
		try
		{
			inputList.push_back(std::stoul(std::string(buffer), NULL, 10));
		}
		catch(std::invalid_argument ex)
		{
			PrintUsage();
			std::cout << "Invalid element in input file " << inputFileName << ":" << buffer << std::endl;
			return EXIT_FAILURE;
		}
	}

	inputFile.close();

	numberOfListElements = inputList.size();

	TestLists();

	return EXIT_SUCCESS;
}

/**
 * @brief Creates list objects of different implementations and calls the TestList method for every one of them
 */
void TestLists(void)
{
	CoarseGrainedList	*cList =	new CoarseGrainedList();
	FineGrainedList		*fList =	new FineGrainedList();
	OptList				*oList =	new OptList();
	LazyList			*lList =	new LazyList();
	LockFreeList		*lfList =	new LockFreeList();

	TestList(cList);
	TestList(fList);
	TestList(oList);
	TestList(lList);
	TestList(lfList);

	delete cList;
	delete fList;
	delete oList;
	delete lList;
	delete lfList;

	return;
}

/**
 * @brief Calls the actual test procedures for the list
 * @param list Pointer to a list object that will be tested
 */
void TestList(List *list)
{
	const char *startMessageSeqCorr = NULL, *endMessageSeqCorr = NULL;
	const char *startMessageParCorr = NULL, *endMessageParCorr = NULL;
	const char *startMessageParPerf = NULL, *endMessageParPerf = NULL;
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

	//set the messages according to the list type
	if(typeid(*list) == typeid(CoarseGrainedList))
	{
		startMessageSeqCorr = "Start Sequential Correctness Test of list with coarse-grained locks\n";
		endMessageSeqCorr = "End Sequential Correctness Test of list with coarse-grained locks\n";
		startMessageParCorr = "Start Parallel Correctness Test of list with coarse-grained locks\n";
		endMessageParCorr = "End Parallel Correctness Test of list with coarse-grained locks\n";
		startMessageParPerf = "Start Parallel Performance Test of list with coarse-grained locks\n";
		endMessageParPerf = "End Parallel Performance Test of list with coarse-grained locks\n";
	}
	if(typeid(*list) == typeid(FineGrainedList))
	{
		startMessageSeqCorr = "Start Sequential Correctness Test of list with fine-grained locks\n";
		endMessageSeqCorr = "End Sequential Correctness Test of list with fine-grained locks\n";
		startMessageParCorr = "Start Parallel Correctness Test of list with fine-grained locks\n";
		endMessageParCorr = "End Parallel Correctness Test of list with fine-grained locks\n";
		startMessageParPerf = "Start Parallel Performance Test of list with fine-grained locks\n";
		endMessageParPerf = "End Parallel Performance Test of list with fine-grained locks\n";
	}
	if(typeid(*list) == typeid(OptList))
	{
		startMessageSeqCorr = "Start Sequential Correctness Test of list with optimistic locking\n";
		endMessageSeqCorr = "End Sequential Correctness Test of list with optimistic locking\n";
		startMessageParCorr = "Start Parallel Correctness Test of list with optimistic locking\n";
		endMessageParCorr = "End Parallel Correctness Test of list with optimistic locking\n";
		startMessageParPerf = "Start Parallel Performance Test of list with optimistic locking\n";
		endMessageParPerf = "End Parallel Performance Test of list with optimistic locking\n";
	}
	if(typeid(*list) == typeid(LazyList))
	{
		startMessageSeqCorr = "Start Sequential Correctness Test of lazy list\n";
		endMessageSeqCorr = "End Sequential Correctness Test of lazy list\n";
		startMessageParCorr = "Start Parallel Correctness Test of lazy list\n";
		endMessageParCorr = "End Parallel Correctness Test of lazy list\n";
		startMessageParPerf = "Start Parallel Performance Test of lazy list\n";
		endMessageParPerf = "End Parallel Performance Test of lazy list\n";
	}
	if(typeid(*list) == typeid(LockFreeList))
	{
		startMessageSeqCorr = "Start Sequential Correctness Test of lock-free list\n";
		endMessageSeqCorr = "End Sequential Correctness Test of lock-free list\n";
		startMessageParCorr = "Start Parallel Correctness Test of lock-free list\n";
		endMessageParCorr = "End Parallel Correctness Test of lock-free list\n";
		startMessageParPerf = "Start Parallel Performance Test of lock-free list\n";
		endMessageParPerf = "End Parallel Performance Test of lock-free list\n";
	}

	if(testMode == FULL || testMode == CORRECTNESS)
	{
		//sequential correctness test
		std::cout << startMessageSeqCorr;
		start = std::chrono::high_resolution_clock::now();
		SeqTestList(list);
		end = std::chrono::high_resolution_clock::now();
		std::cout << endMessageSeqCorr;
		std::cout << "File used: " << inputFileName << std::endl;
		std::cout << "Required time: " << std::chrono::duration<double, std::milli>(end-start).count() << "ms" << std::endl << std::endl;

		//parallel correctness test
		std::cout << startMessageParCorr;
		start = std::chrono::high_resolution_clock::now();
		ParTestList(list);
		end = std::chrono::high_resolution_clock::now();
		std::cout << endMessageParCorr;
		std::cout << "File used: " << inputFileName << std::endl;
		std::cout << "Required time: " << std::chrono::duration<double, std::milli>(end-start).count() << "ms" << std::endl << std::endl;
	}

	if(testMode == FULL || testMode == PERFORMANCE)
	{
		/************************************
		std::cout << startMessageParPerf;
		TODO add extensive performance tests
		std::cout << endMessageParPerf;
		************************************/
	}
}

/**
 * @brief A sequential correctness test
 */
int SeqTestList(List *list)
{
	//fill the list from front to back
	for(int z = numberOfListElements - 1; z >= 0; z--)
	{
		list->add(z);
	}
	//check whether it really contains all the added elements
	for(int z = 0; z < numberOfListElements; z++)
	{
		if(!list->contains(z))
		{
			std::cout << "Error list.contains(z) returns:" << list->contains(z) << "for z: " << z << std::endl;
			return EXIT_FAILURE;
		}
	}
	//remove all the elements in the list
	for(int z = 0; z < numberOfListElements; z++)
	{
		list->remove(z);
	}

	if(list->isEmpty())
		return EXIT_SUCCESS;
	else
	{
		std::cout << "Error, the list should be empty" << std::endl;
		return EXIT_FAILURE;
	}
}

/**
 * @brief A parallel correctness test
 */
int ParTestList(List *list)
{
	std::thread *threads[numberOfThreads];

	for(int z = 0; z < numberOfThreads; z++)
	{
		threads[z] = new std::thread(ParTestListIndividual, list);
	}
	
	sleep(0);
	//wait for all the threads until they have finished
	for(int z = 0; z < numberOfThreads; z++)
	{
		threads[z]->join();
	}

	//check whether the list is empty
	if(list->isEmpty())
		return EXIT_SUCCESS;
	else
	{
		std::cout << "Error, the list should be empty" << std::endl;
		return EXIT_FAILURE;
	}
}

int ParTestListIndividual(List *list)
{
	//should prevent this thread from finishing too fast, since we want as much contention as possible
	sleep(0);
	std::cout << std::dec;
	
	//fill the list with values
	for(int z = 0; z < numberOfListElements; z++)
	{
		list->add(z);
	}

	//empty the list
	for(int z = 0; z < numberOfListElements; z++)
	{
		list->remove(z);
	}
	//writing to cout with one string (in one go) prohibites scrambling of messages
	std::string message;
	std::stringstream ss;
	ss << std::this_thread::get_id();
	message.append("Thread #");
	message.append(ss.str());
	message.append(" finishes\n");
	std::cout << message;
}

void PrintUsage(void)
{
	std::cout << "usage: listTest <numberOfThreads> <listFileName> <testMode>" << std::endl
	<< "or alternatively: listTest" << std::endl << "(uses default values)" << std::endl;
}
