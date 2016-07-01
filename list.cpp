#include "list.h"

bool List::addUnsafe(int item)
{
	Node *pred, *curr;

	pred = head;
	curr = head->getNext();

	while(curr->getItem() < item)
	{
		pred = curr;
		curr = curr->getNext();
	}
	if(curr->getItem() == item)
		return false;
	else
	{
		Node *node = new Node(item, curr);
		pred->setNext(node);
		return true;
	}
}
