#include "QInt.h"

QInt::QInt()
{
	this->sign = 0;
	for (int i = 0; i < 4; i++) {
		this->arrayBits[i] = 0;
	}
}

void QInt::SetBit(int pos, int bit)
{
	if (pos % 32 == 0)
		this->arrayBits[pos / 32 - 1] = (bit << 0) | this->arrayBits[pos / 32 - 1];
	else
		this->arrayBits[pos / 32] = (bit << (32 - pos % 32)) | this->arrayBits[pos / 32];
}

void QInt::DecToQInt(string number)
{
	if (number.at(0) == '-') {
		this->sign = -1;
		number.at(0) = '0';
		string temp = DecToBinary(number);
		temp = twoComplement(temp);
		this->BinaryToQInt(temp);
		return;
	}
	else if (number.compare("0") == 0) {
		this->sign = 0;
		return;
	}
	this->sign = 1;
	int i = 128;
	int len = number.size() - 1;
	int bit;
	while (number != "0") {
		bit = (number[len] - 48) % 2;
		this->SetBit(i, bit);
		number = Div2(number);
		i--;
		len = number.size() - 1;
	}
}

void QInt::BinaryToQInt(string binary)
{
	int i = 128;
	int len = binary.size() - 1;
	while (len >= 0) {
		this->SetBit(i, (binary[len] - 48) % 2);
		i--;
		len--;
	}
}

void QInt::HexToQInt(string hex) {
	string binary = HexToBinary(hex);
	this->BinaryToQInt(binary);
}

string QInt::QIntToBinary()
{
	string binary;
	int pos = 0;
	string temp;
	for (int i = 0; i < 4; i++) {
		if (this->arrayBits[i] != 0) {
			temp = DecToBinary(to_string(this->arrayBits[i]));
			temp.replace(0, 96, ""); //Delete '0', only get 32 bit last
			binary.insert(pos, temp);
			pos = binary.size();
		}
	}
	return binary;
}

string QInt::QIntToDec()
{
	if (this->isZero()) {
		return "0";
	}
	string binary = this->QIntToBinary();
	string Decimal = BinaryToDec(binary, true);
	if (this->sign == -1) {
		Decimal.insert(0, "-");
	}
	return Decimal;
}

string QInt::QIntToHex() {
	string binary = this->QIntToBinary();
	return BinaryToHex(binary);
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
	int i = 127;
	while (i >= 0) {
		// Comput sum of last digits and carry
		sum += ((i >= 0) ? (((this->arrayBits[i / 32] & (1 << 31 - i % 32)) != 0) ? 1 : 0) : 0);
		sum += ((i >= 0) ? (((n.arrayBits[i / 32] & (1 << 31 - i % 32)) != 0) ? 1 : 0) : 0);
		// If current digit sum is 1 or 3, add 1 to result 
		result.SetBit(i + 1, sum % 2);
		// Compute carry 
		sum /= 2;
		// Move to next digits 
		i--;
	}
	//Check result is nagative or postive
	unsigned int nagative = pow(2, 31);
	if (result.arrayBits[0] >= nagative) {
		result.sign = -1;
	}
	else if (!result.isZero()) {
		result.sign = 1;
	}
	return result;
}

QInt QInt::operator-(QInt n)
{
	//Convert from n to -n
	string binary = n.QIntToBinary();
	binary = twoComplement(binary);
	string numberNagative = BinaryToDec(binary, false);
	QInt n1;
	n1.DecToQInt(numberNagative); // -n
	return *this + n1;
}

void QInt::operator=(QInt n)
{
	this->sign = n.sign;
	for (int i = 0; i < 4; i++) {
		this->arrayBits[i] = n.arrayBits[i];
	}
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

bool isSmaller(string str1, string str2)
{
	// Calculate lengths of both string 
	int n1 = str1.length(), n2 = str2.length();
	if (n1 < n2)
		return true;
	if (n2 > n1)
		return false;
	for (int i = 0; i < n1; i++)
	{
		if (str1[i] < str2[i])
			return true;
		else if (str1[i] > str2[i])
			return false;
	}
	return false;
}

string findDiff(string str1, string str2)
{
	// Before proceeding further, make sure str1 
	// is not smaller 
	if (isSmaller(str1, str2))
		swap(str1, str2);
	// Take an empty string for storing result 
	string str = "";
	// Calculate lengths of both string 
	int n1 = str1.length(), n2 = str2.length();
	int diff = n1 - n2;
	// Initially take carry zero 
	int carry = 0;
	// Traverse from end of both strings 
	for (int i = n2 - 1; i >= 0; i--)
	{
		// Do school mathematics, compute difference of 
		// current digits and carry 
		int sub = ((str1[i + diff] - '0') -
			(str2[i] - '0') -
			carry);
		if (sub < 0)
		{
			sub = sub + 10;
			carry = 1;
		}
		else
			carry = 0;
		str.push_back(sub + '0');
	}

	// subtract remaining digits of str1[] 
	for (int i = n1 - n2 - 1; i >= 0; i--)
	{
		if (str1[i] == '0' && carry)
		{
			str.push_back('9');
			continue;
		}
		int sub = ((str1[i] - '0') - carry);
		if (i > 0 || sub > 0) // remove preceding 0's 
			str.push_back(sub + '0');
		carry = 0;
	}
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

string BinaryToDec(string binary, bool twoCompl)
{
	binary = standardizedBinary(binary);
	string Decimal = "0";
	for (int i = 127; i >= 0; i--) {
		if (i == 0 && binary.at(i) == '1' && twoCompl == true) {
			Decimal = findDiff(Decimal, twoHat(127 - i));
		}
		else if (binary.at(i) == '1') {
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

string HexToBinary(string hex)
{
	string binary;
	for (int i = hex.size() - 1; i >= 0; i--) {
		binary.insert(0, codeBinaryOfHex(hex.at(i)));
	}
	return binary;
}






