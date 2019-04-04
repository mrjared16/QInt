#include "XuLyInOut.h"
#include "QInt.h"

int main(int argv, char **arg)
{
	/*int input_index = 1;
	int output_index = 2;

	XuLyInOut::run(arg[input_index], arg[output_index]);*/
	string a = "788323282258381939236275890776";
	QInt b;
	b.stringToQInt(a, 10);
	string c = "-27388934683056007097503944712";
	QInt d;
	d.stringToQInt(c, 10);
	QInt e;
	e = b / d;
	string y = e.toString(10);
	return 0;
}
