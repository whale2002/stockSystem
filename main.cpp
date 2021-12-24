#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

// 股票涨跌数据 
typedef struct changeData {
	string stockCode;    //股票代码
	string shortName;    //股票名称 
	string date;         //日期
	string openingPrice; //开盘价
	string closingPrice; //收盘价
	string highestPrice; //最高价
	string lowestPrice;  //最低价
	string volume;       //成交量
	string turnover;     //成交额度
	string turnoverRate; //换手率
	string FZDE; 		 //跌涨额 
	double changePercent;//跌涨幅
	struct changeData* next;
} changeData, *dataLink;
// 股票 
typedef struct {
	string stockCode;      //股票代码 
	string shortName;	   //股票简称 
	string industryCode;   //行业编码 
	string cate1;          //一级门类 
	string cate2;		   //二级门类 
	string exchange;       //上市交易所
	string fullName;	   //公司全称 
	string date;      	   //上市日期 
	string province;       //省份 
	string city;           //城市 
	string legalPerson;    //法人 
	string location;       //地址 
	string website;        //网址 
	string email;          //邮箱 
	string phone;          //电话 
	string majorBusiness;  //主营业务 
	string businessScope;  //经营范围
	string recentInfo[3];  //最近一日开盘价、收盘价和跌涨幅
	dataLink detail;       //股票涨跌数据，首元结点
	double score;          //评分
	double maxChangePercent; //最大涨跌幅
	string maxDate;        //最大涨跌幅对应的日期 
} StockInfo;
// 哈希节点 
typedef struct _hashNode {
	StockInfo node;
	struct _hashNode *next;
} hashNode, *hashList;
// 二叉树节点 
typedef struct TreeNode {
	StockInfo data;
	TreeNode *left;
	TreeNode *right;
} TNode, *BinTree;

vector<hashList> hashTable(97);  //定义哈希表
StockInfo stocks[200];   //存储200支股票的基本信息和具体股票信息 

// 工具函数 
int hashFun(string str);       //哈希函数
bool kmp(string s, string t);  //KMP
string format(int number);     //格式化序号 

// 功能入口 
void readStockBaseInfo(); //读文件并存到数组中 
void createHashTable();   //创建哈希表
void searchBaseInfo();   //基于哈希搜索 
void searchWebsite();    //基于KMP搜索网址
void searchByBST();	     //基于二叉排序树查询
void createBST(BinTree&); //创建二叉排序树
void insertBST(BinTree&, int); //二叉排序树插入节点 
BinTree searchBST(BinTree T, string key); //搜索指定股票代码
void searchByLinkList();   //基于单链表的股票价格信息查询
dataLink getLinkByDate(string date); //根据日期获取链表
void analyseByInsertSort(); //通过插入排序来进行分析
void sortByOpeningPrice(dataLink); //通过开盘价排序 
void sortByClosingPrice(dataLink); //收盘价排序 
void sortByChangePercent(dataLink); //涨跌幅排序 
void analyseByQuickSort(); //通过快排进行分析
void quickSort(vector<StockInfo>&, int, int); //快排 
int partition(vector<StockInfo>& industry, int low, int high);
void analyseBySelectionSort();  //基于简单选择排序的股票价格分析

