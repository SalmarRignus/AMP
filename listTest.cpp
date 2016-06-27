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


/****************************************Custom Types****************************************/
enum TestMode {FULL, CORRECTNESS, PERFORMANCE};
enum ListType {COARSE, FINE, OPT, LAZY, FREE};
struct TestResult
{
	enum ListType listType;
	char *fileName;
	double duration;
	unsigned int numberOfListElements;
	unsigned int numberOfThreads;
};


/****************************************Prototypes****************************************/
void PrintUsage(void);
TestResult *TestLists(std::list<ListOperation> inputList);
TestResult TestList(List *list, std::list<ListOperation> inputList);
int SeqCorrTestList(List *list); 		//predefined correctness test
std::chrono::duration<double, std::milli> ParTestList(List *list, std::list<ListOperation> inputList);
void ParPerfTestListIndividual(List *list, std::list<ListOperation> inputList);
void ParCorrTestListIndividual(List *list);
void WriteTestResult(TestResult result);


/****************************************Constants****************************************/
#define BUFFER_SIZE								1024
#define WAIT_DURATION							1000		//in milliseconds
#define NUMBER_OF_LIST_IMPLEMENTATIONS			5

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

unsigned int numberOfListElementsCorr = 10000;	//for the predefined correctness tests, which do not depend on any input data

std::mutex canStart;

/**
 * @brief	Performs a couple of tests considering the correctness and performance of different
 *			concurrent list implementations
 * @details Call: listTest <numberOfThreads> <listFileName> <testMode>
 *		numberOfThreads:	the number of threads that are spawned
 *		listFileName:		the name of the file, which contains the list data to use for the tests
 *		testMode:			can be either string "full", "correctness", "performance" - don't use full
 *							(may lead to erroneuos error message for the performance test)
 *							-) "full" indicates that a complete test should be performed using all implemented tests (correctness and performance)
 *							-) "correctness" indicates that only the correctness tests should be performed
 *							-) "performance" indicates that only the performance tests should be performed
 */
