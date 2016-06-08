#include "fineList.h"
#include "coarseList.h"
#include "optList.h"
#include "lazyList.h"
#include "lFreeList.h"

//Window::Window(void)
//{
//	this->curr = NULL;
//	this->pred = NULL;
//}

/**********Coarse-grained List**********/
CoarseGrainedList::Window::Window(Node *pred, Node *curr)
{
	this->curr = curr;
	this->pred = pred;
}


/**********Fined-grained List**********/
FineGrainedList::Window::Window(Node *pred, Node *curr)
{
	this->curr = curr;
	this->pred = pred;
}

void FineGrainedList::Window::unlock(void)
{
	this->curr->mut.unlock();
	this->pred->mut.unlock();
}

