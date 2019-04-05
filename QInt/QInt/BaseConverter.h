#pragma once
#include "BigIntCalculator.h"

class BaseConverter
{
public:
	static string DecToTwosComplementBinary(string number);
	static string TwosComplementBinaryToDec(string binary);

	static string HexToBinary(string hex);
	static string BinaryToHex(string binary);

	 static string twoComplement(string binary);
private:
	 static string codeBinaryOfHex(char hex);
	 static string codeHexOfBinary(string binary);
	 	
	 static string DecToBinary(string number);
	 static string BinaryToDec(string binary);
};	 

