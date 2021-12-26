#include "general.h"

vector<hashList> hashTable(HASHLENGTH);  //定义哈希表
vector<StockInfo> stocks(200);           //存储200支股票的基本信息和具体股票信息
vector<vector<edge>> e;                  //邻接表 
vector<int> d;                           //外部点到内部点的最短距离 
vector<int> vis;                         //标记数组
vector<fundPortfolio> ret;               //最小生成树当中的基金组合
vector<score> scores;                    //评分和名字
vector<int> p;                           //并查集数组 

int main()
{
	readStockInfo();   //读文件，保存数据 
	string choice;

	while(true) {
		system("cls");
		cout << "============================================================" << endl;
		cout << "=======      欢迎使用股票查询与分析系统 V2.0         =======" << endl;
		cout << "=======                                              =======" << endl;
		cout << "=======              1. 查询功能                     =======" << endl;
		cout << "=======              2. 分析功能                     =======" << endl;
		cout << "=======              3. 退出系统                     =======" << endl;
		cout << "=======                                              =======" << endl;
		cout << "============================================================" << endl;
		
		cout << "请输入您要进行的操作(输入不合法内容将重新刷新页面): ";
		cin >> choice;
		
		if(choice == "1") showSearchMenu();
		else if(choice == "2") showAnalyseMenu();
		else if(choice == "3") {
			cout << endl;
			cout << "欢迎下次使用！退出中...." << endl;
			exit(0);
		}
	}
	return 0;
}

// 工具函数 
int hashFun(string str) {
	int length = str.length();
	int sum = 0;
	
	for(int i = 0; i < length; i++) {
		sum += str[i];
	}
	
	return sum % 97;
}
double hashASL(vector<StockInfo> info, vector<hashList> list) {
	int num = info.size();
	double counter = 0;
	
	for(int i = 0; i < num; i++) {
		string name = info[i].stockCode;
		int index = hashFun(name);
		
		hashList stockNode = list[index]->next;
		counter++;
		
		while(stockNode) {
			if(stockNode->node.stockCode == name)  break;
			stockNode = stockNode->next;
			counter++;
		}
	}
	return counter/SIZE;
}
double bstASL(BinTree T, vector<StockInfo> info) {
	int num = info.size();
	
	double counter = 0;
	for(int i = 0; i < num; i++) {
		string code = info[i].stockCode;
		searchBST(T, code, counter);
	}
	
	return counter / num;
}
int encode(string str) {
	int length = str.length();
	int sum = 0;
	
	for(int i = 0; i < length; i++) {
		sum += str[i];
	}
	
	return sum;
}
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
string format(int number) {
	string ret;
	if(number < 10) {
		ret = "0" + to_string(number);
	} else {
		ret = to_string(number);
	}
	
	return ret;
}
bool cmp(const fundPortfolio t1, const fundPortfolio t2) {
	if(t1.weight < t2.weight) return true;
	return false;
} 
bool cmpByScores(const fundPortfolio t1, const fundPortfolio t2) {
	if(t1.scoreSum > t2.scoreSum) return true;
	return false;
} 
int find(int x) {
	return x = p[x] ? x : p[x] = find(p[x]);
}

