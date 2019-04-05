#include "BaseConverter.h"
#include <iostream>
#include <string>


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

	int getSignBit();
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
