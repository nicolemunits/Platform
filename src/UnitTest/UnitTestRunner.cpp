#include "UnitTest.h"

using namespace npl;

int main2(int argc, char* argv[]) {
	Test t;
	if (t.test())
		cout << "[PASS]" << endl;
	else
		cout << "[FAIL]" << endl;
}
