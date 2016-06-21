#include "fineList.h"
#include "optList.h"
#include "lazyList.h"
#include "lFreeList.h"
#include "coarseList.h"

#include <iostream>
#include <thread>
#include <unistd.h>
#include <string>
#include <sstream>
#include <chrono>

int SeqTestList(List *list);
void TestCoarseGrainedList(void);
void TestFineGrainedList(void);
void TestOptimisticSyncList(void);
void TestLazyList(void);
void TestLockFreeList(void);
int ParTestListIndividual(List *list);

#define TEST_LIST_LENGTH	5000
#define NUMBER_OF_THREADS	4

int main(int argc, char *argv[])
{
	TestCoarseGrainedList();
	TestFineGrainedList();
	TestOptimisticSyncList();
	TestLazyList();
	TestLockFreeList();

	return EXIT_SUCCESS;
}

/**
 * @brief A sequential correctness test
 */
int SeqTestList(List *list)
{
	//fill the list from front to back
	for(int z = TEST_LIST_LENGTH - 1; z >= 0; z--)
	{
		list->add(z);
	}
	//check whether it really contains all the added elements
	for(int z = 0; z < TEST_LIST_LENGTH; z++)
	{
		if(!list->contains(z))
		{
			std::cout << "Error list.contains(z) returns:" << list->contains(z) << "for z: " << z << std::endl;
			return EXIT_FAILURE;
		}
	}
	//remove all the elements in the list
	for(int z = 0; z < TEST_LIST_LENGTH; z++)
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
	std::thread *threads[NUMBER_OF_THREADS];

	for(int z = 0; z < NUMBER_OF_THREADS; z++)
	{
		threads[z] = new std::thread(ParTestListIndividual, list);
	}
	
	sleep(0);
	//wait for all the threads until they have finished
	for(int z = 0; z < NUMBER_OF_THREADS; z++)
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
	for(int z = 0; z < TEST_LIST_LENGTH; z++)
	{
		list->add(z);
	}

	//empty the list
	for(int z = 0; z < TEST_LIST_LENGTH; z++)
	{
		list->remove(z);
	}
	//writing to cout with one string prohibites scrambling of messages
	std::string message;
	std::stringstream ss;
	ss << std::this_thread::get_id();
	message.append("Thread #");
	message.append(ss.str());
	message.append(" finishes\n");
	std::cout << message;
}

void TestCoarseGrainedList()
{
	std::cout << "Start Sequential Test of list with coarse-grained locks" << std::endl;
	CoarseGrainedList *cList = new CoarseGrainedList();
	SeqTestList(cList);
	std::cout << "End Sequential Test of list with coarse-grained locks" << std::endl;

	std::cout << "Start Parallel Test of list with coarse-grained locks" << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	ParTestList(cList);
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "End Parallel Test of list with coarse-grained locks" << std::endl;
	std::cout << "Required time: " << std::chrono::duration<double, std::milli>(end-start).count() << "ms" << std::endl << std::endl;

	delete cList;

	return;
}
void TestFineGrainedList()
{
	std::cout << "Start Sequential Test of list with fine-grained locks" << std::endl;
	FineGrainedList *fList = new FineGrainedList();
	SeqTestList(fList);
	std::cout << "End Sequential Test of list with fine-grained locks" << std::endl;

	std::cout << "Start Parallel Test of list with fine-grained locks" << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	ParTestList(fList);
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "End Parallel Test of list with fine-grained locks" << std::endl;
	std::cout << "Required time: " << std::chrono::duration<double, std::milli>(end-start).count() << "ms" << std::endl << std::endl;

	delete fList;

	return;
}
void TestOptimisticSyncList()
{
	std::cout << "Start Sequential Test of list with optimistic synchronization" << std::endl;
	OptList *oList = new OptList();
	SeqTestList(oList);
	std::cout << "End Sequential Test of list with optimistic synchronization" << std::endl;

	std::cout << "Start Parallel Test of list with optimistic synchronization" << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	ParTestList(oList);
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "End Parallel Test of list with optimistic synchronization" << std::endl;
	std::cout << "Required time: " << std::chrono::duration<double, std::milli>(end-start).count() << "ms" << std::endl << std::endl;

	delete oList;

	return;
}
void TestLazyList()
{
	std::cout << "Start Sequential Test of lazy list" << std::endl;
	LazyList *lList = new LazyList();
	SeqTestList(lList);
	std::cout << "End Sequential Test of lazy list" << std::endl;

	std::cout << "Start Parallel Test of lazy list" << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	ParTestList(lList);
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "End Parallel Test of lazy list" << std::endl;
	std::cout << "Required time: " << std::chrono::duration<double, std::milli>(end-start).count() << "ms" << std::endl << std::endl;

	delete lList;

	return;
}
void TestLockFreeList()
{
	std::cout << "Start Sequential Test of lock free list" << std::endl;
	LockFreeList *lFList = new LockFreeList();
	SeqTestList(lFList);
	std::cout << "End Sequential Test of lock free list" << std::endl;

	std::cout << "Start Parallel Test of lock free list" << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	ParTestList(lFList);
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "End Parallel Test of lock free list" << std::endl;
	std::cout << "Required time: " << std::chrono::duration<double, std::milli>(end-start).count() << "ms" << std::endl << std::endl;

	delete lFList;

	return;
}
