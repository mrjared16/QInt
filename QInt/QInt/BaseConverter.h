#pragma once
#include "BigIntCalculator.h"

class BaseConverter
{
public:
	static string getTwosComplementBinOfDec(string decimal);
	static string getDecOfTwosComplementBin(string binary);

	static string getBinOfHex(string hex);
	static string getHexOfBin(string binary);

	 static string getTwosComplement(string binary);
private:
	 static string codeBinOfHex(char hex);
	 static string codeHexOfBin(string binary);
	 	
	 static string getBinOfDec(string decimal);
	 static string getDecOfBin(string binary);
};	 

