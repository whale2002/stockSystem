#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

int main()
{
	ofstream outfile("writedemo.csv", ios::out);
	if(!outfile) {
		cout << "打开文件失败!" << endl;
		exit(1);
	}
	for(int i = 0; i < 3; i++) {
		outfile << 12 << ",";
		outfile << 13 << ",";
		outfile << 14 << ",";
		outfile << "NaN" << endl;
	}
	outfile.close();
	return 0;
}

