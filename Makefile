#compile with c11 standard and enable 16bit CAS (compare and swap) operation
FLAGS = -std=c++11 -mcx16

all:
	g++ $(FLAGS) fineList.cpp coarseList.cpp optList.cpp lazyList.cpp lFreeList.cpp listTest.cpp node.cpp window.cpp -o listTest
	g++ $(FLAGS) createListData.cpp -o createListData

clean:
	rm listTest createListData listTest.exe createListData.exe
