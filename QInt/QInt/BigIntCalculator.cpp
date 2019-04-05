#include "BigIntCalculator.h"

// Tra ve tong hai chuoi so thap phan 'str1', 'str2'
string BigIntCalculator::Sum(string str1, string str2)
{
	//Them chu so 0 vao dau chuoi nao ngan hon de do dai hai chuoi bang nhau
	int len1 = str1.length();
	int len2 = str2.length();

	if (len1 > len2) {
		string addition_0_digit(len1 - len2, '0');
		str2 = addition_0_digit + str2;
	}
	else if (len2 > len1) {
		string addition_0_digit(len2 - len1, '0');
		str1 = addition_0_digit + str1;
	}
	string result = "";
	int carry = 0;
	//Thuc hien nhu phep cong binh thuong, lay chu so cuoi cua chuoi 1 va chuoi 2 truoc roi cong lai voi nhau
	//va tiep tuc lay chu so ke cuoi cua chuoi 1 va chuoi 2 cong lai voi nhau. Cu nhu the cho den khi
	//vi tri xet vuot qua dau chuoi thi dung lai.
	int len = str1.length(); // len1 = len2 = len
	for (int i = len - 1; i >= 0; i--) {
		int sum = (str1[i] - '0') + (str2[i] - '0') + carry;
		result = (char)(sum % 10 + '0') + result; //Sau khi cong hai chu so lai thi chi lay hang don vi
		carry = sum / 10; //sau moi lan cong hai chu so se cap nhat lai so nho
	}
	//Sau khi cong xong ta them so nho vao dau chuoi
	if (carry != 0) {
		result = (char)(carry + '0') + result;
	}
	return result;

}

// Tra ve tich chuoi thap phan ('number' * 2)
string BigIntCalculator::Mult2(string number)
{
	return Sum(number, number);
}

// Tra ve thuong phep chia lay phan nguyen chuoi thap phan ('number' / 2)
string BigIntCalculator::Div2(string number)
{
	string result;
	int i = 1;
	bool flag = false; //kiem tra xem temp co lon hon 2 hay chua?
	int temp = number[0] - '0'; //Lay chu so dau tien tinh tu ben trai qua cua so chia
	while (i <= number.size()) {
		//Neu temp chua lon hon 2 thi tiep tuc lay chu so tiep theo cho den khi nao lon hon 2 
		if (temp < 2 && flag == false) {
			temp = temp * 10 + (number[i] - '0');
			i++;
			continue;
		}
		else {
			//Neu flag = true tuc la temp dang lon hon 2
			flag = true;
		}
		//Lay temp chia 2 de luu lai ket qua
		result += (temp / 2 + '0');

		//Cap nhat lai temp bang cach lay so du va ha xuong chu so ke tiep
		temp = (temp % 2) * 10 + (number[i] - '0');
		i++;
	}
	//Neu so bi chia lon hon so chia thi ra thuong = 0
	if (result.size() == 0)
		return "0";
	return result;
}