#include <cstdlib>

int main(int argc, char *argv[])
{
	system("./listTest 20 ListContentCorrSort.txt full");
	system("./listTest 20 ListContentBackSort.txt full");
	system("./listTest 20 ListContentRandom.txt full");

	return EXIT_SUCCESS;
}
