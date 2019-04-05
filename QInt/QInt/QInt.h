#include <iostream>
#include <string>
#include <math.h>
#include <vector>


using namespace std;

class QInt {
public:
	QInt();
	QInt operator+(QInt n);
	QInt operator-(QInt n);
	QInt operator*(QInt n); 
	QInt operator/(QInt n); 

	QInt operator&(QInt n); 
	QInt operator|(QInt n); 
	QInt operator^(QInt n); 

	QInt operator>>(unsigned int k); 
	QInt operator<<(unsigned int k); 

	QInt operator~();
	QInt rol();
	QInt ror();
	void operator=(QInt n);

	void stringToQInt(string n, int base);
	string toString(int base);
private:
	int getSignBit();

	int getBit(int pos);
	void setBit(int pos, int bit);
	QInt shiftLeft(unsigned int k);
	QInt changeSign();
	
	void decToQInt(string number);
	void binToQInt(string binary);
	void hexToQInt(string hex);

	string toBin();
	string toDec();
	string toHex();

	bool isNegative();
	bool isPositive();


	unsigned int arrayBits[4];
};


//=========================================

string Div2(string number);
string Mult2(string number);
string Sum(string str1, string str2);

string twoComplement(string binary);
string codeBinaryOfHex(char hex);
string codeHexOfBinary(string binary);
string DecToBinary(string number);
string DecToTwosComplementBinary(string number);
string TwosComplementBinaryToDec(string binary); 
string BinaryToDec(string binary);
string BinaryToHex(string binary);
string HexToBinary(string hex);



