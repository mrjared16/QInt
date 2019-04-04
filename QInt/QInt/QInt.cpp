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
	// Tat bit 1
	if (bit == 0)
	{
		this->arrayBits[3 - pos / 32] &= ~(1 << (pos % 32)); //Sua cho nay lai nha! bit => 1, vay moi tat bit duoc
	}
	else // Bat bit
		this->arrayBits[3 - pos / 32] |= (1 << (pos % 32));

}

QInt QInt::shiftLeft(unsigned int k)
{
	QInt temp = *this;
	int bit = this->getBit(128 - k); //Luu lai bit 128 sau khi dich trai = vi tri (128 - k) truoc khi dich trai
	temp = temp << k;
	temp.setBit(128, bit); //cai dat bit cho bit 128 sau khi dich trai da duoc luu o tren
	return temp;
}

QInt QInt::changeSign()
{
	QInt zero;
	return zero - *this; // a = 0 - a = -a
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

	// Cong bit lan luot tu bit 1 den bit 128
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
		if (n.getBit(i) == 1) {
			QInt temp;
			temp = this->shiftLeft(i - 1);
			A = A + temp;
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
		Q.changeSign();
	}
	if (n.isNegative()) {
		M.changeSign();
	}
	int k = 128;
	int bit;
	while (k > 0) {
		//Dich trai [A,Q]
		bit = Q.getBit(128); //Luu lai bit 128 cua Q truoc khi dich trai
		A = A.shiftLeft(1); 
		Q = Q.shiftLeft(1);
		A.setBit(1, bit); //Cai dat bit cho bit 1 cua A = bit 128 cua Q da duoc luu o tren
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
		Q.changeSign();
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
	if (result.getSignBit() == negative_sign_bit)
	{
		// Gan cac bit 127 -> 128 - k + 1 = 1
		for (int i = 127; i >= 128 - k + 1; i--)
		{
			result.setBit(i, negative_sign_bit);
		}
	}
	// Dich cac bit con lai: 128 - k -> 1
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
		result.setBit(i, this->getBit(i - 1));	// 2 -> 128
	}
	
	return result;
}
// Xoay phai 1 bit
QInt QInt::ror()
{
	QInt result;
	// bit dau la bit cuoi cua bit cuoi ban dau
	result.setBit(128, this->getBit(1));
	// Dich cac bit con lai
	for (int i = 128; i - 1 >= 1; i--)
	{
		result.setBit(i - 1, this->getBit(i));
	}
	
	return result;
}


void QInt::operator=(QInt n)
{
	for (int i = 0; i < 4; i++) {
		this->arrayBits[i] = n.arrayBits[i];
	}
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
		while (result[0] == '0')
		{
			result.erase(0, 1);
		}
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
	if (number.at(0) == '-') {
		number.at(0) = '0';
		string temp = DecToBinary(number);
		temp = twoComplement(temp);
		this->binToQInt(temp);
		return;
	}
	else if (number.compare("0") == 0) {
		return;
	}
	int i = 1;
	int len = number.size() - 1;
	int bit;
	while (number != "0") {
		bit = (number[len] - 48) % 2;
		this->setBit(i, bit);
		number = Div2(number);
		i++;
		len = number.size() - 1;
	}
}

void QInt::binToQInt(string binary)
{
	int i = 1;
	int len = binary.size() - 1;
	while (len >= 0) {
		this->setBit(i, (binary[len] - 48) % 2);
		i++;
		len--;
	}
}

void QInt::hexToQInt(string hex) {
	string binary = HexToBinary(hex);
	this->binToQInt(binary);
}


string QInt::toBin()
{
	string binary;
	binary = standardizedBinary(binary);
	string temp;
	for (int i = 0; i < 4; i++) {
		if (this->arrayBits[i] != 0) {
			temp = DecToBinary(to_string(this->arrayBits[i]));
			temp.replace(0, 128 - 32, ""); //Delete '0', only get 32 bit last
			binary.replace(i * 32, 32, temp);
		}
	}
	return binary;
}

