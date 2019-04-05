#include "XuLyInOut.h"

int main(int argv, char **arg)
{
	int input_index = 1;
	int output_index = 2;

	XuLyInOut::run(arg[input_index], arg[output_index]);
	return 0;
}
