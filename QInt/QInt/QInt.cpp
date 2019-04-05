#include "QInt.h"

QInt::QInt()
{
	for (int i = 0; i < 4; i++) {
		this->arrayBits[i] = 0;
	}
}

// pos: 1 - 128
// bit: 0 / 1

void QInt::setBit(int pos, int bit)
{
	pos -= 1;
	// Tat bit
	if (bit == 0)
	{
		this->arrayBits[3 - pos / 32] &= ~(1 << (pos % 32));
	}
	else // Bat bit
		this->arrayBits[3 - pos / 32] |= (1 << (pos % 32));

}

int QInt::getBit(int pos)
{
	pos -= 1;
	int tmp = this->arrayBits[3 - pos / 32] & (1 << pos % 32);
	// bit thu pos = 1 => tmp != 0
	return (tmp == 0) ? 0 : 1;
}


int QInt::getSignBit()
{
	return this->getBit(128);
}

bool QInt::isNegative()
{
	return (this->getSignBit() == 1);
}

bool QInt::isPositive()
{
	return (this->getSignBit() == 0);
}


QInt QInt::operator+(QInt n)
{
	QInt result;
	int sum = 0;
	int i = 1;
	int carry = 0;

	// Cong bit lan luot tu bit (1) -> (128)
	for (int i = 1; i <= 128; i++) {
		// Cong hai bit vao sum
		// Cong them carry neu co
		sum = this->getBit(i) + n.getBit(i) + carry;
		result.setBit(i, sum % 2);
		// Tinh carry  
		carry = sum / 2;
	}

	return result;
}

QInt QInt::operator-(QInt n)
{
	// a - b = a + (-b)
	QInt n1;
	// chuyen sang dang bu hai => chuyen sang QInt
	n1.stringToQInt(BaseConverter::twoComplement(n.toBin()), 2); // n1 = -n
	return (*this + n1);
}

QInt QInt::operator*(QInt n)
{
	QInt A;
	for (int i = 1; i <= 128; i++) {
		// Nhan tung bit cua n voi (*this) roi cong cac ket qua
		// Chi co cac bit 1 moi anh huong toi tich
		if (n.getBit(i) == 1) {
			// (*this) * 1 = (*this)
			// Qua 1 bit thi dich trai 1 lan => dich trai (i - 1) lan
			A = A + this->shiftLeft(i - 1);
		}
	}
	return A;
}

QInt QInt::operator/(QInt n)
{
	QInt A;
	QInt Q = *this;
	QInt M = n;
	//Neu Q hoac M la so am thi doi thanh so duong va thuc hien phep chia
	if (this->isNegative()) {
		Q = Q.changeSign();
	}
	if (n.isNegative()) {
		M = M.changeSign();
	}
	int k = 128;
	int bit;
	while (k > 0) {
		bit = Q.getSignBit();	//Luu lai bit most left cua Q truoc khi dich trai

		//Dich trai [A,Q]
		A = A.shiftLeft(1);
		Q = Q.shiftLeft(1);

		A.setBit(1, bit);		//Cai dat bit cho bit 1 cua A = most left cua Q

		A = A - M;
		if (A.isNegative()) {
			A = A + M;
			Q.setBit(1, 0);
		}
		else {
			Q.setBit(1, 1);
		}
		k--;
	}
	//Neu Q hoac M khac dau thi doi dau thuong thanh so am
	if (this->getSignBit() != n.getSignBit()) {
		Q = Q.changeSign();
	}
	return Q;
}


QInt QInt::operator>>(unsigned int k)
{
	QInt result;
	// Luu bit dau
	result.setBit(128, this->getSignBit());
	// Truong hop QInt < 0, 1000 >> 2 = 1110
	int negative_sign_bit = 1;
	if (this->isNegative())
	{
		// Gan cac bit (127) -> (128 - k + 1) = 1
		for (int i = 127; i >= 128 - k + 1; i--)
		{
			result.setBit(i, negative_sign_bit);
		}
	}

	// Dich cac bit con lai: (128 - k) -> (1)
	for (int i = 128; i - k >= 1; i--)
	{
		result.setBit(i - k, this->getBit(i));
	}

	return result;
}

QInt QInt::operator<<(unsigned int k)
{
	QInt result;
	// Luu bit dau
	result.setBit(128, this->getSignBit());
	// Dich cac bit con lai
	// Bat dau tu 127, bo qua bit dau
	for (int i = 127; i - k >= 1; i--)
	{
		result.setBit(i, this->getBit(i - k));
	}

	return result;
}


