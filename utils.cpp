#include "general.h"

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
// 权值排序规则
bool cmp(const fundPortfolio t1, const fundPortfolio t2) {
	if(t1.weight < t2.weight) return true;
	return false;
} 
// 总评分 
bool cmpByScores(const fundPortfolio t1, const fundPortfolio t2) {
	if(t1.scoreSum > t2.scoreSum) return true;
	return false;
} 
// 并查集查找
int find(int x) {
	return x = p[x] ? x : p[x] = find(p[x]);
} 
