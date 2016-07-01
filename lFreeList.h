#include "list.h"
#include <atomic>

//#define COMPARE_EXCHANGE_WEAK

class LockFreeList : public List
{
public:
	class Node;
	class AtomicElements
	{
	public:
		AtomicElements(void);
		AtomicElements(Node *next, bool erased);
		Node *next;
		bool erased;
	};

	class Node
	{
	private:
		//key element according to which the elements are also ordered
		int item;
		std::atomic<AtomicElements> atomicElements;

	public:
		Node(void) : item(0), atomicElements(AtomicElements(NULL, false)){}
		Node(int item) : item(item), atomicElements(AtomicElements(NULL, false)){}
		Node(int item, Node *next) : item(item), atomicElements(AtomicElements(next, false)){}
		int getItem(void);
		void setItem(int item);
		Node *getNext(void);
		void setNext(Node *next);
		void markAsErased(void);
		bool isErased(void);
		std::atomic<AtomicElements> *getAtomics(void);
	};

	class Window
	{
	public:
		Node *curr, *pred;
		Window(Node *pred, Node *curr);
		//Window(void);
		void lock(void);
		void unlock(void);
	};

	LockFreeList(void);
	~LockFreeList();
	Window find(int item);
	bool contains(int item);
	bool add(int item);
	bool addUnsafe(int item);
	bool remove(int item);
	bool isEmpty(void);

private:
	Node *head, *tail;
};
