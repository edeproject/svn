#include "EdbusDict.h"
#include <stdio.h>

int main() {
	EdbusDict dict;
	dict.push_back(3, "foo");
	dict.push_back(2, "baz");
	dict.push_back(1, "taz");

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

	dict2.push_back("key", 34);
	dict2.push_back("key2", 4);

	printf("%i %i\n", dict.size(), dict2.size());

	return 0;
}
