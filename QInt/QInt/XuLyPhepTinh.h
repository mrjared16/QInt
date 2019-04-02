#pragma once
#include <string>
using namespace std;
class XuLyPhepTinh
{
public:
	XuLyPhepTinh(string input);
	string result();
private:
	string operand1, operand2;
	int base1, base2;
	string operator_str;
};

