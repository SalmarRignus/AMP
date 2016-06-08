#include "list.h"

class OptList : public List
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
		void lock(void);
		void unlock(void);
	};

	OptList(void);
	~OptList();
	Window find(int item);
	bool contains(int item);
	bool add(int item);
	bool remove(int item);
	bool isEmpty(void);
	bool validate(Window w);

private:
	Node *head, *tail;
};
