// 所有结构体信息 

#include <string>
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
// 边结构
struct edge {
	int v, w;
};
// 边结构，不需要邻接表 
struct Edge {
	int u, v, w;
};
// MST中的边以及连接两条边的点，即基金组合 
struct fundPortfolio {
	int fund1;
	int fund2;
	int weight;
	double scoreSum;
};
struct score {
	string name;
	double score;
};
