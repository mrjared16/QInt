#include "XuLyInOut.h"
#include "XuLyPhepTinh.h"

#include <fstream>
#include <iostream>

// Doc file input, truyen phep toan vao XuLyPhepTinh, tinh toan va ghi vao file output
void XuLyInOut::run(string input_name, string output_name)
{
	string buffer;
	ifstream f_inp;
	f_inp.open(input_name);
	// Mo file input va doc
	//das
	if (!f_inp.good())
	{
		// Mo khong duoc: Sai duong dan hoac file ko ton tai
		cout << "Mo file " << input_name << " khong thanh cong!" << endl;
		return;
	}
	// Neu mo thanh cong

	// Tao file output
	ofstream f_out;
	f_out.open(output_name);

	if (!f_out.good())
	{
		// Mo khong duoc: Sai duong dan hoac file ko ton tai
		cout << "Xay ra loi khi tao file " << output_name << "!" << endl;
		return;
	}

	// Bat dau doc ghi file
	while (getline(f_inp, buffer))	// Doc tung dong cua file
	{
		XuLyPhepTinh services(buffer);	// Cho vao class xu li phep tinh
		f_out << services.result() << endl;	// Xuat ket qua vao file output
	}

	// Dong file
	f_inp.close();
	f_out.close();
}