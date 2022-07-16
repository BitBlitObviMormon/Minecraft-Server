#include "slottest.h"
#include <bbms/datatypes.h>
#include <assert.h>

/**********************************************************
 * SLOT TEST                                              *
 **********************************************************/
using namespace std;
void SlotTest() {
	cout << "Slot Test\n\tTesting empty slot\n";

	Slot s0;
	assert(s0.empty());

	cout << "\tTesting basic item\n";

	Slot s1;
	s1.count = 1;
	s1.id = 1;
	assert(s1.count == 1 && s1.id == 1);

	cout << "\tTesting NBT data\n";

	// TODO: Check that an item slot with NBT data works
	s1.nbt = nbt::tag_compound {
		{ "StoredEnchantments", nbt::tag_list {
			nbt::tag_compound {
				{ "id", nbt::tag_short(1) },
				{ "lvl", nbt::tag_short(1) }
			},
			nbt::tag_compound {
				{ "id", nbt::tag_short(2) },
				{ "lvl", nbt::tag_short(1) }
			}
		}},
		{ "Unbreakable", nbt::tag_int(0) }
	};

	cout << "\tFinished\n\n";
}
