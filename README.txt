In this project we investigate the efficiency of different concurrent list implementations.

Use the Makefile to compile the project.
To generate test data, call:
./createListData
If you want to add new test files, modify createListData.cpp.

For testing either use
./test
or directly call
./listTest
with the appropriate paramters, for example:
./listTest 4 ListContentRandom_80_10_10.txt performance