int main(int argc, char *argv[])
{
	char buffer[BUFFER_SIZE];
	std::list<ListOperation> inputList;


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
	enum Operation op;
	while(!inputFile.eof())
	{
		//read the operation
		inputFile.getline(buffer, BUFFER_SIZE-1, ' ');
		if(strcmp(buffer, "CONTAINS") == 0)
			op = CONTAINS;
		else if(strcmp(buffer, "ADD") == 0)
			op = ADD;
		else if(strcmp(buffer, "REMOVE") == 0)
			op = REMOVE;
		else
		{
			//in case there is a trailing newline character at the end of the file
			if(inputFile.eof())
				break;
			std::cout << "Error, when reading input file: " << inputFileName << std::endl << "invalid operation: " << buffer << std::endl;
			exit(EXIT_FAILURE);
		}
		try
		{
			//read a possible value to perform the operation on
			inputFile.getline(buffer, BUFFER_SIZE-1, '\n');
			inputList.push_back(ListOperation(op, std::stoul(std::string(buffer), NULL, 10)));
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

	TestResult *result;
	result = TestLists(inputList);

	if(testMode == PERFORMANCE || testMode == FULL)
	{
		for(int z = 0; z < NUMBER_OF_LIST_IMPLEMENTATIONS; z++)
		{
			WriteTestResult(result[z]);
		}
	}

	return EXIT_SUCCESS;
}

/**
 * @brief Creates list objects of different implementations and calls the TestList method for every one of them
 * @returns A dynamically allocated array of objects of type TestResult
 */
TestResult *TestLists(std::list<ListOperation> inputList)
{
	CoarseGrainedList	*cList =	new CoarseGrainedList();
	FineGrainedList		*fList =	new FineGrainedList();
	OptList				*oList =	new OptList();
	LazyList			*lList =	new LazyList();
	LockFreeList		*lfList =	new LockFreeList();

	TestResult *result = new TestResult[5];

	result[0] = TestList(cList, inputList);
	result[1] = TestList(fList, inputList);
	result[2] = TestList(oList, inputList);
	result[3] = TestList(lList, inputList);
	result[4] = TestList(lfList, inputList);

	//delete cList;
	//delete fList;
	//delete oList;
	//delete lList;
	//delete lfList;

	return result;
}

/**
 * @brief Calls the actual test procedures for the list
 * @returns An object of type TestResult
 * @param list Pointer to a list object that will be tested
 * @param inputList A list (conventional C++ list) filled with the list operations to be used
 *		  in the performance test
 */
TestResult TestList(List *list, std::list<ListOperation> inputList)
{
	const char *startMessageSeqCorr = NULL, *endMessageSeqCorr = NULL;
	const char *startMessageParCorr = NULL, *endMessageParCorr = NULL;
	const char *startMessageParPerf = NULL, *endMessageParPerf = NULL;
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	TestResult result;

	//set the messages according to the list type
	if(typeid(*list) == typeid(CoarseGrainedList))
	{
		startMessageSeqCorr = "Start Sequential Correctness Test of list with coarse-grained locks\n";
		endMessageSeqCorr = "End Sequential Correctness Test of list with coarse-grained locks\n";
		startMessageParCorr = "Start Parallel Correctness Test of list with coarse-grained locks\n";
		endMessageParCorr = "End Parallel Correctness Test of list with coarse-grained locks\n";
		startMessageParPerf = "Start Parallel Performance Test of list with coarse-grained locks\n";
		endMessageParPerf = "End Parallel Performance Test of list with coarse-grained locks\n";
		result.listType = COARSE;
	}
	if(typeid(*list) == typeid(FineGrainedList))
	{
		startMessageSeqCorr = "Start Sequential Correctness Test of list with fine-grained locks\n";
		endMessageSeqCorr = "End Sequential Correctness Test of list with fine-grained locks\n";
		startMessageParCorr = "Start Parallel Correctness Test of list with fine-grained locks\n";
		endMessageParCorr = "End Parallel Correctness Test of list with fine-grained locks\n";
		startMessageParPerf = "Start Parallel Performance Test of list with fine-grained locks\n";
		endMessageParPerf = "End Parallel Performance Test of list with fine-grained locks\n";
		result.listType = FINE;
	}
	if(typeid(*list) == typeid(OptList))
	{
		startMessageSeqCorr = "Start Sequential Correctness Test of list with optimistic locking\n";
		endMessageSeqCorr = "End Sequential Correctness Test of list with optimistic locking\n";
		startMessageParCorr = "Start Parallel Correctness Test of list with optimistic locking\n";
		endMessageParCorr = "End Parallel Correctness Test of list with optimistic locking\n";
		startMessageParPerf = "Start Parallel Performance Test of list with optimistic locking\n";
		endMessageParPerf = "End Parallel Performance Test of list with optimistic locking\n";
		result.listType = OPT;
	}
	if(typeid(*list) == typeid(LazyList))
	{
		startMessageSeqCorr = "Start Sequential Correctness Test of lazy list\n";
		endMessageSeqCorr = "End Sequential Correctness Test of lazy list\n";
		startMessageParCorr = "Start Parallel Correctness Test of lazy list\n";
		endMessageParCorr = "End Parallel Correctness Test of lazy list\n";
		startMessageParPerf = "Start Parallel Performance Test of lazy list\n";
		endMessageParPerf = "End Parallel Performance Test of lazy list\n";
		result.listType = LAZY;
	}
	if(typeid(*list) == typeid(LockFreeList))
	{
		startMessageSeqCorr = "Start Sequential Correctness Test of lock-free list\n";
		endMessageSeqCorr = "End Sequential Correctness Test of lock-free list\n";
		startMessageParCorr = "Start Parallel Correctness Test of lock-free list\n";
		endMessageParCorr = "End Parallel Correctness Test of lock-free list\n";
		startMessageParPerf = "Start Parallel Performance Test of lock-free list\n";
		endMessageParPerf = "End Parallel Performance Test of lock-free list\n";
		result.listType = FREE;
	}

	if(testMode == FULL || testMode == CORRECTNESS)
	{
		//sequential correctness test
		std::cout << startMessageSeqCorr;
		SeqCorrTestList(list);
		std::cout << endMessageSeqCorr;
		std::cout << "File used: " << inputFileName << std::endl << std::endl;

		//parallel correctness test
		std::cout << startMessageParCorr;
		ParTestList(list, inputList);
		std::cout << endMessageParCorr;
		std::cout << "File used: " << inputFileName << std::endl << std::endl;
	}

	if(testMode == FULL || testMode == PERFORMANCE)
	{
		std::cout << startMessageParPerf;
		std::chrono::duration<double, std::milli> duration = ParTestList(list, inputList);
		std::cout << endMessageParPerf;
		std::cout << "File used: " << inputFileName << std::endl;
		std::cout << "Required time: " << duration.count() << "ms" << std::endl << std::endl;
		result.fileName = inputFileName;
		result.duration = duration.count();
		result.numberOfListElements = numberOfListElements;
		result.numberOfThreads = numberOfThreads;
	}

	return result;
}

/**
 * @brief A sequential correctness test
 */
int SeqCorrTestList(List *list)
{
	//fill the list from front to back
	for(int z = numberOfListElementsCorr - 1; z >= 0; z--)
	{
		list->add(z);
	}
	//check whether it really contains all the added elements
	for(int z = 0; static_cast<unsigned int>(z) < numberOfListElementsCorr; z++)
	{
		if(!list->contains(z))
		{
			std::cout << "Error list.contains(z) returns:" << list->contains(z) << "for z: " << z << std::endl;
			return EXIT_FAILURE;
		}
	}
	//remove all the elements in the list
	for(int z = 0; static_cast<unsigned int>(z) < numberOfListElementsCorr; z++)
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
 * @brief A parallel test of the list implementations
 * @detail Generates threads, which perform certain tests
 */
std::chrono::duration<double, std::milli> ParTestList(List *list, std::list<ListOperation> inputList)
{
	std::thread *threads[numberOfThreads];
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

	//this lock prevents the called threads from starting
	canStart.lock();
	if(testMode == PERFORMANCE || testMode == FULL)
	{
		for(int z = 0; static_cast<unsigned int>(z) < numberOfThreads; z++)
		{
			threads[z] = new std::thread(ParPerfTestListIndividual, list, std::list<ListOperation>(inputList));
		}
	}
	if(testMode == CORRECTNESS || testMode == FULL)
	{
		for(int z = 0; static_cast<unsigned int>(z) < numberOfThreads; z++)
		{
			threads[z] = new std::thread(ParCorrTestListIndividual, list);
		}
	}
	//order of starting the time measurement and the unlocking operation may affect the result
	start = std::chrono::high_resolution_clock::now();
	//unlocking unleashes the individual threads
	canStart.unlock();
	
	//wait for all the threads until they have finished
	for(int z = 0; static_cast<unsigned int>(z) < numberOfThreads; z++)
	{
		threads[z]->join();
	}
	end = std::chrono::high_resolution_clock::now();

	if(testMode == CORRECTNESS)
	{
		//check whether the list is empty
		if(list->isEmpty())
			//for a correctness test always return a duration of 0
			return std::chrono::duration<double, std::milli>(0);
		else
		{
			std::cout << "Error, the list should be empty" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	return std::chrono::duration<double, std::milli>(end-start);
}

void ParPerfTestListIndividual(List *list, std::list<ListOperation> inputList)
{
	//std::this_thread::sleep_until(start);
	//thread can only start after the main thread has unlocked this lock
	canStart.lock();
	canStart.unlock();

	for(std::list<ListOperation>::iterator it = inputList.begin(); !inputList.empty() && it != inputList.end(); it++)
	{
		switch(it->operation)
		{
		case CONTAINS:
			list->contains(it->data);
			break;
		case ADD:
			list->add(it->data);
			break;
		case REMOVE:
			list->remove(it->data);
			break;
		case FAIL:
			std::cout << "Error, invalid operation" << std::endl;
			exit(EXIT_FAILURE);
		default:
			std::cout << "Error, program flow should never reach this" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

/**
 * @brief A parallel correctness test
 */
void ParCorrTestListIndividual(List *list)
{
	//add elements to the list
	for(int z = 0; static_cast<unsigned int>(z) < numberOfListElementsCorr; z++)
	{
		list->add(z);
	}
	//remove them again from the list
	for(int z = static_cast<int>(numberOfListElementsCorr - 1); z >= 0; z--)
	{
		list->remove(z);
	}

	//writing to cout with one string (in one go) prohibites scrambling of messages
	//the output should not happen during performance testing, as it would only waste time
	std::cout << std::dec;
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

/**
 * @brief Writes the results of a test run to a file
 * @details It uses the following format:
 *			 <type> <filename> <duration in milliseconds> <number of list elements> <number of threads>
 *			type: either "coarse", "fine", "opt", "lazy", "free"
 *			filename: the name of the input file
 *			duration: the duration of the test run in milliseconds
 */
void WriteTestResult(TestResult result)
{
	std::ofstream outputFile;
	char buffer[BUFFER_SIZE];

	strcat(strcpy(buffer, "./"), OUTPUT_FILE_NAME);
	try
	{
		outputFile.open(buffer, std::fstream::app);
	}
	catch(std::exception ex)
	{
		//ignore the exception, error handling regarding failure to open file follows below
	}
	if((outputFile.rdstate() & std::fstream::failbit) != 0)
	{
		std::cout << "Error when opening file: " << OUTPUT_FILE_NAME << std::endl;
		exit(EXIT_FAILURE);
	}

	//construct the character string to write to the file
	switch(result.listType)
	{
	case COARSE:
		strcpy(buffer, "coarse ");
		break;
	case FINE:
		strcpy(buffer, "fine ");
		break;
	case OPT:
		strcpy(buffer, "opt ");
		break;
	case LAZY:
		strcpy(buffer, "lazy ");
		break;
	case FREE:
		strcpy(buffer, "free ");
		break;
	default:
		std::cout << "Error, program flow should never enter this section" << std::endl;
		exit(EXIT_FAILURE);
	}

	strcat(strcat(strcat(buffer, result.fileName), " "), std::to_string(result.duration).c_str());
	strcat(strcat(buffer, " "), std::to_string(result.numberOfListElements).c_str());
	strcat(strcat(buffer, " "), std::to_string(result.numberOfThreads).c_str());
	strcat(buffer, "\n");
	outputFile.write(buffer, strlen(buffer));
	outputFile.close();
}
