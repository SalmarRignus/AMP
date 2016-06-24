#include "lazyList.h"
#include <limits>
#include <iostream>


/**
 * @brief Constructs a lazy list
 */
LazyList::LazyList(void)
{
	tail = new Node(std::numeric_limits<int>::max(), NULL);
	head = new Node(std::numeric_limits<int>::min(), tail);
}

/**
 * @brief Performs a delete operation for all elements of the list
 */
LazyList::~LazyList()
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
LazyList::Window LazyList::find(int item)
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
bool LazyList::contains(int item)
{
	Node *n = head;

	while(n->getItem() < item)
		n = n->getNext();
	
	return (n->getItem() == item) && !n->isErased();
}

/**
 * @brief Adds an element to the list
 * @param item The item of the element, which should be added
 * @return True if the element has been successfully added
 *		   False if the element was not added (already exists)
 */
bool LazyList::add(int item)
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
bool LazyList::remove(int item)
{
	Window w = find(item);
	if(w.curr->getItem() == item)
	{
		w.curr->markAsErased();
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

bool LazyList::validate(Window w)
{
	return	!w.pred->isErased() &&
			!w.curr->isErased() &&
			w.pred->getNext() == w.curr;
}

/**
 * @details Check whether the list is empty
 */
bool LazyList::isEmpty(void)
{
	bool isEmpty = false;

	head->mut.lock();
	if(head->getNext() == tail)
		isEmpty = true;

	head->mut.unlock();

	return isEmpty;
}
