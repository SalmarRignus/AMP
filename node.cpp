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

/**********Lazy List Implementation**********/
LazyList::Node::Node(void)
{
	this->item = 0;
	this->next = NULL;
	this->erased = false;
}
LazyList::Node::Node(int item)
{
	this->item = item;
	this->next = NULL;
	this->erased = false;
}
LazyList::Node::Node(int item, LazyList::Node *next)
{
	this->item = item;
	this->next = next;
	this->erased = false;
}
int LazyList::Node::getItem(void)
{
	return this->item;
}
void LazyList::Node::setItem(int item)
{
	this->item = item;
}
LazyList::Node *LazyList::Node::getNext(void)
{
	return this->next;
}
void LazyList::Node::setNext(LazyList::Node *next)
{
	this->next = next;
}
bool LazyList::Node::isErased(void)
{
	return erased;
}
void LazyList::Node::markAsErased(void)
{
	erased = true;
}

/**********Lock free List Implementation**********/
//LockFreeList::Node::Node(void)
//{
//	this->item = 0;
//}
//LockFreeList::Node::Node(int item)
//{
//	this->item = item;
//}
//LockFreeList::Node::Node(int item, LockFreeList::Node *next)
//{
//	this->item = item;
//}
int LockFreeList::Node::getItem(void)
{
	return this->item;
}
void LockFreeList::Node::setItem(int item)
{
	this->item = item;
}
LockFreeList::Node *LockFreeList::Node::getNext(void)
{
	return this->atomicElements.load().next;
}
void LockFreeList::Node::setNext(LockFreeList::Node *next)
{
	AtomicElements AE = this->atomicElements.load();
	AE.next = next;
	this->atomicElements.store(AE);
}
bool LockFreeList::Node::isErased(void)
{
	return this->atomicElements.load().erased;
}
void LockFreeList::Node::markAsErased(void)
{
	AtomicElements AE = this->atomicElements.load();
	AE.erased = true;
	this->atomicElements.store(AE);
}
std::atomic<LockFreeList::AtomicElements> *LockFreeList::Node::getAtomics(void)
{
	return &(this->atomicElements);
}

/**********marked lock-free list**********/
//int LockFreeMarkedList::Node::getItem(void)
//{
//	return this->item;
//}
//void LockFreeMarkedList::Node::setItem(int item)
//{
//	this->item = item;
//}
//LockFreeMarkedList::Node *LockFreeMarkedList::Node::getNext(void)
//{
//	return this->atomicElements.load();
//}
//void LockFreeMarkedList::Node::setNext(LockFreeMarkedList::Node *next)
//{
//	this->next.store(next);
//}
