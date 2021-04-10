#include "Main.h"
#include "String.h"

std::string String::Dec2Hex(BYTE value)
{
	std::stringstream Output;
	Output << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << (int)value;
	std::string wut = Output.str();

	return Output.str();
}

std::string String::Byte2Dec(BYTE value)
{
	std::stringstream Output;
	Output << std::uppercase << std::setw(2) << std::setfill('0') << (int)value;
	std::string wut = Output.str();

	return Output.str();
}

std::string String::toupper(std::string str)
{
	for (unsigned int i = 0; i < str.length(); i++)
	{
		if (97 <= str[i] && str[i] <= 122)str[i] -= 32; //a - z
	}

	return str;
}

std::string String::tolower(std::string str)
{
	for (unsigned int i = 0; i < str.length(); i++)
	{
		if (65 <= str[i] && str[i] <= 90)str[i] += 32;//A - Z
	}

	return str;
}