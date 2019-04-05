#include "BaseConverter.h"


// Lay gia tri bu hai cua chuoi nhi phan 'binary'
string BaseConverter::getTwosComplement(string binary)
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

// Lay chuoi binary cua chuoi thap phan 'decimal' 
string BaseConverter::getBinOfDec(string decimal)
{
	string result = "";
	char bit;
	while (decimal != "0")
	{
		// Lay chu so cuoi chia 2
		bit = (decimal.back() - '0') % 2;
		// Them vao truoc ket qua
		result = (char)(bit + '0') + result;
		// Chia 2 toi khi = 0
		decimal = BigIntCalculator::Div2(decimal);
	}
	return result;
}

// Lay chuoi thap phan cua chuoi nhi phan 'binary' 
string BaseConverter::getDecOfBin(string binary)
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
			result = BigIntCalculator::Sum(result, tmp);
		}
		tmp = BigIntCalculator::Mult2(tmp);
		len--;
	}
	return result;
}

// Lay chuoi nhi phan dang bu hai tu chuoi thap phan 'decimal'
string BaseConverter::getTwosComplementBinOfDec(string decimal)
{
	string result;
	// Neu la so am
	if (decimal[0] == '-')
	{
		// Loai bo dau, chuyen sang binary
		// Lay bu 2 cua no
		string pos_dec = decimal.substr(1);
		string pos_bin = getBinOfDec(pos_dec);
		result = getTwosComplement(pos_bin);
	}
	else
	{
		result = getBinOfDec(decimal);
	}
	return result;
}

// Lay chuoi thap phan cua chuoi nhi phan duoi dang bu hai 'binary'
string BaseConverter::getDecOfTwosComplementBin(string binary)
{
	string result;

	// So am khi bit thu 128 la 1
	if (binary[0] == '1' && binary.length() == 128)
	{
		string pos_bin = getTwosComplement(binary);
		string pos_dec = getDecOfBin(pos_bin);
		result = '-' + pos_dec;
		return result;
	}
	result = getDecOfBin(binary);
	return result;
}


// Lay chuoi nhi phan cua chuoi he thap luc 'hex' 
string BaseConverter::getBinOfHex(string hex)
{
	string binary;
	int len = hex.length();
	for (int i = len - 1; i >= 0; i--) {
		binary = codeBinOfHex(hex[i]) + binary;
	}
	return binary;
}

// Lay chuoi he thap luc cua chuoi nhi phan 'binary' 
string BaseConverter::getHexOfBin(string binary)
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
		result = codeHexOfBin(get_4_lastbits) + result;		// Them vao truoc result
	}
	return result;
}

// Lay dang nhi phan 4 bit cua he thap luc phan
string BaseConverter::codeBinOfHex(char hex)
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

// Lay ki tu Hex tu chuoi nhi phan 4 bit
string BaseConverter::codeHexOfBin(string binary)
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










