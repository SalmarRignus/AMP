#include "fineList.h"
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

