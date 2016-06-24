enum Operation {FAIL, CONTAINS, ADD, REMOVE};
struct ListOperation
{
	enum Operation operation;;
	unsigned int data;
};
