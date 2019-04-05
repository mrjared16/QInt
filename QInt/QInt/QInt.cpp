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
	n1.stringToQInt(twoComplement(n.toBin()), 2); // n1 = -n
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
	string binary = DecToTwosComplementBinary(number);
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
	string binary = HexToBinary(hex);
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
	string result = TwosComplementBinaryToDec(binary);
	return result;
}

string QInt::toHex() {
	string binary = this->toBin();
	return BinaryToHex(binary);
}















string Div2(string number)
{
	string result;
	// Find prefix of number that is larger 
	// than divisor. 
	int temp = number[0] - '0';
	int i = 1;
	while (i <= number.size()) {
		if (temp < 2) {
			temp = temp * 10 + number[i] - '0';
			i++;
		}
		else
			break;
	}
	// Repeatedly divide divisor with temp. After  
	// every division, update temp to include one  
	// more digit. 
	while (i <= number.size()) {
		result += temp / 2 + '0';
		temp = (temp % 2) * 10 + number[i] - '0';
		i++;
	}
	// If divisor is greater than number 
	if (result.size() == 0)
		return "0";
	return result;
}

string Sum(string str1, string str2)
{
	// Before proceeding further, make sure length 
	// of str2 is larger. 
	if (str1.length() > str2.length())
		swap(str1, str2);
	// Take an empty string for storing result 
	string str = "";
	// Calculate lenght of both string 
	int n1 = str1.length(), n2 = str2.length();
	// Reverse both of strings 
	reverse(str1.begin(), str1.end());
	reverse(str2.begin(), str2.end());
	int carry = 0;
	for (int i = 0; i < n1; i++)
	{
		// Do school mathematics, compute sum of 
		// current digits and carry 
		int sum = ((str1[i] - '0') + (str2[i] - '0') + carry);
		str.push_back(sum % 10 + '0');
		// Calculate carry for next step 
		carry = sum / 10;
	}

	// Add remaining digits of larger number 
	for (int i = n1; i < n2; i++)
	{
		int sum = ((str2[i] - '0') + carry);
		str.push_back(sum % 10 + '0');
		carry = sum / 10;
	}
	// Add remaining carry 
	if (carry)
		str.push_back(carry + '0');
	// reverse resultant string 
	reverse(str.begin(), str.end());
	return str;
}

string Mult2(string number)
{
	return Sum(number, number);
}


string twoComplement(string binary)
{
	// index: tro den 1 bit cuoi cung
	// Bu 2: 
	// + ~ cac bit tu trai qua den (index - 1)
	// + Giu nguyen cac bit tu index tro di

	int len = binary.length();
	bool flag = false;
	string result;
	for (int i = len - 1; i >= 0; i--)
	{
		int c = binary[i];
		if (flag)
		{
			// Dao bit
			c = (c == '0') ? '1' : '0';
		}
		else if (c == '1')
		{
			// Gap bit 1 dau tien, danh dau
			flag = true;
		}
		// Them vao bit hien tai vao truoc
		result = (char)c + result;
	}
	// Them bit 1 phia truoc cho du 128 bit
	string addition_bits(128 - len, '1');
	result = addition_bits + result;
	return result;
}


string DecToBinary(string number)
{
	string result = "";
	char bit;
	while (number != "0")
	{
		// Lay chu so cuoi chia 2
		bit = (number.back() - '0') % 2;
		// Them vao truoc ket qua
		result = (char)(bit + '0') + result;
		// Chia 2 toi khi = 0
		number = Div2(number);
	}
	return result;
}

string DecToTwosComplementBinary(string number)
{
	string result;
	// Neu la so am
	if (number[0] == '-')
	{
		// Loai bo dau, chuyen sang binary
		// Lay bu 2 cua no
		string pos_dec = number.substr(1);
		string pos_bin = DecToBinary(pos_dec);
		result = twoComplement(pos_bin);
	}
	else
	{
		result = DecToBinary(number);
	}
	return result;
}

string TwosComplementBinaryToDec(string binary)
{
	string result;

	// So am khi bit thu 128 la 1
	if (binary[0] == '1' && binary.length() == 128)
	{
		string pos_bin = twoComplement(binary);
		string pos_dec = BinaryToDec(pos_bin);
		result = '-' + pos_dec;
		return result;
	}
	result = BinaryToDec(binary);
	return result;
}

string BinaryToDec(string binary)
{
	string result = "0";
	string tmp = "1"; // 2 ^ 0
	int len = binary.length() - 1;
	while (len >= 0)
	{
		// Cong tu trai qua phai
		// + bit * (2^0) -> (2^(len - 1))
		if (binary[len] == '1')
		{
			result = Sum(result, tmp);
		}
		tmp = Mult2(tmp);
		len--;
	}
	return result;
}


string HexToBinary(string hex)
{
	string binary;
	int len = hex.length();
	for (int i = len - 1; i >= 0; i--) {
		binary = codeBinaryOfHex(hex[i]) + binary;
	}
	return binary;
}

string BinaryToHex(string binary)
{
	string result, get_4_lastbits;
	string tmp = binary;
	while (tmp != "")
	{
		int len = tmp.length();
		// Lay tung chuoi 4 bit cuoi

		// Neu du 4 bit
		if (len >= 4)
		{
			get_4_lastbits = tmp.substr(len - 4);	// Lay 4 bit cuoi
			tmp = tmp.substr(0, len - 4);			// Cat di 4 ki tu cuoi
		}
		// Neu khong du 4 bit
		else
		{
			// Them 0 vao truoc chuoi tmp cho du 4 bit
			string addition_zero(4 - len, '0');
			get_4_lastbits = addition_zero + tmp;
			tmp = "";
		}
		result = codeHexOfBinary(get_4_lastbits) + result;		// Them vao truoc result
	}
	return result;
}


string codeBinaryOfHex(char hex)
{
	switch (hex)
	{
	default: return "0000";
	case '1': return "0001";
	case '2': return "0010";
	case '3': return "0011";
	case '4': return "0100";
	case '5': return "0101";
	case '6': return "0110";
	case '7': return "0111";
	case '8': return "1000";
	case '9': return "1001";
	case 'A': return "1010";
	case 'B': return "1011";
	case 'C': return "1100";
	case 'D': return "1101";
	case 'E': return "1110";
	case 'F': return "1111";
	}
}

string codeHexOfBinary(string binary)
{
	if (binary == "0001") return "1";
	else if (binary == "0010") return "2";
	else if (binary == "0011") return "3";
	else if (binary == "0100") return "4";
	else if (binary == "0101") return "5";
	else if (binary == "0110") return "6";
	else if (binary == "0111") return "7";
	else if (binary == "1000") return "8";
	else if (binary == "1001") return "9";
	else if (binary == "1010") return "A";
	else if (binary == "1011") return "B";
	else if (binary == "1100") return "C";
	else if (binary == "1101") return "D";
	else if (binary == "1110") return "E";
	else if (binary == "1111") return "F";
	else return "0";
}













