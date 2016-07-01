#include "list.h"

class FineGrainedList : public List
{
public:
	class Node
	{
	private:
		//key element according to which the elements are also ordered
		int item;
		Node *next;

	public:
		Node(void);
		Node(int item);
		Node(int item, Node *next);
		std::mutex mut;
		int getItem(void);
		void setItem(int item);
		Node *getNext(void);
		void setNext(Node *next);
	};

	class Window
	{
	public:
		Node *curr, *pred;
		Window(Node *pred, Node *curr);
		//Window(void);
		void unlock(void);
	};

	FineGrainedList(void);
	~FineGrainedList();
	Window find(int item);
	bool contains(int item);
	bool add(int item);
	bool addUnsafe(int item);
	bool remove(int item);
	bool isEmpty(void);

private:
	Node *head, *tail;
};
