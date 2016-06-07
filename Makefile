all:
	g++ -std=c++11 fineList.cpp listTest.cpp node.cpp window.cpp -o listTest

clean:
	rm listTest listTest.exe
