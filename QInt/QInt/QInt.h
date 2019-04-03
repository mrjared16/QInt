#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <math.h>
#include <vector>

#define NEGATIVE -1
#define ZERO 0
#define POSITIVE 1

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
	int getBit(int pos);
	void setBit(int pos, int bit);

	void decToQInt(string number);
	void binToQInt(string binary);
	void hexToQInt(string hex);

	string toBin();
	string toDec();
	string toHex();

	bool isZero();

	unsigned int arrayBits[4];
	int sign; // -1 is nagative, 1 is positive, 0 is zero
};


//=========================================
// Ham tham khao tren mang
string Div2(string number);
string Sum(string str1, string str2);
string multiply(string num1, string num2);
//=========================================

string twoHat(int num); //Find 2^(num) = ?
string standardizedBinary(string binary); //
string twoComplement(string binary);
string codeBinaryOfHex(char hex);
string codeHexOfBinary(string binary);
string DecToBinary(string number);
string BinaryToDec(string binary); // if twoCompl = true, convert two complement format
string BinaryToHex(string binary);
string HexToBinary(string hex);



