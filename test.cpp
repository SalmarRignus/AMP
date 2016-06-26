#include <cstdlib>

int main(int argc, char *argv[])
{
	system("./listTest 20 ListContentCorrectness.txt correctness");

	system("./listTest 20 ListContentCorrSort.txt performance");
	system("./listTest 20 ListContentBackSort.txt performance");

	system("./listTest 20 ListContentRandom_80_10_10.txt performance");
	system("./listTest 20 ListContentRandom_70_20_10.txt performance");
	system("./listTest 20 ListContentRandom_60_20_20.txt performance");
	system("./listTest 20 ListContentRandom_50_35_15.txt performance");
	system("./listTest 20 ListContentRandom_40_30_30.txt performance");

	return EXIT_SUCCESS;
}
