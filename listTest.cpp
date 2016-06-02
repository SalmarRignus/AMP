#include "fineList.h"
#include "optList.h"
#include "lazyList.h"
#include "lFreeList.h"
#include "coarseList.h"

#include <iostream>

int SeqTestList(List *list);
void SeqTestCoarseGrainedList(void);
void SeqTestFineGrainedList(void);
void SeqTestOptimisticSyncList(void);
void SeqTestLazyList(void);
void SeqTestLockFreeList(void);

#define TEST_LIST_LENGTH	10000

int main(int argc, char *argv[])
{
	SeqTestCoarseGrainedList();
	SeqTestFineGrainedList();
	SeqTestOptimisticSyncList();
	SeqTestLazyList();
	SeqTestLockFreeList();

	return EXIT_SUCCESS;
}

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
	return EXIT_SUCCESS;
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
	std::cout << "Start Test of list with fine-grained locks" << std::endl;
	FineGrainedList *fList = new FineGrainedList();
	SeqTestList(fList);
	std::cout << "End Test of list with fine-grained locks" << std::endl;
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
