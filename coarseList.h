#include "list.h"

class CoarseGrainedList: public List
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
		int getItem(void);
		void setItem(int item);
		Node *getNext(void);
		void setNext(Node *next);
	};

	class Window
	{
	public:
		Node *curr, *pred;
		Window(Node *curr, Node *pred);
		//Window(void);
	};

	CoarseGrainedList(void);
	~CoarseGrainedList();
	Window find(int item);
	bool contains(int item);
	bool add(int item);
	bool remove(int item);
	bool isEmpty(void);

private:
	std::mutex mut;
	Node *head, *tail;
};
