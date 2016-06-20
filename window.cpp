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

/**********List with optimistic locking**********/
OptList::Window::Window(Node *pred, Node *curr)
{
	this->curr = curr;
	this->pred = pred;
}

void OptList::Window::lock(void)
{
	this->curr->mut.lock();
	this->pred->mut.lock();
}

void OptList::Window::unlock(void)
{
	this->curr->mut.unlock();
	this->pred->mut.unlock();
}

/**********Lazy List**********/
LazyList::Window::Window(Node *pred, Node *curr)
{
	this->curr = curr;
	this->pred = pred;
}

void LazyList::Window::lock(void)
{
	this->curr->mut.lock();
	this->pred->mut.lock();
}

void LazyList::Window::unlock(void)
{
	this->curr->mut.unlock();
	this->pred->mut.unlock();
}

/**********Lock Free List**********/
LockFreeList::Window::Window(Node *pred, Node *curr)
{
	this->curr = curr;
	this->pred = pred;
}