int main()
{
	readStockBaseInfo();   //读文件，保存数据 
	int choice;

	while(true) {
		cout << "=====================================================" << endl;
		cout << "=====   欢迎使用股票查询与分析系统 V1.0        =====" << endl;
		cout << "=====      请输入您要执行的操作                =====" << endl;
		cout << "=====    1. 基于哈希表的股票基本信息查询       =====" << endl;
		cout << "=====    2. 基于KMP的股票网址查询              =====" << endl;
		cout << "=====    3. 基于二叉排序树的股票基本信息查询   =====" << endl;
		cout << "=====    4. 基于单链表的股票价格信息查询       =====" << endl;
		cout << "=====    5. 基于直接插入排序的股票价格分析     =====" << endl;
		cout << "=====    6. 基于快速排序的股票价格分析         =====" << endl;
		cout << "=====    7. 基于简单选择排序的股票价格分析     =====" << endl;
		cout << "=====    8. 基于Floyd的股票相关性计算          =====" << endl;
		cout << "=====    9. 基于Prim最小生成树的股票基金筛选   =====" << endl;
		cout << "=====    10.基于二部图的股票基金筛选           =====" << endl;
		cout << "=====    11.基于Kruskal最小生成树的股票基金筛选=====" << endl;
		cout << "=====    12.基于二叉排序树的股票基本信息删除   =====" << endl;
		cout << "====================================================" << endl;
		
		cout << "请输入您要进行的操作: ";
		cin >> choice;
		switch(choice) {
			case 1:
				searchBaseInfo();
				break;
			case 2:
				searchWebsite();
				break;
			case 3:
				searchByBST();
				break;
			case 4:
				searchByLinkList();
				break;
			case 5:
				analyseByInsertSort();
				break;
			case 6:
				analyseByQuickSort();
				break;
			case 7:
				analyseBySelectionSort();
				break;
			case 8:
				 
		}
	}
	return 0;
}
// 哈希函数 
int hashFun(string str) {
	int length = str.length();
	int sum = 0;
	
	for(int i = 0; i < length; i++) {
		sum += str[i];
	}
	
	return sum % 97;
}
// 返回字符串编码和 
int encode(string str) {
	int length = str.length();
	int sum = 0;
	
	for(int i = 0; i < length; i++) {
		sum += str[i];
	}
	
	return sum;
}
// KMP
bool kmp(string s, string t) {
	int m = t.length();
        int n = s.length();
        s = '0' + s;
        t = '0' + t;

        vector<int> next(m+1, 0);
        next[0] = -1;  //起始next[0]==-1  next[1] = 0

        int j = 2, i = next[1];   //j是游标，不断往后移动  i是长度为j的字符串匹配前后匹配的最大长度 始终等于next[j]

        while(j <= m) {
            while(i!=-1 && t[i+1]!=t[j])  i = next[i];  //匹配不到就一直回退, 直到i为0

            if(i==-1) i = 0, j++;  //i回退到0的话，i=next[0]=-1, i要恢复到0，j继续往后扫描

            else {    //next[j] = t[i+1]
                next[j] = i+1;
                i = next[j];
                j++;
            }
        }

        i = 1, j = 1;

        while(i<=n && j<=m) {
            while(j>=1 && t[j]!=s[i])  j = next[j-1]+1;  //不匹配的话子串游标后移，父串游标不变

            if(!j) j = 1, i++;
            else i++, j++;
        }

        if(j<m) return false;  //j没走到最后，说明i走完了，j没走完 为匹配成功

        else return true;
}
//格式化序号 
string format(int number) {
	string ret;
	if(number < 10) {
		ret = "0" + to_string(number);
	} else {
		ret = to_string(number);
	}
	
	return ret;
}
// 录入股票基本信息 
void readStockBaseInfo() {
 	ifstream in("股票基本信息数据/A股公司简介.csv");   //读取文件 
 	string row;
 	getline(in, row); 			//第一行略过
 	int i = 0;      			//股票基本信息数组下标
 	string path1 = "股票涨跌数据/";
 	string path2 = ".txt";
 	string fullPath;
 	
 	while(getline(in, row)) {
	 //字符串是以逗号分割的
	 stringstream ss(row);  
	 string str;  //相当于一个中间缓存
	 vector<string> lineArray;
	 
	 while(getline(ss, str, ',')) lineArray.push_back(str);
	 
	 stocks[i].stockCode = lineArray[0];
	 stocks[i].shortName = lineArray[1];
	 stocks[i].industryCode = lineArray[2];
	 stocks[i].cate1 = lineArray[3];
	 stocks[i].cate2 = lineArray[4];
	 stocks[i].exchange = lineArray[5];
	 stocks[i].fullName = lineArray[6];
	 stocks[i].date = lineArray[7];
	 stocks[i].province = lineArray[8];
	 stocks[i].city = lineArray[9];
	 stocks[i].legalPerson = lineArray[10];
	 stocks[i].location = lineArray[11];
	 stocks[i].website = lineArray[12];
	 stocks[i].email = lineArray[13];
	 stocks[i].phone = lineArray[14];
	 stocks[i].majorBusiness = lineArray[15];
	 stocks[i].businessScope = lineArray[16];
	 
	 fullPath = path1 + lineArray[0] + path2;  //完整文件路径
	
	 ifstream fp(fullPath);
	 string line;
	
	 getline(fp, line);  //跳过第一行
	 getline(fp, line);  //正式读取
	 
	 vector<string> arr;
	 istringstream s(line);
	 while(getline(s, str, ' '))  arr.push_back(str);
	 
	 stocks[i].recentInfo[0] = arr[1];
	 stocks[i].recentInfo[1] = arr[2];
	 stocks[i].recentInfo[2] = arr[8];
	 
	 stocks[i].detail = new changeData;
	 dataLink head = stocks[i].detail;
	 
	 head->date = arr[0];
	 head->openingPrice = arr[1];
	 head->closingPrice = arr[2];
	 head->highestPrice = arr[3];
	 head->lowestPrice = arr[4];
	 head->volume = arr[5];
	 head->turnover = arr[6];
	 head->turnoverRate = arr[7];
	 head->FZDE = arr[8];
	 head->changePercent = stod(arr[9], 0);
	 head->next = NULL;
	 
	 //读取txt文件，每一行都需要录入，这波终究是逃不开
	while(getline(fp, line)) {
		vector<string> arr;
		istringstream s(line);
	 	while(getline(s, str, ' '))  arr.push_back(str);
	 	
	 	dataLink node = new changeData;
	 	node->date = arr[0];
		node->openingPrice = arr[1];
		node->closingPrice = arr[2];
		node->highestPrice = arr[3];
		node->lowestPrice = arr[4];
		node->volume = arr[5];
		node->turnover = arr[6];
		node->turnoverRate = arr[7];
		node->FZDE = arr[8];
		node->changePercent = stod(arr[9], 0);
		node->next = NULL;
		
		head->next = node;
		head = head->next;
	}
	i++;
	}
	for(int i = 0; i < 200; i++) {
		stocks[i].score = 0;
	}
	ifstream infile("股票基本信息数据/60支股票信息2.csv");
	string scope;
	
	getline(infile, scope);
	getline(infile, scope);

	while(getline(infile, scope)) {
		stringstream ss(scope);
		string str;
	
		vector<string> arr;
		while(getline(ss, str, ',')) arr.push_back(str);

		for(int i = 0; i < 200; i++) {
			if(stocks[i].stockCode == arr[1]) {
				stocks[i].score = stod(arr[2], 0);
				break;
			}
		}
	} //stocks[200]里面现在有评分了
	
	for(int i = 0; i < 200; i++) {
		dataLink head = stocks[i].detail;
		double maxChangePercent = head->changePercent;
		string date;
		while(head) {
			if(maxChangePercent < head->changePercent) {
				maxChangePercent = head->changePercent;
				date = head->date;
			}
			head = head->next;
		}
		stocks[i].maxChangePercent = maxChangePercent;
		stocks[i].date = date;
	}
}
// 创建哈希表 
void createHashTable() {
	for(int i = 0; i < 200; i++) {
		string str = stocks[i].stockCode;   //哈希函数核心代码 
		int index = hashFun(str);
		
		cout << index << str << endl;
		
		if(!hashTable[index]->next) {  //不存在 
			hashList insertNode = new hashNode;
			
			insertNode->node.stockCode = stocks[i].stockCode;
			insertNode->node.shortName = stocks[i].shortName;
			insertNode->node.industryCode = stocks[i].industryCode;
			insertNode->node.cate1 = stocks[i].cate1;
			insertNode->node.cate2 = stocks[i].cate2;
			insertNode->node.exchange = stocks[i].exchange;
			insertNode->node.fullName = stocks[i].fullName;
			insertNode->node.date = stocks[i].date;
			insertNode->node.province = stocks[i].province;
			insertNode->node.city = stocks[i].city;
			insertNode->node.legalPerson = stocks[i].legalPerson;
			insertNode->node.location = stocks[i].location;
			insertNode->node.website = stocks[i].website;
			insertNode->node.email = stocks[i].email;
			insertNode->node.phone = stocks[i].phone;
			insertNode->node.majorBusiness = stocks[i].majorBusiness;
			insertNode->node.businessScope = stocks[i].businessScope;
			
			insertNode->next = NULL;
			
			hashTable[index]->next = insertNode;
		} 
		else {
			// 存在，开链  头插法
			hashList head = hashTable[index]->next;   // 必要步骤 
			hashList insertNode = new hashNode;

			insertNode->node.stockCode = stocks[i].stockCode;
			insertNode->node.shortName = stocks[i].shortName;
			insertNode->node.industryCode = stocks[i].industryCode;
			insertNode->node.cate1 = stocks[i].cate1;
			insertNode->node.cate2 = stocks[i].cate2;
			insertNode->node.exchange = stocks[i].exchange;
			insertNode->node.fullName = stocks[i].fullName;
			insertNode->node.date = stocks[i].date;
			insertNode->node.province = stocks[i].province;
			insertNode->node.city = stocks[i].city;
			insertNode->node.legalPerson = stocks[i].legalPerson;
			insertNode->node.location = stocks[i].location;
			insertNode->node.website = stocks[i].website;
			insertNode->node.email = stocks[i].email;
			insertNode->node.phone = stocks[i].phone;
			insertNode->node.majorBusiness = stocks[i].majorBusiness;
			insertNode->node.businessScope = stocks[i].businessScope;
			
			insertNode->next = head;
			hashTable[index]->next = insertNode;
		}
	}
}
// 哈希查找基本信息
void searchBaseInfo() {
	//初始化哈希表
	for(int i = 0; i < 97; i++) {
		hashTable[i] = new hashNode;  //这里记得new  成功！！！！！ 
		hashTable[i]->next = NULL;  
	}
	
	createHashTable();
	cout << hashTable[31]->node.shortName << 123;
	
	string number;
	int flag = 1;
	
	cout << "请输入您要查询的股票代码（例如：cn_600519）：";
	
	while(cin>>number && flag) {
		cout << number << endl;
		int index = hashFun(number);
		
		if(hashTable[index]->next == NULL) {
			cout << "不存在该股票，请重新输入！";
			cout << "请输入您要查询的股票代码（例如：cn_600519）：";
		} 
		else {
			hashList stockNode = hashTable[index]->next;
			while(stockNode) {
				if(stockNode->node.stockCode == number) {
					cout << "查找成功！" << endl;
					cout << "股票名称：" << stockNode->node.shortName << endl;
					cout << "股票代码：" << stockNode->node.stockCode << endl;
					cout << "一级行业：" << stockNode->node.cate1 << endl;
					cout << "二级行业：" << stockNode->node.cate2 << endl;
					cout << "主营业务：" << stockNode->node.majorBusiness << endl;
					flag = 0;
					break;
				}
				stockNode = stockNode->next;
			}
		}
	}
} 
// KMP网址查找
void searchWebsite() {
	cout << "请输入网址关键字: ";
	string keywords;
	cin >> keywords;
	int flag = 1;
	
	for(int i = 0; i < 200; i++) {
		if(kmp(stocks[i].website, keywords)) {
			cout << stocks[i].shortName << " " << stocks[i].stockCode << endl;
			flag = 0;
			break;
		}
	}
}
// 基于二叉排序树查询 
void searchByBST() {
	// https://www.cnblogs.com/linfangnan/p/12958068.html
	BinTree T = NULL;
	createBST(T);
	// 创建完毕！
	string toSearch;
	cout << "请输入要查询的股票代码：";
	cin >> toSearch;
	BinTree node = searchBST(T, toSearch);
	cout << "最近一日股票信息如下：" << endl; 
	cout << "开盘价 " << node->data.recentInfo[0] << endl;
	cout << "收盘价 " << node->data.recentInfo[1] << endl;
	cout << "涨跌幅 " << node->data.recentInfo[2] << endl;
}
// 创建BST
void createBST(BinTree &T) {
	for(int i = 0; i < 200; i++) {
		insertBST(T, i);  // i是第几个
	}
}
// 插入节点 
void insertBST(BinTree& root, int i) {
	if(!root) {  // 叶子结点 
		root = new TNode;
		root->data = stocks[i];
		root->left = NULL;
		root->right = NULL;
	}
	else {
		if(stocks[i].stockCode < root->data.stockCode) {
			insertBST(root->left, i);
		} else if(stocks[i].stockCode > root->data.stockCode) {
			insertBST(root->right, i);
		}
	}
}
// 根据股票代码在二叉排序树上搜索
BinTree searchBST(BinTree T, string key) {
	if(T == NULL || key == T->data.stockCode)
       return T;
    else if(key < T->data.stockCode)      //递归进入左子树查找
       return searchBST(T->left, key);
    else                                 //递归进入右子树查找
       return searchBST(T->right, key);
}
// 基于单链表的股票价格信息查询 
void searchByLinkList() {
	cout << "请输入要查询的日期：";
	string date;
	cin >> date;  
	//录入成功！！！！！
	 
	dataLink link = getLinkByDate(date);
	
	cout << "股票代码" << "       " << "股票名称" << "      " << "开盘价" << "      " << "收盘价" << "      " << "涨跌幅" << endl;
	link = link->next;
	while(link) {
		cout << link->stockCode << "      ";
		cout << link->shortName << "      ";
		cout << link->openingPrice << "      ";
		cout << link->closingPrice << "      ";
		cout << link->changePercent << endl;
		link = link->next;
	}
	
}
// 根绝日期获取所有股票该天的股票信息 
dataLink getLinkByDate(string date) {
	dataLink link = new changeData;
	dataLink p = link; //游标 
	
	for(int i = 0; i < 200; i++) {
		dataLink head = stocks[i].detail; //拿到首元结点啦！ 
		while(head) {
			if(head->date == date) {
				dataLink node = new changeData;
				node->stockCode = stocks[i].stockCode;
				node->shortName = stocks[i].shortName;
				node->openingPrice = head->openingPrice;
				node->closingPrice = head->closingPrice;
				node->changePercent = head->changePercent;
				p->next = node;
				p = p->next;
				break;
			}
		}
	}
	
	return link;
}
// 基于直接插入排血的股票价格分析
void analyseByInsertSort() {
	string date;
	cout << "请输入查询日期："; 
	cin >> date;
	
	dataLink link = getLinkByDate(date);
	cout << "1. 根据开盘价排序" << endl;
	cout << "2. 根据收盘价排序" << endl;
	cout << "3. 根据涨跌幅排序" << endl;
	cout << "请输入排序方式：";
	int option;
	cin >> option;
	switch(option) {
		case 1:
			sortByOpeningPrice(link);
			break;
		case 2:
			sortByClosingPrice(link);
			break;
		case 3:
			sortByChangePercent(link);
			break;
	}
}
// 根据开盘价排序
void sortByOpeningPrice(dataLink L) {
	//https://blog.csdn.net/vv_017/article/details/80502837
	dataLink p, pre, q;
	p = L->next->next;		// 先保存下L的第二个元素，因为下一步要将L变成只有一个元素的有序表。  
	L->next->next = NULL;	// 将L变成只有一个元素的有序表
	// 从L的第二个元素开始遍历整个L直至表尾 
	while(p != NULL){
		q = p->next;
		pre = L;	// 先用pre来保存L。
		//https://blog.csdn.net/sinat_40872274/article/details/81367815
		while(pre->next !=NULL && stod(pre->next->openingPrice, 0) > stod(p->openingPrice, 0)) // 遍历pre所指向的有序表L，直至找到比p大的节点 
			pre = pre->next; 
		p->next = pre->next;
		pre->next = p;
		p = q;	
	}
	int index = 1;
	L = L->next;
	ofstream outfile("价格和涨跌幅排序结果-开盘价降序.csv", ios::out);
	outfile << "序号" << "," << "股票代码" << "," << "股票名称" << "," << "开盘价" << "," << "收盘价"
	<< "," << "涨跌幅" << endl;
	cout << "序号" << "      "<< "股票代码" << "       " << "股票名称" << "      " << "开盘价" << "      " << "收盘价" << "      " << "涨跌幅" << endl;
	while(L) {
		cout << index << "      ";
	 	cout << L->stockCode << "      ";
		cout << L->shortName << "      ";
		cout << L->openingPrice << "      ";
		cout << L->closingPrice << "      ";
		cout << L->changePercent << endl;
		outfile << index << "," << L->stockCode << "," << L->shortName << "," << L->openingPrice << ","
		<< L->closingPrice << "," << L->changePercent << endl;
		L = L->next;
		index++;
	}
	outfile.close();
}
// 根据收盘价排序
void sortByClosingPrice(dataLink L) {
	dataLink p, pre, q;
	p = L->next->next;		// 先保存下L的第二个元素，因为下一步要将L变成只有一个元素的有序表。  
	L->next->next = NULL;	// 将L变成只有一个元素的有序表
	// 从L的第二个元素开始遍历整个L直至表尾 
	while(p != NULL){
		q = p->next;
		pre = L;	// 先用pre来保存L。
		while(pre->next !=NULL && stod(pre->next->closingPrice, 0) > stod(p->closingPrice, 0)) // 遍历pre所指向的有序表L，直至找到比p大的节点 
			pre = pre->next; 
		p->next = pre->next;
		pre->next = p;
		p = q;	
	}
	int index = 1;
	L = L->next;
	ofstream outfile("价格和涨跌幅排序结果-收盘价降序.csv", ios::out);
	outfile << "序号" << "," << "股票代码" << "," << "股票名称" << "," << "开盘价" << "," << "收盘价"
	<< "," << "涨跌幅" << endl;
	cout << "序号" << "      "<< "股票代码" << "       " << "股票名称" << "      " << "开盘价" << "      " << "收盘价" << "      " << "涨跌幅" << endl;
	while(L) {
		cout << index << "      ";
	 	cout << L->stockCode << "      ";
		cout << L->shortName << "      ";
		cout << L->openingPrice << "      ";
		cout << L->closingPrice << "      ";
		cout << L->changePercent << endl;
		outfile << index << "," << L->stockCode << "," << L->shortName << "," << L->openingPrice << ","
		<< L->closingPrice << "," << L->changePercent << endl;
		L = L->next;
		index++;
	}
	outfile.close();
}
// 根据涨跌幅排序
void sortByChangePercent(dataLink L) {
	dataLink p, pre, q;
	p = L->next->next;		// 先保存下L的第二个元素，因为下一步要将L变成只有一个元素的有序表。  
	L->next->next = NULL;	// 将L变成只有一个元素的有序表
	// 从L的第二个元素开始遍历整个L直至表尾 
	while(p != NULL){
		q = p->next;
		pre = L;	// 先用pre来保存L。
		while(pre->next !=NULL && pre->next->changePercent > p->changePercent) // 遍历pre所指向的有序表L，直至找到比p大的节点 
			pre = pre->next; 
		p->next = pre->next;
		pre->next = p;
		p = q;	
	}
	int index = 1;
	L = L->next;
	ofstream outfile("价格和涨跌幅排序结果-涨跌幅降序.csv", ios::out);
	outfile << "序号" << "," << "股票代码" << "," << "股票名称" << "," << "开盘价" << "," << "收盘价"
	<< "," << "涨跌幅" << endl;
	cout << "序号" << "      "<< "股票代码" << "       " << "股票名称" << "      " << "开盘价" << "      " << "收盘价" << "      " << "涨跌幅" << endl;
	while(L) {
		cout << index << "      ";
	 	cout << L->stockCode << "      ";
		cout << L->shortName << "      ";
		cout << L->openingPrice << "      ";
		cout << L->closingPrice << "      ";
		cout << L->changePercent << endl;
		outfile << index << "," << L->stockCode << "," << L->shortName << "," << L->openingPrice << ","
		<< L->closingPrice << "," << L->changePercent << endl;
		L = L->next;
		index++;
	}
	outfile.close();
}
// 基于快速排序的股票价格分析
void analyseByQuickSort() {
	
	string cate;
	cout << "请输入一级行业：";
	cin >> cate;
	
	vector<StockInfo> industry;  //该行业的股票
	industry.push_back(stocks[0]);
	
	for(int i = 0; i < 200; i++) {
		if(stocks[i].cate1 == cate && stocks[i].score > 0) {
			industry.push_back(stocks[i]);
		}
	}
	quickSort(industry, 1, industry.size()-1);
	int n = industry.size();
	for(int i = 1; i < n-1; i++) {
		cout << i << "   " << industry[i].stockCode << "   " << industry[i].shortName << 
		"   " << industry[i].maxChangePercent << "   " << industry[i].maxDate << endl;
	}
}
void quickSort(vector<StockInfo> &industry, int low, int high) {
	if(low < high) {
		int pivitloc = partition(industry, low, high);
		quickSort(industry, low, pivitloc-1);
		quickSort(industry, pivitloc+1, high);
	}
}
int partition(vector<StockInfo> &industry, int low, int high) {
	industry[0] = industry[low];   
	double pivotkey = industry[low].maxChangePercent;

    while(low < high)
	{ 
	   while(low < high && industry[high].maxChangePercent < pivotkey )  
	   	--high;
       industry[low] = industry[high];
       while (low < high && industry[low].maxChangePercent >= pivotkey )  
	   	++low;
       industry[high] = industry[low];
    }
    industry[low] = industry[0]; 
    return low;
}
// 基于简单选择排序的股票价格分析
void analyseBySelectionSort() {
	ifstream in("股票基本信息数据/60支股票信息2.csv");
	string row;
	
	getline(in, row);
	getline(in, row);
	vector<vector<string>> arr1;

	while(getline(in, row)) {
		stringstream ss(row);
		string str;
	
		vector<string> arr;
		while(getline(ss, str, ',')) arr.push_back(str);
		arr1.push_back(arr);
	}  //读好数据放到 arr1 当中 
	
	int num = arr1.size();
	for(int i = 0; i < num-1; i++) {
		int k = i;
		for(int j = i+1; j < num; j++) {
			if(stod(arr1[j][2]) > stod(arr1[k][2])) k = j;
		}
		if(k != i) swap(arr1[i], arr1[k]);
	}  //根据评分排序  OK！
	 
	vector<StockInfo> arr2;
	for(int i = 0; i < 200; i++) {
		if(stocks[i].score > 0)  arr2.push_back(stocks[i]);
	}
	
	int n = arr2.size();
	for(int i = 0; i < n-1; i++) {
		int k = i;
		for(int j = i+1; j < n; j++) {
			if( stod(arr2[j].recentInfo[1]) > stod(arr2[k].recentInfo[1]) ) k = j;
		}
		if(k != i) swap(arr2[i], arr2[k]);
	}  //根据收盘价排序  OK!
	
	cout << "序号" << "   " << "股票代码" << "   " << "股票名称" << "   " << "评分  " ;
	cout << "序号" << "   " << "股票代码" << "   " << "股票名称" << "   " << "收盘价" << endl;
	
	ofstream scoreSort("评分排序.csv", ios::out);
	scoreSort << "序号" << "," << "股票代码" << "," << "股票名称" << "," << "评分" << endl;
	ofstream priceSort("收盘价排序.csv", ios::out);
	priceSort << "序号" << "," << "股票代码" << "," << "股票名称" << "," << "收盘价" << endl;
	
	for(int i = 0; i < 60; i++) { 
		cout << format(i+1) << "   " << arr1[i][0] << "   " << arr1[i][1] << "   " << arr1[i][2] << " |";
		cout << format(i+1) << "   " << arr2[i].stockCode << "   " << arr2[i].shortName << "   " << arr2[i].recentInfo[1] << endl;
		
		scoreSort << i+1 << "," << arr1[i][0] << "," << arr1[i][1] << "," << arr1[i][2] << endl;
		priceSort << i+1 << "," << arr2[i].stockCode << "," << arr2[i].shortName << "," << arr2[i].recentInfo[1] << endl;
	}

	scoreSort.close();
	priceSort.close();
}
// 















