#ifndef LIST
#define LIST
#include<mutex>

class List
{
public:
	virtual bool contains(int item) {return false;}
	virtual bool add(int item) {return false;}
	virtual bool remove(int item) {return false;}
	virtual bool isEmpty(void) {return false;}
};

#endif
