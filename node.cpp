#include "fineList.h"
#include "coarseList.h"
#include "optList.h"
#include "lazyList.h"
#include "lFreeList.h"

/**********Fine-grained List**********/
/**
 @brief Constructors for the Node class
 */
FineGrainedList::Node::Node(void)
{
	this->item = 0;
	this->next = NULL;
}
FineGrainedList::Node::Node(int item)
{
	this->item = item;
	this->next = NULL;
}
FineGrainedList::Node::Node(int item, FineGrainedList::Node *next)
{
	this->item = item;
	this->next = next;
}
int FineGrainedList::Node::getItem(void)
{
	return this->item;
}
void FineGrainedList::Node::setItem(int item)
{
	this->item = item;
}
FineGrainedList::Node *FineGrainedList::Node::getNext(void)
{
	return this->next;
}
void FineGrainedList::Node::setNext(FineGrainedList::Node *next)
{
	this->next = next;
}

/**********Coarse-grained List**********/
CoarseGrainedList::Node::Node(void)
{
	this->item = 0;
	this->next = NULL;
}
CoarseGrainedList::Node::Node(int item)
{
	this->item = item;
	this->next = NULL;
}
CoarseGrainedList::Node::Node(int item, CoarseGrainedList::Node *next)
{
	this->item = item;
	this->next = next;
}
int CoarseGrainedList::Node::getItem(void)
{
	return this->item;
}
void CoarseGrainedList::Node::setItem(int item)
{
	this->item = item;
}
CoarseGrainedList::Node *CoarseGrainedList::Node::getNext(void)
{
	return this->next;
}
void CoarseGrainedList::Node::setNext(CoarseGrainedList::Node *next)
{
	this->next = next;
}

/**********List with optimistic locking**********/
OptList::Node::Node(void)
{
	this->item = 0;
	this->next = NULL;
}
OptList::Node::Node(int item)
{
	this->item = item;
	this->next = NULL;
}
OptList::Node::Node(int item, OptList::Node *next)
{
	this->item = item;
	this->next = next;
}
int OptList::Node::getItem(void)
{
	return this->item;
}
void OptList::Node::setItem(int item)
{
	this->item = item;
}
OptList::Node *OptList::Node::getNext(void)
{
	return this->next;
}
void OptList::Node::setNext(OptList::Node *next)
{
	this->next = next;
}