// 功能入口
void showSearchMenu() {
	bool flag = true;
	while(flag) {
		system("cls");
		cout << "============================================================" << endl;
		cout << "=======     1. 基于哈希表的股票基本信息查询          =======" << endl;
  		cout << "=======     2. 基于KMP的股票网址查询                 =======" << endl;
  		cout << "=======     3. 基于二叉排序树的股票基本信息查询      =======" << endl;
  		cout << "=======     4. 基于单链表的股票价格信息查询          =======" << endl;
  		cout << "=======     5. 返回上级菜单                          =======" << endl;
  		cout << "============================================================" << endl;
  		cout << endl;
  		
  		cout << "请输入您要执行的功能：";
  		int opt;  
		cin >> opt;
  		switch(opt) {
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
  				flag = false;
  				break;
		}
	}
}
void showAnalyseMenu() {
	bool flag = true;
	while(flag) {
		system("cls");
		cout << "============================================================" << endl;
		cout << "=======     1. 基于直接插入排序的股票价格分析        =======" << endl;
  		cout << "=======     2. 基于快速排序的股票价格分析            =======" << endl;
  		cout << "=======     3. 基于简单选择排序的股票价格分析        =======" << endl;
  		cout << "=======     4. 基于Floyd的股票相关性计算             =======" << endl;
  		cout << "=======     5. 基于Prim最小生成树的股票基金筛选      =======" << endl;
		cout << "=======     6. 基于二部图的股票基金筛选              =======" << endl;
		cout << "=======     7. 基于Kruskal最小生成树的股票基金筛选   =======" << endl;
		cout << "=======     8. 基于二叉排序树的股票基本信息删除      =======" << endl;
  		cout << "=======     9. 返回上级菜单                          =======" << endl;
  		cout << "============================================================" << endl;
  		cout << endl;
  		
  		cout << "请输入您要执行的功能：";
  		int opt;  
		cin >> opt;
  		switch(opt) {
  			case 1:
  				analyseByInsertSort();
  				break;
  			case 2:
  				analyseByQuickSort();
  				break;
  			case 3:
  				analyseBySelectionSort();
  				break;
  			case 4:
  				analyseByFloyd();
  				break;
  			case 5:
  				analyseByPrim();
  				break;
  			case 6:
  				analyseByBigraph();
  				break;
  			case 7:
  				anylyseByKruskal();
  				break;
  			case 8:
  				deleteBSTNode();
  				break;
  			case 9:
  				flag = false;
  				break;
		}
	}
}

