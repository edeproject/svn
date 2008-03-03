#include "EdbusDict.h"
#include <stdio.h>

int main() {
	EdbusDict dict;
	dict.push_back(3, "foo");
	dict.push_back(2, "baz");
	dict.push_back(1, "taz");

	EdbusDict dict2 = dict;
	//dict2.replace(3, "goo");

	EdbusData e1,e2;

	e1 = dict.find(3);
	e2 = dict2.find(3);

	printf("%s %s\n", e1.to_string(), e2.to_string());

	e1 = EdbusData::from_string("bla bla");
	e2 = e1;

	printf("%s == %s\n", e1.to_string(), e2.to_string());

	e2 = EdbusData::from_string("xxxxxxxxxx");
	printf("%s == %s\n", e1.to_string(), e2.to_string());

	return 0;
}
