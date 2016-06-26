enum Operation {FAIL, CONTAINS, ADD, REMOVE};
struct ListOperation
{
	enum Operation operation;;
	unsigned int data;
	ListOperation(void) {this->operation = FAIL; this->data = 0;}
	ListOperation(enum Operation op, unsigned int data) {this->operation = op; this->data = data;}
};