QInt QInt::operator&(QInt n)
{
	QInt result;
	int tmp = 0;
	for (int i = 1; i <= 128; i++)
	{
		tmp = this->getBit(i) & n.getBit(i);
		result.setBit(i, tmp);
	}

	return result;
}

QInt QInt::operator|(QInt n)
{
	QInt result;
	int tmp = 0;
	for (int i = 1; i <= 128; i++)
	{
		tmp = this->getBit(i) | n.getBit(i);
		result.setBit(i, tmp);
	}

	return result;
}

QInt QInt::operator^(QInt n)
{
	QInt result;
	int tmp = 0;
	for (int i = 1; i <= 128; i++)
	{
		tmp = this->getBit(i) ^ n.getBit(i);
		result.setBit(i, tmp);
	}

	return result;
}


QInt QInt::operator~()
{
	QInt result;
	for (int i = 1; i <= 128; i++)
	{
		result.setBit(i, (this->getBit(i) == 0) ? 1 : 0);
	}

	return result;
}
// Xoay trai 1 bit
QInt QInt::rol()
{
	QInt result;
	// bit cuoi la bit dau cua bit ban dau
	result.setBit(1, this->getSignBit());
	for (int i = 128; i - 1 >= 1; i--)
	{
		result.setBit(i, this->getBit(i - 1));	// i: (128) -> (2)
	}

	return result;
}
// Xoay phai 1 bit
QInt QInt::ror()
{
	QInt result;
	// bit dau la bit cuoi cua bit ban dau
	result.setBit(128, this->getBit(1));
	// Dich cac bit con lai
	for (int i = 128; i - 1 >= 1; i--)
	{
		result.setBit(i - 1, this->getBit(i)); // i: (128) -> (2)
	}

	return result;
}


void QInt::operator=(QInt n)
{
	for (int i = 0; i < 4; i++) {
		this->arrayBits[i] = n.arrayBits[i];
	}
}


// Dich trai kieu luan li
// Su dung trong phep nhan, chia
QInt QInt::shiftLeft(unsigned int k)
{
	QInt result;
	// Dich cac bit qua trai k don vi
	for (int i = 128; i - k >= 1; i--)
	{
		result.setBit(i, this->getBit(i - k));
	}

	return result;
}

// Lay so doi cua -x cua x
QInt QInt::changeSign()
{
	QInt zero;
	return zero - *this; // a = 0 - a = -a
}


void QInt::stringToQInt(string n, int base)
{
	switch (base)
	{
	case 2:
		binToQInt(n);
		break;
	case 10:
		decToQInt(n);
		break;
	case 16:
		hexToQInt(n);
		break;
	}
}

string QInt::toString(int base)
{
	string result = "";
	switch (base)
	{
	case 2:
		result = toBin();
		break;
	case 10:
		result = toDec();
		break;
	case 16:
		result = toHex();
		break;
	}
	return result;
}


void QInt::decToQInt(string number)
{
	if (number == "0")
		return;
	string binary = BaseConverter::DecToTwosComplementBinary(number);
	this->binToQInt(binary);
}

// Doc chuoi binary vao QInt
// binary: Khong can chuan hoa du 128 ki tu
// Cac bit 0 dau con lai duoc tao boi constructor
void QInt::binToQInt(string binary)
{
	int i = 1;
	int len = binary.size() - 1;
	// QInt danh dau bit tu trai sang phai
	// QInt.bit(1) = lastCharOf(binary)
	// i tang cho toi khi ve toi dau chuoi
	while (len >= 0) {
		this->setBit(i, (binary[len] - '0')); // % 2);
		i++;
		len--;
	}
}

// Doc chuoi hex vao QInt
void QInt::hexToQInt(string hex) {
	// Chuyen sang binary dang toi gian
	// 0x01 => 0001 
	string binary = BaseConverter::HexToBinary(hex);
	this->binToQInt(binary);
}


// Chuyen sang binary dang toi gian
string QInt::toBin()
{
	string binary = "";
	bool flag = false;	// Danh dau doc dc bit 1 dau tien => bo cac bit 0 o dau
	// Doc tu trai -> phai

	for (int i = 128; i >= 1; i--)
	{
		int bit = this->getBit(i);
		if (!flag)
		{
			if (bit == 0)
				continue;
			flag = true;
		}
		binary += (bit + '0');
	}
	return binary;
}

string QInt::toDec()
{
	string binary = this->toBin();
	string result = BaseConverter::TwosComplementBinaryToDec(binary);
	return result;
}

string QInt::toHex() {
	string binary = this->toBin();
	string result = BaseConverter::BinaryToHex(binary);
	return result;
}
