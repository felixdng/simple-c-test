#include <iostream>
#include <stdlib.h>
#include "test.h"

using namespace std;

int main(int argc, char *argv[])
{
	if (argc != 2) {
		cout << argv[0] << " <number>" << endl;
		return 1;
	}
	int num = strtoul(argv[1], NULL, 0);
    Test myTest;
    myTest.TestRun(num);
    return 0;
}
