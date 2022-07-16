#include "angletest.h"
#include <bbms/datatypes.h>
#include <assert.h>

/***********************************************************
 * ANGLE TEST                                              *
 ***********************************************************/
using namespace std;
void AngleTest() {
	cout << "Angle Test\n\tVerifying size\n";

	Byte angle;
	assert(sizeof(angle) == 1);

	cout << "\tFinished\n\n";
}
