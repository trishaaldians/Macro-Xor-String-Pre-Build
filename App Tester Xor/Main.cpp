#include <Windows.h>
#include <stdio.h>

#include "../Common/XorStr/XorStr.h"

int main(void)
{
	//example use:
	//just add XOR("")

	//printf(XOR("This string is hide\n"));
	//printf(XOR("sorry my english is bad (:\n"));

	printf(/*This string is hide\n*/XorStr<0x29,21,0xE184BE23>("\x7D\x42\x42\x5F\x0D\x5D\x5B\x42\x58\x5C\x54\x14\x5C\x45\x17\x50\x50\x5E\x5E\x36"+0xE184BE23).decode);
	printf(/*sorry my english is bad (:\n*/XorStr<0x6C,28,0x9052AED6>("\x1F\x02\x1C\x1D\x09\x51\x1F\x0A\x54\x10\x18\x10\x14\x10\x09\x13\x5C\x14\x0D\x5F\xE2\xE0\xE6\xA3\xAC\xBF\x8C"+0x9052AED6).decode);

	system("pause"); // sorry i just wanna stop the process
}