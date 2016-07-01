#compile with c11 standard and enable 16bit CAS (compare and swap) operation
FLAGS = -std=c++11 -mcx16 -Wall -O3 -pthread

all: listTest createListData test

listTest: fineList.cpp fineList.h coarseList.cpp coarseList.h optList.cpp optList.h lazyList.cpp lazyList.h lFreeList.cpp lFreeList.h listTest.cpp node.cpp window.cpp list.cpp
	g++ $(FLAGS) fineList.cpp coarseList.cpp optList.cpp lazyList.cpp lFreeList.cpp listTest.cpp node.cpp window.cpp list.cpp -o listTest
createListData: createListData.cpp
	g++ $(FLAGS) createListData.cpp -o createListData
test: test.cpp
	g++ $(FLAGS) test.cpp -o test

clean:
	rm -f listTest createListData test listTest.exe createListData.exe test.exe

.PHONY: all clean