string QInt::toDec()
{
	string binary = this->toBin();
	if (this->isNegative()) {
		binary = twoComplement(binary);
	}
	string Decimal = BinaryToDec(binary);
	if (this->isNegative()) {
		Decimal.insert(0, "-");
	}
	return Decimal;
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

string multiply(string num1, string num2)
{
	int n1 = num1.size();
	int n2 = num2.size();
	if (n1 == 0 || n2 == 0)
		return "0";
	// will keep the result number in vector 
	// in reverse order 
	vector<int> result(n1 + n2, 0);
	// Below two indexes are used to find positions 
	// in result.  
	int i_n1 = 0;
	int i_n2 = 0;
	// Go from right to left in num1 
	for (int i = n1 - 1; i >= 0; i--)
	{
		int carry = 0;
		int n1 = num1[i] - '0';
		// To shift position to left after every 
		// multiplication of a digit in num2 
		i_n2 = 0;
		// Go from right to left in num2              
		for (int j = n2 - 1; j >= 0; j--)
		{
			// Take current digit of second number 
			int n2 = num2[j] - '0';
			// Multiply with current digit of first number 
			// and add result to previously stored result 
			// at current position.  
			int sum = n1 * n2 + result[i_n1 + i_n2] + carry;
			// Carry for next iteration 
			carry = sum / 10;
			// Store result 
			result[i_n1 + i_n2] = sum % 10;
			i_n2++;
		}
		// store carry in next cell 
		if (carry > 0)
			result[i_n1 + i_n2] += carry;
		// To shift position to left after every 
		// multiplication of a digit in num1. 
		i_n1++;
	}
	// ignore '0's from the right 
	int i = result.size() - 1;
	while (i >= 0 && result[i] == 0)
		i--;
	// If all were '0's - means either both or 
	// one of num1 or num2 were '0' 
	if (i == -1)
		return "0";
	// generate the result string 
	string s = "";
	while (i >= 0)
		s += to_string(result[i--]);
	return s;
}

string twoHat(int num)
{
	string result = "1";
	for (int i = 1; i <= num; i++) {
		result = multiply(result, "2");
	}
	return result;
}

string standardizedBinary(string binary)
{
	string temp = binary;
	while (temp.size() < 128) {
		temp.insert(0, "0");
	}
	return temp;
}

string twoComplement(string binary)
{
	binary = standardizedBinary(binary);
	for (int i = 0; i < 128; i++) {
		if (binary.at(i) == '1') {
			binary.at(i) = '0';
		}
		else if (binary.at(i) == '0') {
			binary.at(i) = '1';
		}
	}
	for (int i = 127; i >= 0; i--) {
		if (binary.at(i) == '1')
			binary.at(i) = '0';
		else if (binary.at(i) == '0') {
			binary.at(i) = '1';
			break;
		}
	}
	return binary;
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

string HexToBinary(string hex)
{
	string binary;
	for (int i = hex.size() - 1; i >= 0; i--) {
		binary.insert(0, codeBinaryOfHex(hex.at(i)));
	}
	return binary;
}

string DecToBinary(string number)
{
	string binary = "";
	binary = standardizedBinary(binary);
	int i = 127;
	int bit;
	int len = number.size() - 1;
	while (number != "0") {
		bit = (number[len] - 48) % 2;
		if (binary.at(i) == '1' || bit == 1) {
			binary.at(i) = '1';
		}
		number = Div2(number);
		len = number.size() - 1;
		i--;
	}
	return binary;
}

string BinaryToDec(string binary)
{
	binary = standardizedBinary(binary);
	string Decimal = "0";
	for (int i = 127; i >= 0; i--) {
		if (binary.at(i) == '1') {
			Decimal = Sum(Decimal, twoHat(127 - i));
		}
	}
	while (Decimal.at(0) == '0' && Decimal.compare("0") != 0) {
		Decimal.replace(0, 1, "");
	}
	return Decimal;
}

string BinaryToHex(string binary)
{
	binary = standardizedBinary(binary);
	int i = 127;
	string code;
	string result;
	while (i >= 0) {
		for (int j = 0; j < 4; j++) { //Get 4 bit last in binary string
			code += binary.at(i);
			i--;
		}
		reverse(code.begin(), code.end());
		result.insert(0, codeHexOfBinary(code));
		code = ""; //initialization code string
	}
	while (result.at(0) == '0') { //Delete 0 in front
		result.replace(0, 1, "");
	}
	return result;
}








