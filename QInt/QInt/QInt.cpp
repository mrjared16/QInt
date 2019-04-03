#include "QInt.h"

QInt::QInt()
{
	this->sign = ZERO;
	for (int i = 0; i < 4; i++) {
		this->arrayBits[i] = 0;
	}
}

// pos: 1 - 128
// bit: 0 / 1

void QInt::setBit(int pos, int bit)
{
	pos -= 1;
	this->arrayBits[3 - pos / 32] |= (bit << pos % 32);
}

int QInt::getBit(int pos)
{
	pos -= 1;
	int tmp = this->arrayBits[3 - pos / 32] & (1 << pos % 32);
	return (tmp == 0) ? 0 : 1;
}

bool QInt::isZero()
{
	for (int i = 0; i < 4; i++) {
		if (this->arrayBits[i] != 0) {
			return false;
		}
	}
	return true;
}

QInt QInt::operator+(QInt n)
{
	QInt result;
	int sum = 0;
	int i = 1;
	for (int i = 1; i <= 128; i++) {
		// Comput sum of last digits and carry
		sum += this->getBit(i);
		sum += n.getBit(i);
		// If current digit sum is 1 or 3, add 1 to result 
		result.setBit(i, sum % 2);
		// Compute carry 
		sum /= 2;
	}
	//Check result is nagative or postive
	result.sign = (result.getBit(128) == 0) ? POSITIVE : NEGATIVE;
	return result;
}

QInt QInt::operator-(QInt n)
{
	//Convert from n to -n
	QInt n1;
	n1.stringToQInt(twoComplement(n.toBin()), 2); // -n
	return *this + n1;
}

QInt QInt::operator*(QInt n)
{
	QInt A;
	for (int i = 1; i <= 128; i++) {
		if (n.getBit(i) == 1) {
			QInt temp;
			for (int j = 128; j - (i - 1) >= 1; j--)
			{
				temp.setBit(j, this->getBit(j - (i - 1)));
			}
			A = A + temp;
		}
	}
	return A;
}

QInt QInt::operator/(QInt n)
{
	QInt A;
	QInt Q;
	QInt zero;
	QInt M;
	if (this->sign == NEGATIVE) {
		Q = zero - *this;
	}
	else
		Q = *this;
	if (n.sign == NEGATIVE) {
		M = zero - n;
	}
	else
		M = n;
	int k = 128;
	int bit,bit1,bit2;
	while (k > 0) {
		string d = Q.toBin();
		bit = Q.getBit(127);
		bit1 = Q.getBit(128);
		bit2 = A.getBit(127);
		A = A << 1;
		Q = Q << 1;
		if (bit2 == 1) {
			A.arrayBits[0] |= (1 << 31);
		}
		else
			A.arrayBits[0] &= ~(1 << 31);
		if (bit == 1) {
			Q.arrayBits[0] |= (1 << 31);
		}
		else 
			Q.arrayBits[0] &= ~(1 << 31);
		if (bit1 == 1) {
			A.arrayBits[3] |= 1;
		}
		else
			A.arrayBits[3] &= ~1;
		d = Q.toBin();
		A = A - M;
		if (A.sign == NEGATIVE) {
			A = A + M;
			Q.arrayBits[3] &= ~1;
		}
		else
			Q.arrayBits[3] |= 1;
		k--;
	}
	Q.sign = (Q.getBit(128) == 0) ? POSITIVE : NEGATIVE;
	if (this->sign != n.sign) {
		Q = zero - Q;
	}
	return Q;
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
	result.sign = (result.getBit(128) == 0) ? POSITIVE : NEGATIVE;
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
	result.sign = (result.getBit(128) == 0) ? POSITIVE : NEGATIVE;
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
	result.sign = (result.getBit(128) == 0) ? POSITIVE : NEGATIVE;
	return result;
}

QInt QInt::operator>>(unsigned int k)
{
	QInt result;
	result.setBit(128, this->getBit(128));
	for (int i = 1; i + k <= 128; i++)
	{
		result.setBit(i, this->getBit(i + k));	// 1 + k -> 128
	}
	result.sign = (result.getBit(128) == 0) ? POSITIVE : NEGATIVE;
	return result;
}

QInt QInt::operator<<(unsigned int k)
{
	// Luu y dich trai, result luu bit dau roi dich cac bit con lai
	QInt result;
	result.setBit(128, this->getBit(128));
	//Bat dau tu 2, bo qua bit 1
	for (int i = 127; i - k >= 1; i--)	
	{
		result.setBit(i, this->getBit(i - k));	// 2 -> 128
	}
	result.sign = (result.getBit(128) == 0) ? POSITIVE : NEGATIVE;
	return result;
}

QInt QInt::operator~()
{
	QInt result;
	for (int i = 1; i <= 128; i++)
	{
		result.setBit(i, (this->getBit(i) == 0) ? 1 : 0);
	}
	result.sign = (result.getBit(128) == 0) ? POSITIVE : NEGATIVE;
	return result;
}

// Xoay trai 1 bit
QInt QInt::rol()
{
	QInt result;
	// bit cuoi la bit dau cua bit ban dau
	result.setBit(1, this->getBit(128));
	for (int i = 128; i - 1 >= 1; i--)
	{
		result.setBit(i, this->getBit(i - 1));	// 2 -> 128
	}
	result.sign = (result.getBit(128) == 0) ? POSITIVE : NEGATIVE;
	return result;
}

// Xoay phai 1 bit
QInt QInt::ror()
{
	QInt result;
	// bit dau la bit cuoi cua bit cuoi ban dau
	string tmp = this->toString(2);
	result.setBit(128, this->getBit(1));
	for (int i = 1; i + 1 <= 128; i++)
	{
		// bit moi vi tri i la bit cu vi tri truoc no 1 don vi
		result.setBit(i, this->getBit(i + 1));
	}
	result.sign = (result.getBit(128) == 0) ? POSITIVE : NEGATIVE;
	return result;
}

void QInt::operator=(QInt n)
{
	this->sign = n.sign;
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
		this->sign = NEGATIVE;
		number.at(0) = '0';
		string temp = DecToBinary(number);
		temp = twoComplement(temp);
		this->binToQInt(temp);
		return;
	}
	else if (number.compare("0") == 0) {
		this->sign = ZERO;
		return;
	}
	this->sign = POSITIVE;
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
	if (this->isZero()) {
		return "0";
	}
	string binary = this->toBin();
	if (this->sign == NEGATIVE) {
		binary = twoComplement(binary);
	}
	string Decimal = BinaryToDec(binary);
	if (this->sign == NEGATIVE) {
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
	while (Decimal.at(0) == '0') {
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








