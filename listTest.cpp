#include "fineList.h"
#include "optList.h"
#include "lazyList.h"
#include "lFreeList.h"
#include "coarseList.h"

#include <iostream>
#include <thread>
#include <unistd.h>

int SeqTestList(List *list);
void SeqTestCoarseGrainedList(void);
void SeqTestFineGrainedList(void);
void SeqTestOptimisticSyncList(void);
void SeqTestLazyList(void);
void SeqTestLockFreeList(void);
int ParTestListIndividual(List *list);

#define TEST_LIST_LENGTH	5000
#define NUMBER_OF_THREADS	2

int main(int argc, char *argv[])
{
	SeqTestCoarseGrainedList();
	SeqTestFineGrainedList();
	SeqTestOptimisticSyncList();
	SeqTestLazyList();
	SeqTestLockFreeList();

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
	
	sleep(10);
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
		std::cout << "Thread #" << std::this_thread::get_id() << " adds " << z << std::endl;
		list->add(z);
	}

	//empty the list
	for(int z = 0; z < TEST_LIST_LENGTH; z++)
	{
		std::cout << "Thread #" << std::this_thread::get_id() << " removes " << z << std::endl;
		list->remove(z);
	}
	
	std::cout << "Thread #" << std::this_thread::get_id() << " finishes" << std::endl;
}

void SeqTestCoarseGrainedList()
{
	std::cout << "Start Test of list with coarse-grained locks" << std::endl;
	//TODO
	std::cout << "End Test of list with coarse-grained locks" << std::endl;
	return;
}
void SeqTestFineGrainedList()
{
	std::cout << "Start Sequential Test of list with fine-grained locks" << std::endl;
	FineGrainedList *fList = new FineGrainedList();
	SeqTestList(fList);
	std::cout << "End Sequential Test of list with fine-grained locks" << std::endl;

	std::cout << "Start Parallel Test of list with fine-grained locks" << std::endl;
	ParTestList(fList);
	std::cout << "End Parallel Test of list with fine-grained locks" << std::endl;
	return;
}
void SeqTestOptimisticSyncList()
{
	//TODO
	return;
}
void SeqTestLazyList()
{
	//TODO
	return;
}
void SeqTestLockFreeList()
{
	//TODO
	return;
}
