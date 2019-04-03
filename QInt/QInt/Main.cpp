//#include "XuLyInOut.h"
#include "QInt.h"

int main(int argv, char **arg)
{
	/*int input_index = 1;
	int output_index = 2;

	XuLyInOut::run(arg[input_index], arg[output_index]);*/
	string a = "65364821948532146";
	QInt b;
	b.stringToQInt(a, 10);
	string c = "651342711921841";
	QInt d;
	d.stringToQInt(c, 10);
	QInt e = b * d;
	string i = e.toString(10);


	return 0;
}