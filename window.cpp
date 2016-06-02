#include "fineList.h"

//Window::Window(void)
//{
//	this->curr = NULL;
//	this->pred = NULL;
//}

FineGrainedList::Window::Window(Node *curr, Node *pred)
{
	this->curr = curr;
	this->pred = pred;
}

void FineGrainedList::Window::unlock(void)
{
	this->curr->mut.unlock();
	this->pred->mut.unlock();
}