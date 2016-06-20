#compile with c11 standard and enable 16bit CAS (compare and swap) operation
FLAGS = -std=c++11 -mcx16

all:
	g++ $(FLAGS) *.cpp -o listTest

clean:
	rm listTest listTest.exe
