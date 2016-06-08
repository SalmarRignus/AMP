#include "optList.h"
#include <limits>
#include <iostream>


/**
 * @brief Constructs a list with "optimistic locking"
 */
OptList::OptList(void)
{
	tail = new Node(std::numeric_limits<int>::max(), NULL);
	head = new Node(std::numeric_limits<int>::min(), tail);
}

/**
 * @brief Performs a delete operation for all elements of the list
 */
OptList::~OptList()
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
 * @detail If the place, where the element should be was found, it returns a
		   "window element" containing a predecessor and current node
		   inbetween which the said element should be
 * @param item Key value to look for in the list
 */
OptList::Window OptList::find(int item)
{
	while(true)
	{
		Node *pred = head;
		Node *curr = pred->getNext();

		while(curr->getItem() < item)
		{
			pred = curr;
			curr = curr->getNext();
		}

		Window w(pred, curr);
		w.lock();
		if(validate(w))
			return w;
		
		w.unlock();
	}
}

/**
 * @brief Returns true if the item is in the list and false otherwise
 * @param item Element to look for in the list
 * @return True if the item is in the list and false otherwise
 */
bool OptList::contains(int item)
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
bool OptList::add(int item)
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
bool OptList::remove(int item)
{
	Window w = find(item);
	if(w.curr->getItem() == item)
	{
		w.pred->setNext(w.curr->getNext());
		w.unlock();
		//we must not physically delete a node, as it is unsafe
		//delete(w.curr);
		return true;
	}
	else
	{
		w.unlock();
		return false;
	}
}

bool OptList::isEmpty(void)
{
	bool isEmpty = false;

	head->mut.lock();
	if(head->getNext() == tail)
		isEmpty = true;

	head->mut.unlock();

	return isEmpty;
}

bool OptList::validate(Window w)
{
	Node *n = head;
	while(n->getItem() <= w.pred->getItem())
	{
		if(n == w.pred)
			return n->getNext() == w.curr;

		n = n->getNext();
	}

	return false;
}
