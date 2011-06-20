#include "emotiv.h"
#include <iostream>

int main()
{
	emotiv myheadset;
	unsigned char key[] = "\x31\x00\x39\x54\x38\x10\x37\x42\x31\x00\x39\x48\x38\x00\x37\x50";
	myheadset.setKey(key);


	while(1)
	{
		myheadset.readBuffer();
	}

	
	return 0;
}
