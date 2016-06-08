#include "EdbusList.h"
#include <stdio.h>

int main() {
	EdbusList lst = EdbusList::create_struct();
	lst << EdbusData::from_int32(3) << EdbusData::from_string("Foo") << EdbusData::from_int32(4);
	printf("size: %i\n", lst.size());

	EdbusList::const_iterator it = lst.begin(), it_end = lst.end();
	printf("%i\n", (*it).to_int32());
	++it;
	printf("%s\n", (*it).to_string());
	++it;
	printf("%i\n", (*it).to_int32());

	EdbusList array = EdbusList::create_array();
	array << EdbusData::from_int32(3) << EdbusData::from_string("Foo") << EdbusData::from_int32(4);
	printf("size: %i\n", array.size());

	return 0;
}