// 功能实现 
// 录入股票基本信息 
void readStockInfo() {
 	ifstream in("股票基本信息数据/A股公司简介.csv");   //读取文件, 定义文件流 
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
	 stocks[i].cate1 = lineArray[3];
	 stocks[i].cate2 = lineArray[4];
	 stocks[i].date = lineArray[7];
	 stocks[i].website = lineArray[12];
	 stocks[i].majorBusiness = lineArray[15];
	 stocks[i].businessScope = lineArray[16];
	 
	 fullPath = path1 + lineArray[0] + path2;  //完整文件路径
	 ifstream fp(fullPath);
	 string line;
	
	 getline(fp, line);  //跳过第一行
	 getline(fp, line);  //正式读取, 这里是只读最近一日的 
	 
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
	 head->changePercent = stod(arr[9], 0);
	 head->next = NULL;
	 
	//读取txt文件，每一行都需要录入，终究是逃不开
	 while(getline(fp, line)) {
		 vector<string> arr;
		 istringstream s(line);
	 	 while(getline(s, str, ' '))  arr.push_back(str);
	 	
	 	 dataLink node = new changeData;
	 	 node->date = arr[0];
		 node->openingPrice = arr[1];
		 node->closingPrice = arr[2];
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
	getline(infile, scope);   //跳过前两行 
	
	score scoreNode;
	scores.push_back(scoreNode);  // 目的是让下标从1开始 
		
	while(getline(infile, scope)) {
		stringstream ss(scope);
		string str;
	
		vector<string> arr;
		while(getline(ss, str, ',')) arr.push_back(str);
		
		score scoreNode;
		scoreNode.name = arr[0];
		scoreNode.score = stod(arr[2], 0);
		scores.push_back(scoreNode);
		
		for(int i = 0; i < 200; i++) {
			if(stocks[i].stockCode == arr[1]) {
				stocks[i].score = stod(arr[2], 0);
				break;
			}
		}
	}   //stocks[200]里面现在有评分了
	
	// 最大涨跌幅和对应的日期 
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
		stocks[i].maxDate = date;
	}
}
// 创建哈希表 
void createHashTable() {
	//初始化哈希表
	for(int i = 0; i < HASHLENGTH; i++) {
		hashTable[i] = new hashNode;  //这里记得new  成功！！！！！ 
		hashTable[i]->next = NULL;  
	}
	
	for(int i = 0; i < SIZE; i++) {
		string str = stocks[i].stockCode;   //哈希函数核心代码 
		int index = hashFun(str);
		
		if(!hashTable[index]->next) {       //不存在
			hashList insertNode = new hashNode;
			
			insertNode->node.stockCode = stocks[i].stockCode;
			insertNode->node.shortName = stocks[i].shortName;
			insertNode->node.cate1 = stocks[i].cate1;
			insertNode->node.cate2 = stocks[i].cate2;
			insertNode->node.date = stocks[i].date;
			insertNode->node.website = stocks[i].website;
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
			insertNode->node.cate1 = stocks[i].cate1;
			insertNode->node.cate2 = stocks[i].cate2;
			insertNode->node.date = stocks[i].date;
			insertNode->node.website = stocks[i].website;
			insertNode->node.majorBusiness = stocks[i].majorBusiness;
			insertNode->node.businessScope = stocks[i].businessScope;
			
			insertNode->next = head;
			hashTable[index]->next = insertNode;
		}
	}
}
// 哈希查找基本信息
void searchBaseInfo() {
	createHashTable();
	
	string number;
	int flag1 = 1;
	
	cout << "请输入您要查询的股票代码（例如：cn_600519）：";
	
	while(flag1 && cin >> number) {
		int index = hashFun(number);
		
		if(!hashTable[index]->next) {
			cout << "不存在该股票！" << endl;
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
					flag1 = 0;
					break;
				}
				stockNode = stockNode->next;
			}
			if(!flag1) cout << "查找成功的ASL为：" << hashASL(stocks, hashTable) << endl;
			else cout << "不存在该股票，请重新输入" << endl;
		}
		
		int isBreak = 0;
		while(!isBreak) {
			cout << "请确认是否继续查找(yes/no): ";
			string isGo;
			cin >> isGo;
			if(isGo == "no") {
				flag1 = 0;
				isBreak = 1;
			}
			else if(isGo == "yes") {
				cout << "请输入您要查询的股票代码（例如：cn_600519）：";
				break;
			}
			else cout << "输入不合法，请重新输入！" << endl;
		}	
	}
} 
// KMP网址查找
void searchWebsite() {
	int isExit = 0;
	
	while(!isExit) {
		cout << "请输入网址关键字: ";
		string keywords;
		cin >> keywords;
		
		int flag = 0;
		
		for(int i = 0; i < SIZE; i++) {
			if(kmp(stocks[i].website, keywords)) {
				cout << "匹配成功！\n\n"; 
				cout << "股票名称：" << stocks[i].shortName << "   " << " 股票代码：" << 
				stocks[i].stockCode << endl << endl;
				flag = 1;
				break;
			}
		}
		
		if(!flag) cout << "抱歉，暂无该网址相关信息" << endl;
		
		int isBreak = 0;
		while(!isBreak) {
			cout << "请确认是否继续查找(yes/no): ";
			string isGo;
			cin >> isGo;
			if(isGo == "no") {
				isExit = 1;
				isBreak = 1;
			}
			else if(isGo == "yes") {
				cout << "请输入网址关键字：";
				break;
			}
			else cout << "输入不合法，请重新输入！" << endl;
		}	
	}	
}
// 基于二叉排序树查询 
void searchByBST() {
	BinTree T = NULL;
	
	createBST(T, stocks);
	// 创建完毕！
	
	int isExit = 0;
	while(!isExit) {
		string toSearch;
		cout << "请输入要查询的股票代码：";
		cin >> toSearch;
		
		double times = 0;
		BinTree node = searchBST(T, toSearch, times);
		if(!node) {    //返回为空 
			cout << "查询失败，不存在该股票！" << endl;
		} else {
			cout << "最近一日股票信息如下：" << endl; 
			cout << "开盘价 " << node->data.recentInfo[0] << endl;
			cout << "收盘价 " << node->data.recentInfo[1] << endl;
			cout << "涨跌幅 " << node->data.recentInfo[2] << endl;
			cout << "查找成功的ASL为" << bstASL(T, stocks) << endl;
		}
		
		int isBreak = 0;
		while(!isBreak) {
			cout << "请确认是否继续查找(yes/no): ";
			string isGo;
			cin >> isGo;
			if(isGo == "no") {
				isExit = 1;
				isBreak = 1;
			}
			else if(isGo == "yes") {
				break;
			}
			else cout << "输入不合法，请重新输入！" << endl;
		}	
	}
}
// 创建BST
void createBST(BinTree &T, vector<StockInfo> stocksInfo) {
	for(int i = 0; i < SIZE; i++) {
		insertBST(T, i, stocksInfo);  // i是第几个
	}
}
// 插入节点 
void insertBST(BinTree& root, int i, vector<StockInfo> stocksInfo) {
	if(!root) {  // 叶子结点 
		root = new TNode;
		root->data = stocks[i];
		root->left = NULL;
		root->right = NULL;
	}
	else {
		if(stocks[i].stockCode < root->data.stockCode) {
			insertBST(root->left, i, stocksInfo);
		} else if(stocks[i].stockCode > root->data.stockCode) {
			insertBST(root->right, i, stocksInfo);
		}
	}
}
// 根据股票代码在二叉排序树上搜索
BinTree searchBST(BinTree T, string key, double& times) {
	if(T == NULL || key == T->data.stockCode){
		times++;
		return T;
	}
    else if(key < T->data.stockCode) {  //递归进入左子树查找
    	times++;
    	return searchBST(T->left, key, times);
	}        
    else {							    //递归进入右子树查找
    	times++;
		return searchBST(T->right, key, times);
	}                                     
}
// 基于单链表的股票价格信息查询 
void searchByLinkList() {
	int isExit = 0;
	
	while(!isExit) {
		cout << "请输入要查询的日期：";
		string date;
		cin >> date;
		
		dataLink link = getLinkByDate(date);
	
		if(!link->next) cout << "该日数据不存在！" << endl;
		
		else {
			link = link->next;
			printf("%-10s%-10s%-10s%-10s%-10s\n", "股票代码", "股票名称", "开盘价", "收盘价", "涨跌幅");
			
			while(link) {
				printf("%-10s", link->stockCode.c_str());
				printf("%-10s", link->shortName.c_str());
				printf("%-10s", link->openingPrice.c_str());
				printf("%-10s", link->closingPrice.c_str());
				printf("%-10f", link->changePercent);
				printf("\n");
				link = link->next;
			}	
		}
		
		int isBreak = 0;
		while(!isBreak) {
			cout << "请确认是否继续查找(yes/no): ";
			string isGo;
			cin >> isGo;
			if(isGo == "no") {
				isExit = 1;
				isBreak = 1;
			}
			else if(isGo == "yes") {
				break;
			}
			else cout << "输入不合法，请重新输入！" << endl;
		}	
	}
}
// 根据日期获取所有股票该天的股票信息 
dataLink getLinkByDate(string date) {
	dataLink link = new changeData;
	link->next = NULL;
	dataLink p = link; //游标 
	
	for(int i = 0; i < SIZE; i++) {
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
			head = head->next;
		}
	}
	
	return link;
}

