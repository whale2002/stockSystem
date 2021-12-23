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

//https://blog.csdn.net/chenlin41204050/article/details/78429437
//https://blog.csdn.net/sinat_38792591/article/details/100190990?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~default-0.no_search_link&spm=1001.2101.3001.4242.1

