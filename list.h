#ifndef LIST
#define LIST
#include<mutex>

class List
{
public:
	class Node
	{
	public:
		Node(int item, Node *next) {}
		virtual	Node *getNext(void) {return NULL;}
		virtual int getItem(void) {return 0;}
		virtual void setNext(Node *next) {return;}
	};
	virtual bool contains(int item) {return false;}
	virtual bool add(int item) {return false;}
	virtual bool addUnsafe(int item);
	virtual bool remove(int item) {return false;}
	virtual bool isEmpty(void) {return false;}
	//virtual ~List();
private:
	Node *head, *tail;
};

#endif
