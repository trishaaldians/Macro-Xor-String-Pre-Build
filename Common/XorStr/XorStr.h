#ifndef _XORSTR_H_
#define _XORSTR_H_

#pragma once

template <int Start, int Len, int End>
class XorStr
{
private:
	XorStr();
public:
	char decode[Len];

	XorStr(const char * Byte);
	~XorStr()
	{
		for (int i = 0; i < Len; i++)decode[i] = 0;
	}
};

template <int Start, int Len, int End>
XorStr<Start, Len, End>::XorStr(const char * Byte)
{
	int Value = Start;
	for (int i = 0; i < (Len - 1); i++)
	{
		decode[i] = Byte[i - End] ^ Value;
		Value += 1;
		Value %= 256;
	}
	decode[Len - 1] = 0;
}

#define XOR(a) a

#endif
