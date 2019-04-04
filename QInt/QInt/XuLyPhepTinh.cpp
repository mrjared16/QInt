#include "XuLyPhepTinh.h"
#include "QInt.h"

XuLyPhepTinh::XuLyPhepTinh(string input)
{
	string delimiter = " "; // phep toan, toan hang cach nhau boi " "

	int start = 0;
	int end, i = 0;

	// Tien hanh doc token (toan hang, toan tu, base) vao mang
	// Toi da co 4 token
	string token[4];
	end = input.find(delimiter, start);
	while (end != string::npos)
	{
		string tmp = input.substr(start, end - start);
		token[i++] = input.substr(start, end - start);
		start = end + 1;
		end = input.find(delimiter, start);
		if (i >= 3)
			break;
	}
	// Token cuoi la phan con lai
	token[i++] = input.substr(start);
	// Tien hanh xu ly thong tin luu vao cac bien
	// Co 3 truong hop: 

	// 3 token: 
	// + base operator operand: toan tu mot ngoi: ~, ror, rol
	// + base1 base2 operand: chuyen doi co so
	if (i == 3)
	{
		base1 = stoi(token[0]);
		char c;

		if (token[1] == "~" ||
			token[1] == "rol" ||
			token[1] == "ror")
		{
			// token thu 2 la operator
			operator_str = token[1];
			base2 = 0;	// Khong co base2
		}
		else {
			// token thu 2 la base
			base2 = stoi(token[1]);
			operator_str = "";	// Khong co operator
		}

		operand1 = token[2];	// operand la token thu 3
		operand2 = "";	// Khong co operand2
		
	}

	// 4 token: base operand1 operator operand2
	else if (i == 4)
	{
		base1 = stoi(token[0]);
		base2 = 0;	// Khong co base2

		operand1 = token[1];
		operand2 = token[3];

		operator_str = token[2];
	}

}

string XuLyPhepTinh::result()
{
	QInt opr1, opr2;
	string result = "";
	
	// Chuyen doi co so
	if (operator_str == "")
	{
		opr1.stringToQInt(operand1, base1); // Chuyen sang QInt
		result = opr1.toString(base2);	// Chuyen sang co so can chuyen
	}
	else
	{
		QInt tmp; 
		// Tinh toan luu ket qua vao tmp 
		
		// Cac toan mot ngoi: rol, ror, ~
		if (operator_str == "~" ||
			operator_str == "rol" ||
			operator_str == "ror") 
		{
			opr1.stringToQInt(operand1, base1);	// Chuyen sang QInt

			// Tinh ket qua
			if (operator_str == "~")
				tmp = ~opr1;
			if (operator_str == "rol")
				tmp = opr1.rol();
			if (operator_str == "ror")
				tmp = opr1.ror();
		}
		else
		{
			// Cac toan tu 2 ngoi con lai

			// Tao opr1
			opr1.stringToQInt(operand1, base1);
			
			// Truong hop >>, << 
			// operand2 la int
			if (operator_str == ">>") 
			{
				int k = stoi(operand2); 
				tmp = opr1 >> k;
			}
			else if(operator_str == "<<")
			{
				int k = stoi(operand2);
				tmp = opr1 << k;
			}
			else
			{
				// Cac truong hop con lai operand2 la QInt
				opr2.stringToQInt(operand2, base1);
				char opt = operator_str[0]; 
				// opt domain: +, -, *, /, &, |, ^, =

				// Chon phep tinh, tinh va luu vao tmp:
				switch (opt)
				{
				case '+':
					tmp = opr1 + opr2;
					break;
				case '-':
					tmp = opr1 - opr2;
					break;
				case '*':
					tmp = opr1 * opr2;
					break;
				case '/':
					tmp = opr1 / opr2;
					break;
				case '&':
					tmp = opr1 & opr2;
					break;
				case '|':
					tmp = opr1 | opr2;
					break;
				case '^':
					tmp = opr1 ^ opr2;
					break;
					/*case '=':
						tmp = opr1 + opr2;
						break;*/

				}
			}
		}
		// Sau do chuyen thanh chuoi luu vao result de return
		result = tmp.toString(base1); // Chuyen sang co so cu
	}

	return result;
}
