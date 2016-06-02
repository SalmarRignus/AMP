#include "fineList.h"
#include <limits>


/**
 * @brief Constructs a list with fine grained locks
 */
CoarseGrainedList::CoarseGrainedList(void)
{
	head = new Node(std::numeric_limits<int>::min(), tail);
	tail = new Node(std::numeric_limits<int>::max(), NULL);
}

/**
 * @brief Performs a delete operation for all elements of the list
 */
CoarseGrainedList::~CoarseGrainedList()
{
	Node *elem;
	Node *elemOld = head->getNext();

	if(elemOld->getNext() != NULL)
	{
		for(elem = elemOld->getNext(); elem->getNext() != NULL; elem = elem->getNext())
		{
			delete elemOld;
			elemOld = elem;
		}
	}
}

/**
 * @brief Searches for an item in the list
 * @param item Key value to look for in the list
 */
Window CoarseGrainedList::find(int item)
{
	//Look for item or successor
	Node *pred = head;
	pred->mut.lock();

	Node *curr = pred->getNext();
	curr->mut.lock();
	while(curr->getItem() < item)
	{
		pred->mut.unlock();
		pred = curr;
		curr = curr->getNext();
		curr->mut.lock();
	}
	return Window(pred, curr);
}

bool CoarseGrainedList::contains(int item)
{
	Window w = find(item);
	bool found;

	found = (item == w.curr->getItem());
	w.unlock();

	return found;
}

/**
 * @brief Adds an element to the list
 * @param item The item of the element, which should be added
 * @return True if the element has been successfully added
 *		   False if the element was not added (already exists)
 */
bool CoarseGrainedList::add(int item)
{
	Window w = find(item);

	if(w.curr->getItem() == item)
	{
		w.unlock();
		return false;
	}
	else
	{
		Node *node = new Node(item, w.curr);
		w.pred->setNext(node);
		w.unlock();
		return true;
	}
}

/**
 * @brief Removes an element from the list
 * @param item The item (key element) of the corresponding element, which should be removed
 * @return True if the element was successfully removed from the list
 *		   False if the element could not be removed (does not exist)
 */
bool CoarseGrainedList::remove(int item)
{
	Window w = find(item);
	if(w.curr->getItem() == item)
	{
		w.pred->setNext(w.curr->getNext());
		w.unlock();
		delete(w.curr);
		return true;
	}
	else
		return false;
}
