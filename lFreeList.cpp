#include "lFreeList.h"
#include <limits>
#include <iostream>
#include <atomic>


/**
 * @brief Constructs a lock free list
 */
LockFreeList::LockFreeList(void)
{
	tail = new Node(std::numeric_limits<int>::max(), NULL);
	head = new Node(std::numeric_limits<int>::min(), tail);
}

/**
 * @brief Performs a delete operation for all elements of the list
 */
LockFreeList::~LockFreeList()
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
LockFreeList::Window LockFreeList::find(int item)
{
	retry:	while(true)
	{
		Node *pred = head;
		Node *curr = pred->getNext();

		while(true)
		{
			while(curr->isErased())
			{
				AtomicElements AEPred(curr, false);
				AtomicElements AECurr(curr->getNext(), false);

				//unlink curr from the list
#ifndef COMPARE_EXCHANGE_WEAK
				if(!pred->getAtomics()->compare_exchange_strong(AEPred, AECurr))
					goto retry;
#else
				if(!pred->getAtomics()->compare_exchange_weak(AEPred, AECurr))
					goto retry;
#endif
				curr = curr->getNext();
			}

			if(curr->getItem() >= item)
			{
				Window w(pred, curr);
				return w;
			}
			pred = curr;
			curr = curr->getNext();
		}
	}
}

/**
 * @brief Returns true if the item is in the list and false otherwise
 * @param item Element to look for in the list
 * @return True if the item is in the list and false otherwise
 */
bool LockFreeList::contains(int item)
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
bool LockFreeList::add(int item)
{
	while(true)
	{
		Window w = find(item);
		
		if(w.curr->getItem() == item)
		{
			return false;
		}
		else
		{
			Node *node = new Node(item, w.curr);

			AtomicElements AEPred(w.curr, false);
			AtomicElements AENode(node, false);

			//if the next pointer of pred still points to curr and it is not marked as erased,
			//then atomically insert the new node
#ifndef COMPARE_EXCHANGE_WEAK
			if(w.pred->getAtomics()->compare_exchange_strong(AEPred, AENode))
				return true;
			if(w.pred->getAtomics()->compare_exchange_weak(AEPred, AENode))
				return true;
#else
#endif
		}
	}
}

/**
 * @brief Removes an element from the list
 * @param item The item (key element) of the corresponding element, which should be removed
 * @return True if the element was successfully removed from the list
 *		   False if the element could not be removed (does not exist)
 */
bool LockFreeList::remove(int item)
{
	while(true)
	{
		Window w = find(item);
		if(w.curr->getItem() == item)
		{
			Node *succ = w.curr->getNext();
			AtomicElements AECurr(succ, false);
			AtomicElements AEErasedCurr(succ, true);

			//if curr still points to its "former" next element and this next element is not deleted
			//then mark curr as deleted otherwise start over
#ifndef COMPARE_EXCHANGE_WEAK
			if(!w.curr->getAtomics()->compare_exchange_strong(AECurr, AEErasedCurr))
				continue;
#else
			if(!w.curr->getAtomics()->compare_exchange_weak(AECurr, AEErasedCurr))
				continue;
#endif

			AtomicElements AEPred(w.curr, true);
			//try to unlink curr from the list if it still exists
#ifndef COMPARE_EXCHANGE_WEAK
			w.pred->getAtomics()->compare_exchange_strong(AEPred, AECurr);
#else
			w.pred->getAtomics()->compare_exchange_weak(AEPred, AECurr);
#endif

			return true;
		}
		else
		{
			return false;
		}
	}
}

bool LockFreeList::isEmpty(void)
{
	if(head->getNext() == tail)
		return true;
	//traverse the list and check whether all elements are marked as "erased"
	else
	{
		for(Node *node = head->getNext(); node != tail; node = node->getNext())
		{
			if(!node->isErased())
				return false;
		}
		return true;
	}
}

LockFreeList::AtomicElements::AtomicElements(void)
{
	this->next = NULL;
	this->erased = false;
}

LockFreeList::AtomicElements::AtomicElements(Node *next, bool erased)
{
	this->next = next;
	this->erased = erased;
}
