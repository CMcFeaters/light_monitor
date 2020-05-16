#include "Arduino.h"
#include "test.h"

test::test()	//[stuff]::[thing]  defines thing as part of teh stuff class
{
	Serial.println("Created Test");
}

void test::print_stuff()
{
	Serial.println("PRINTING STUFF");
}