// 基于直接插入排血的股票价格分析
void analyseByInsertSort() {
	int isExit = 0;
	
	while(!isExit) {
		string date;
		cout << "请输入查询日期："; 
		cin >> date;
		
		dataLink link = getLinkByDate(date);
		
		if(!link->next) {
			cout << "该日数据不存在！" << endl;
		} 
		else {
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
		
		int isBreak = 0;
		while(!isBreak) {
			cout << "请确认是否继续(yes/no): ";
			string isGo;
			cin >> isGo;
			if(isGo == "no") {
				isExit = 1;
				isBreak = 1;
			}
			else if(isGo == "yes") {
				break;
			}
			else cout << "输入不合法，请重新输入！" << endl;
		}	
	}
}
// 根据开盘价排序
void sortByOpeningPrice(dataLink L) {
	dataLink p, pre, q;
	p = L->next->next;		 // 先保存下L的第二个元素，因为下一步要将L变成只有一个元素的有序表。  
	L->next->next = NULL;	 // 将L变成只有一个元素的有序表
	
   // 从L的第二个元素开始遍历整个L直至表尾 
	while(p){
		q = p->next;
		pre = L;	// 先用pre来保存L
		
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
	printf("%-4s%-10s%-10s%-10s%-10s%-10s\n", "序号", "股票代码", "股票名称", "开盘价", "收盘价", "涨跌幅");
	
	while(L) {
		printf("%-4d", index);
		printf("%-10s", L->stockCode.c_str());
		printf("%-10s", L->shortName.c_str());
		printf("%-10s", L->openingPrice.c_str());
		printf("%-10s", L->closingPrice.c_str());
		printf("%-10f", L->changePercent);
		printf("\n");
		
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
	p = L->next->next;		 // 先保存下L的第二个元素，因为下一步要将L变成只有一个元素的有序表。  
	L->next->next = NULL;	 // 将L变成只有一个元素的有序表
	
	// 从L的第二个元素开始遍历整个L直至表尾 
	while(p){
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
	printf("%-4s%-10s%-10s%-10s%-10s%-10s\n", "序号", "股票代码", "股票名称", "开盘价", "收盘价", "涨跌幅");
	 
	while(L) {
		printf("%-4d", index);
		printf("%-10s", L->stockCode.c_str());
		printf("%-10s", L->shortName.c_str());
		printf("%-10s", L->openingPrice.c_str());
		printf("%-10s", L->closingPrice.c_str());
		printf("%-10f", L->changePercent);
		printf("\n");
		
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
	while(p){
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
	
	printf("%-4s%-10s%-10s%-10s%-10s%-10s\n", "序号", "股票代码", "股票名称", "开盘价", "收盘价", "涨跌幅");
	while(L) {
		printf("%-4d", index);
		printf("%-10s", L->stockCode.c_str());
		printf("%-10s", L->shortName.c_str());
		printf("%-10s", L->openingPrice.c_str());
		printf("%-10s", L->closingPrice.c_str());
		printf("%-10f", L->changePercent);
		printf("\n");
		
		outfile << index << "," << L->stockCode << "," << L->shortName << "," << L->openingPrice << ","
		<< L->closingPrice << "," << L->changePercent << endl;
		L = L->next;
		index++;
	}
	outfile.close();
}
// 基于快速排序的股票价格分析
void analyseByQuickSort() {
	int isExit = 0;
	
	while(!isExit) {
		string cate;
		cout << "请输入一级行业：";
		cin >> cate;
		
		vector<StockInfo> industry;    //该行业的股票
		industry.push_back(stocks[0]); //随便push一个进去 
		
		for(int i = 0; i < 200; i++) {
			if(stocks[i].cate1 == cate && stocks[i].score > 0) {
				industry.push_back(stocks[i]);
			}
		}
		quickSort(industry, 1, industry.size()-1);
		int n = industry.size();
		
		
		for(int i = 1; i < n; i++) {
			cout << i << "   " << industry[i].stockCode << "   " << industry[i].shortName << 
			"   " << industry[i].maxChangePercent << "   " << industry[i].maxDate << endl;
		}
		
		int isBreak = 0;
		while(!isBreak) {
			cout << "请确认是否继续(yes/no): ";
			string isGo;
			cin >> isGo;
			if(isGo == "no") {
				isExit = 1;
				isBreak = 1;
			}
			else if(isGo == "yes") {
				break;
			}
			else cout << "输入不合法，请重新输入！" << endl;
		}	
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
	
	
	int isExit = 0;
	while(!isExit) {
		printf("%-5s%-10s%-10s%-10s", "序号", "股票代码", "股票名称", "评分");
		printf("%-5s%-10s%-10s%-10s\n", "序号", "股票代码", "股票名称", "收盘价");
		
		ofstream scoreSort("评分排序.csv", ios::out);
		scoreSort << "序号" << "," << "股票代码" << "," << "股票名称" << "," << "评分" << endl;
		ofstream priceSort("收盘价排序.csv", ios::out);
		priceSort << "序号" << "," << "股票代码" << "," << "股票名称" << "," << "收盘价" << endl;
		
		for(int i = 0; i < 60; i++) {
			printf("%-5d", i+1); 
			printf("%-10s", arr1[i][0].c_str());
			printf("%-10s", arr1[i][1].c_str());
			printf("%-10s", arr1[i][2].c_str());
			cout << "|";
			printf("%-5d", i+1); 
			printf("%-10s", arr2[i].stockCode.c_str());
			printf("%-10s", arr2[i].shortName.c_str());
			printf("%-10s", arr2[i].recentInfo[1].c_str());
			cout << endl;
			
			scoreSort << i+1 << "," << arr1[i][0] << "," << arr1[i][1] << "," << arr1[i][2] << endl;
			priceSort << i+1 << "," << arr2[i].stockCode << "," << arr2[i].shortName << "," << arr2[i].recentInfo[1] << endl;
		}
	
		scoreSort.close();
		priceSort.close();
		
		int isBreak = 0;
		while(!isBreak) {
			cout << "输入任意内容以继续 ";
			string isGo;
			cin >> isGo;
			if(isGo!="") {
				isExit = 1;
				isBreak = 1;
			}
		}	
	}	
}
// 基于Floyd的股票相关性计算
void analyseByFloyd() {	
	vector<vector<int>> graph(N+1, vector<int>(N+1, 0)); //60只股票邻接矩阵 
	for(int i = 1; i <= N; i++)
		for(int j = 1; j <= N; j++) {
			graph[i][j] = INF;   //无穷
		} 
			
	for(int i = 1; i <= N; i++) {
		graph[i][i] = 0; 
	} 	//初始化邻接矩阵 
	
	ifstream in("股票基本信息数据/60支股票信息1.csv");
	string row;
	getline(in, row);
	
	while(getline(in, row)) {
		stringstream ss(row);
		string str;
		
		vector<string> arr;
		while(getline(ss, str, ',')) arr.push_back(str);
		
		int dot1 = stoi(arr[0]);
		int dot2 = stoi(arr[1]);
		int dis = stoi(arr[2]);
		graph[dot1][dot2] = dis;
		graph[dot2][dot1] = dis;
	}
	
	for(int k = 1; k <= N; k++) {
		for(int i = 1; i <= N; i++) {
			for(int j = 1; j <= N; j++) {
				if(graph[i][k] + graph[k][j] < graph[i][j])
					graph[i][j] = graph[i][k] + graph[k][j];
			}
		}
	}
	
	int isExit = 0;
	
	while(!isExit) {
		cout << "请依次输入两个数字，分别表示图中两个节点：";
		int node1, node2;
		cin >> node1 >> node2;
		cout << "最短路径为" << graph[node1][node2] << endl;
			
		int isBreak = 0;
		while(!isBreak) {
			cout << "请确认是否继续(yes/no): ";
			string isGo;
			cin >> isGo;
			if(isGo == "no") {
				isExit = 1;
				isBreak = 1;
			}
			else if(isGo == "yes") {
				break;
			}
			else cout << "输入不合法，请重新输入！" << endl;
		}	
	}
}
// 基于Prim最小生成树的股票基金筛选
void analyseByPrim() {
	vis.assign(N+1, false);
	d.assign(N+1, INF);
	e.resize(N+1);
	
	ifstream in("股票基本信息数据/60支股票信息1.csv");
	string row;
	getline(in, row);
	
	while(getline(in, row)) {
		stringstream ss(row);
		string str;
		
		vector<string> arr;
		while(getline(ss, str, ',')) arr.push_back(str);
		//字符串转整数 https://chinese.freecodecamp.org/news/string-to-int-in-c-how-to-convert-a-string-to-an-integer-example/
		int dot1 = stoi(arr[0]);
		int dot2 = stoi(arr[1]);
		int dis = stoi(arr[2]);
		
		e[dot1].push_back({dot2, dis});
		e[dot2].push_back({dot1, dis});
	}
	MST();   //这时最小生成树已经生成了
	
	sort(ret.begin(), ret.end(), cmp);
	int num = ret.size();
	
	cout << "边的权值：" << ret[1].weight << " 边的结点1：" <<  scores[ret[1].fund1].name << " 边的结点2：" << scores[ret[1].fund2].name << endl;

	vector<fundPortfolio> arr; //权值为2的 
	for(int i = 0; i <= num; i++) {
		if(ret[i].weight == 2) arr.push_back(ret[i]);
	}
	
	int n = arr.size();
	for(int i = 0; i < n; i++) {
		arr[i].scoreSum = scores[arr[i].fund1].score + scores[arr[i].fund2].score;
	}
	sort(arr.begin(), arr.end(), cmpByScores);
	
	cout << "边的权值：" << arr[0].weight << " 边的结点1：" <<  scores[arr[0].fund1].name << " 边的结点2：" << scores[arr[0].fund2].name << endl;
	cout << "边的权值：" << arr[1].weight << " 边的结点1：" <<  scores[arr[1].fund1].name << " 边的结点2：" << scores[arr[1].fund2].name << endl;
	
	cout << "输入任意内容以继续 ";
	string isGo;
	cin >> isGo;
}
// Prim 朴素算法 
void MST() {
	d[1] = 0;   //最开始内部是一个空集
	int fund1 = 0;
	int fund2 = 0;
	
	//每一轮选一条内部到外部最小的边 
	for(int i = 1; i <= N; i++) {
		int min = INF;
		
		for(int k = 1; k <= N; k++) {
			if(!vis[k] && d[k] < min) {
				min = d[k];
				fund2 = k;
			}
		}
		
		for(int k = 1; k <= N; k++) {
			if(vis[k]) {
				int n = e[k].size();
				for(int j = 0; j < n; j++) {
					if(e[k][j].w == min && e[k][j].v == fund2) fund1 = k;
				}
			}
		}
		
		vis[fund2] = 1;
		
		for(const edge& item : e[fund2]) {
			int v = item.v, w = item.w;
			d[v] = d[v] < w ? d[v] : w;
		}
		
		struct fundPortfolio funds;
		funds.fund1 = fund1;
		funds.fund2 = fund2;
		funds.weight = min;
		
		ret.push_back(funds);
	}
}
// 基于Kruskal最小生成树的股票基金筛选
void anylyseByKruskal() {
	vector<Edge> es;
		
	ifstream in("股票基本信息数据/60支股票信息1.csv");
	string row;
	getline(in, row);
	
	while(getline(in, row)) {
		stringstream ss(row);
		string str;
		
		vector<string> arr;
		while(getline(ss, str, ',')) arr.push_back(str);
		//字符串转整数 https://chinese.freecodecamp.org/news/string-to-int-in-c-how-to-convert-a-string-to-an-integer-example/
		int dot1 = stoi(arr[0]);
		int dot2 = stoi(arr[1]);
		int dis = stoi(arr[2]);
		
		es.push_back({dot1, dot2, dis});
	}
	sort(es.begin(), es.end(), [](const Edge& x, const Edge& y) {
                return x.w < y.w;
    });
    
    int m = es.size();
    
    p.resize(m);
    for(int i = 0; i <= N; i++) p[i] = i;
    vector<fundPortfolio> res;
    
    for(int i = 0; i < m; i++) {
    	int u = es[i].u;
    	int v = es[i].v;
    	int w = es[i].w;
    	
    	int fu = find(u);
    	int fv = find(v);
    	
    	if(fu == fv) continue;
    	p[fu] = fv;
    	fundPortfolio tmp;
    	tmp.fund1 = fu;
    	tmp.fund2 = fv;
    	tmp.weight = w;
    	res.push_back(tmp);
	}
	cout << "边的权值：" << res[0].weight << " 边的结点1：" <<  scores[res[0].fund1].name << " 边的结点2：" << scores[res[0].fund2].name << endl;
	
	vector<fundPortfolio> arr; //权值为2的 
	for(int i = 0; i <= m; i++) {
		if(res[i].weight == 2) arr.push_back(res[i]);
	}
	
	int n = arr.size();
	for(int i = 0; i < n; i++) {
		arr[i].scoreSum = scores[arr[i].fund1].score + scores[arr[i].fund2].score;
	}
	sort(arr.begin(), arr.end(), cmpByScores);
	
	cout << "边的权值：" << arr[0].weight << " 边的结点1：" <<  scores[arr[0].fund1].name << " 边的结点2：" << scores[arr[0].fund2].name << endl;
	cout << "边的权值：" << arr[1].weight << " 边的结点1：" <<  scores[arr[1].fund1].name << " 边的结点2：" << scores[arr[1].fund2].name << endl;
	
	cout << "输入任意内容以继续 ";
	string isGo;
	cin >> isGo;
}
// 基于二部图的股票基金筛选
void analyseByBigraph() {
	
}
// 删除二叉排序树节点功能入口
void deleteBSTNode() {
	BinTree T = NULL;
	createBST(T, stocks); //创建完毕
	
	cout << "请输入要删除的股票代码：";
	string toDelete;
	cin >> toDelete;
	
	BinTree ret = NULL;
	BinTree node = deleteBST(T, toDelete, ret);
	printResult(ret);
	
	cout << "输入任意内容以继续 ";
	string isGo;
	cin >> isGo;
}
// 删除节点
BinTree deleteBST(BinTree& BST, string str, BinTree& ret) {
	BinTree ptr;
 
    if (BST == NULL)  //要删除的结点不存在
    	cout << "Not Found\n" << endl; 
	 
    else {					                  //要删除的结点存在
		if (str == BST->data.stockCode) {     //找到了要删除的结点
            if (BST->left && BST->right)      //左右子树都存在
            {
                ptr = FindMin(BST->right);    //选择右子树的最小结点
                BST->data = ptr->data;        //用右子树的最小结点取代被删除结点
                BST->right = deleteBST(BST->right, BST->data.stockCode, ret);      //删除原来的右子树的最小结点
            }
            else                              //结点的子树存在空树
            {
                if (BST->left == NULL)        //结点的左子树为空树(此时可能右结点也是空树)
                    BST = BST->right;
                else                          //结点的右子树为空树
                    BST = BST->left;
            }
            ret = BST;
	    }
	    //递归进入左子树查找删除结点
        else if(str < BST->data.stockCode) {
        	BST->left = deleteBST(BST->left, str, ret);
		}
		//递归进入右子树查找删除结点
        else {
        	BST->right = deleteBST(BST->right, str, ret);
		}
    }
    
    return BST;
}
// 查找最小值
BinTree FindMin(BinTree BST) {
    if (BST != NULL) {
        while (BST->left)      //沿着左子树向下挖掘
            BST = BST->left;
    }
    
    return BST;
}
// 打印最新节点和左右节点信息 
void printResult(BinTree node) {
	cout << "当前新节点为: " << endl;
	cout << node->data.shortName << "   " << node->data.stockCode << "   " << node->data.recentInfo[0] 
	<< "   " << node->data.recentInfo[1] << "   " << node->data.recentInfo[2] << endl << endl;
	
	if(node->left) {
		cout << "左孩子为：" << endl;
		cout << node->left->data.shortName << "   " << node->left->data.stockCode << "   " 
		<< node->left->data.recentInfo[0] << "   " << node->left->data.recentInfo[1] << "   " 
		<< node->data.recentInfo[2] << endl << endl;
	}

	else cout << "不存在左孩子" << endl << endl;
	
	if(node->right) {
		cout << "右孩子为：" << endl;
		cout << node->right->data.shortName << "   " << node->right->data.stockCode << "   " 
		<< node->right->data.recentInfo[0] << "   " << node->right->data.recentInfo[1] << "   " 
		<< node->right->data.recentInfo[2] << endl << endl;
	}
	
	else cout << "不存在右孩子" << endl << endl;
}














