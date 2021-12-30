// 头文件, 宏定义, 函数声明 

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "struct.h"
using namespace std;

#define INF 0x3f3f3f3f
#define N 60
#define SIZE 200
#define HASHLENGTH 97

vector<StockInfo> stocks(200);           //存储200支股票的基本信息和具体股票信息
vector<vector<edge>> e;                  //邻接表 
vector<int> d;                           //外部点到内部点的最短距离 
vector<int> vis;                         //标记数组
vector<vector<int>> graph(N+1, vector<int>(N+1, 0)); //60只股票邻接矩阵 
vector<fundPortfolio> ret;               //最小生成树当中的基金组合
vector<score> scores;                    //评分和名字
vector<int> p;                           //并查集数组 
vector<int> G[SIZE];                     //图
int color[SIZE];                         //顶点i的颜色 1 或 -1

// 工具函数 
int hashFun(string str);       //哈希函数
double hashASL(vector<StockInfo>, vector<hashList>); //求哈希表ASL 
double bstASL(BinTree, vector<StockInfo>); //求二叉排序树ASL 
bool kmp(string s, string t);  //KMP
string format(int number);     //格式化序号
bool cmp(const fundPortfolio t1, const fundPortfolio t2);         //比较函数 
bool cmpByScores(const fundPortfolio t1, const fundPortfolio t2); //比较函数 
int find(int x);               //find函数 

// 功能入口
void showSearchMenu();                    //展示查询功能菜单 
void showAnalyseMenu();                   //展示分析功能菜单 

// 具体功能实现 
void readStockInfo();                     //读文件并保存信息 
void createHashTable();                   //创建哈希表
void searchBaseInfo();                    //基于哈希搜索 
void searchWebsite();                     //基于KMP搜索网址
void searchByBST();	                      //基于二叉排序树查询
void createBST(BinTree&, vector<StockInfo>);                 //创建二叉排序树
void insertBST(BinTree&, int, vector<StockInfo>);            //二叉排序树插入节点 
BinTree searchBST(BinTree T, string key,double&); //搜索指定股票代码
void searchByLinkList();                  //基于单链表的股票价格信息查询
dataLink getLinkByDate(string date);      //根据日期获取链表

void analyseByInsertSort();               //通过插入排序来进行分析
void sortByOpeningPrice(dataLink);        //通过开盘价排序 
void sortByClosingPrice(dataLink);        //收盘价排序 
void sortByChangePercent(dataLink);       //涨跌幅排序 
void analyseByQuickSort();                //通过快排进行分析
void quickSort(vector<StockInfo>&, int, int); //快排 
int partition(vector<StockInfo>& industry, int low, int high);
void analyseBySelectionSort();            //基于简单选择排序的股票价格分析
void analyseByFloyd();                    //基于Floyd的股票相关性计算
void analyseByPrim();                     //基于Prim最小生成树的股票基金筛选
void analyseByBigraph();                  //基于二部图的股票基金筛选
void deleteBST(){}                        //基于二叉排序树的股票基本信息删除
void anylyseByKruskal();                  //基于Kruskal最小生成树的股票基金筛选
void MST();                               //最小生成树-普利姆算
int find(int x);                          //并查集查找
void deleteBSTNode();                     //删除二叉排序树节点功能入口
BinTree deleteBST(BinTree& BST, string str, BinTree& ret); //删除节点 
BinTree FindMin(BinTree BST);              //寻找最小值
void printResult(BinTree);             //打印新节点，左右节点信息
void showBestStocks(vector<int> arr1, vector<int> arr2); //展示二部图最佳股票组合 
bool solve(vector<int> v);             //二部图 
