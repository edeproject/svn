#include "EdbusDict.h"
#include <stdio.h>

int main() {
	EdbusDict dict;
	dict.append(3, "foo");
	dict.append(2, "baz");
	dict.append(1, "taz");

	EdbusDict dict2 = dict;

	EdbusData e1,e2;

	e1 = dict.find(3);
	e2 = dict2.find(3);

	printf("%s %s\n", e1.to_string(), e2.to_string());

	e1 = EdbusData::from_string("bla bla");
	e2 = e1;

	printf("%s == %s\n", e1.to_string(), e2.to_string());

	e2 = EdbusData::from_string("xxxxxxxxxx");
	printf("%s == %s\n", e1.to_string(), e2.to_string());

	dict2.clear();
	e1 = dict2.find(2);
	e2 = dict.find(2);

	printf("%i %i\n", e1.is_valid(), e2.is_valid());

	dict2.append("key", 34);
	dict2.append("key2", 4);
	dict2.append("key2", 5);

	printf("%i %i\n", dict.size(), dict2.size());
	e1 = dict2.find("key2");
	printf("%i\n", e1.to_int32());

	dict.clear();
	dict2.clear();

	dict.append(3, 3);
	dict.append(1, 3);
	dict.append('c', 3); // must not be accepted

	dict2.append("xx", 3);
	dict2.append("yy", 66);
	dict2.append("zz", 9);

	printf("dict == dict2 %i %i %i\n", dict == dict2, dict.size(), dict2.size());

	puts("here---");
	EdbusDict::iterator it = dict2.begin(), it_end;
	it_end = dict2.end();

	while(it != it_end) {
		printf("key: %s value: %i\n", (*it).key.to_string(), (*it).value.to_int32());
		++it;
	}

	/*
	 * test for empty dict
	 * TODO: add this to the suite too
	 */
	EdbusDict dd;

	it = dd.begin(), it_end = dd.end();
	while(it != it_end) {
		printf("key: %i value: %i\n", (*it).key.type(), (*it).value.type());
		++it;
	}


	return 0;
}
