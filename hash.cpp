#include<stdio.h>
#include<time.h>//time用到的头文件
#include<stdlib.h>//随机数srand用到的头文件
#include<ctype.h>//toascii()用到的头文件
#include<string.h>//查找姓名时比较字符串用的头文件
#define HASH_LEN 50//哈希表的长度
#define P 47//小于哈希表长度的P
#define NAME_LEN 30//姓名表的长度

typedef struct//姓名表
{
    char *key;    //名字的拼音
    int m;       //拼音所对应的ASCII和
}NAME;

typedef struct//哈希表
{
    char *key;   //名字的拼音
    int m;      //拼音所对应的ASCII总和，即关键字
    int si;     //查找长度
}HASH;

NAME Name[HASH_LEN];        //全局定义姓名表,最大长度为50
HASH Hash[HASH_LEN];        //全局定义哈希表，最大长度为50
int d[30],i,j;    //全局定义随机数,循环用的i、j

void InitName()
{//姓名表的初始化
    Name[0].key="lvsongxian";
    Name[1].key="yuanlei";
    Name[2].key="daiziwen";
    Name[3].key="chenyonghui";
    Name[4].key="zhangliang";
    Name[5].key="liubei";
    Name[6].key="sunshangxiang";
    Name[7].key="liyuanfang";
    Name[8].key="huge";
    Name[9].key="liuyifei";
    Name[10].key="anyixuan";
    Name[11].key="wangbaoqiang";
    Name[12].key="yangyiming";
    Name[13].key="hujing";
    Name[14].key="guowen";
    Name[15].key="xuyang";
    Name[16].key="lilu";
    Name[17].key="shenjinfeng";
    Name[18].key="xuhui";
    Name[19].key="huangjing";
    Name[20].key="guanyu";
    Name[21].key="chenlong";
    Name[22].key="huangliang";
    Name[23].key="liyan";
    Name[24].key="haojian";
    Name[25].key="zhangfei";
    Name[26].key="shuxiang";
    Name[27].key="sunyingjie";
    Name[28].key="wangbo";
    Name[29].key="zhaoqing";
for (i=0;i<NAME_LEN;i++)    //将字符串的各个字符所对应的ASCII码相加，所得的整数做为哈希表的关键字
{
    int s=0;
        char *p=Name[i].key;
    for (j=0;*(p+j)!='\0';j++)
        s+=toascii(*(p+j));
        Name[i].m=s;
}
}

void CreateHash()
{//建立哈希表
       for(i=0;i<HASH_LEN;i++) //清空哈希表，未经此操作将储存空数据
       {
              Hash[i].key="\0";
              Hash[i].m =0;
              Hash[i].si=0;
       }
       for(i=0;i<NAME_LEN;i++)
       {
              int sum=1,j=0,t;
              int adr=(Name[i].m)%P;  //除留余数法H(key)=key%P，除数为P=47
              if(Hash[adr].si==0)     //如果不冲突，将姓名表赋值给哈希表
              {
                     Hash[adr].m =Name[i].m;
                     Hash[adr].key=Name[i].key;
                     Hash[adr].si=1;
              }
              else                         //如果冲突
              {
                  t=adr;        //线性探测法处理冲突
                 for(;Hash[adr].si!=0&&adr<HASH_LEN;adr++)//从冲突下一个位置开始探测
                     {
                            sum=sum+1;//每次查找，查找次数+1
                            if(adr==HASH_LEN-1)//如果找到最后一个仍然没有位置
                            {
                                for(;Hash[adr].si!=0&&adr<t;adr++)//从第一个开始探测
                                 sum=sum+1;//每次查找，查找次数+1
                                 if(adr==t) printf("哈希表已满\n");//如果找到上次的位置仍然没有，则输出哈希表已满
                            }
                     }
                     Hash[adr].m =Name[i].m;  //将姓名表复制给哈希表对应的位置上
                     Hash[adr].key=Name[i].key;
                     Hash[adr].si=sum;
              }
       }
}

void DisplayName()//显示姓名表
{
       printf("\n地址 \t\t 姓名 \t\t 关键字\n");
       for (i=0;i<NAME_LEN;i++)
       printf("%2d %18s \t\t  %d  \n",i,Name[i].key,Name[i].m);
}

void DisplayHash()// 显示哈希表
{
       float asl=0.0;
       printf("\n\n 地址 \t\t 姓名 \t\t 关键字 \t 搜索长度\n"); //显示的格式
       for (i=0;i<HASH_LEN;i++)
       {
              printf("%2d %18s \t\t  %d \t\t  %d\n",i,Hash[i].key,Hash[i].m,Hash[i].si);
              asl+=Hash[i].si;
       }
       asl/=NAME_LEN;//求得ASL
       printf("\n\n平均查找长度：ASL(%d)=%f \n",NAME_LEN,asl);
}

void FindName()//查找
{
    char name[20]={0};
    int s=0,sum=1,adr;
    printf("\n请输入想要查找的姓名的拼音:");
    scanf("%s",name);
    getchar();
    for (j=0;j<20;j++)//求出姓名的拼音所对应的ASCII作为关键字
        s+=toascii(name[j]);
    adr=s%P;           //除留余数法
    j=0;
    if(Hash[adr].m==s&&!strcmp(Hash[adr].key,name)) //分3种情况进行判断，并输出查找结果
        printf("\n姓名:%s   关键字:%d   地址:%d   查找长度为: 1\n",Hash[adr].key,s,adr);
    else if (Hash[adr].m==0)
        printf("\n没有想要查找的人!\n");
    else
       {
            while(1)
              {
                  adr=(adr+d[j++])%HASH_LEN;//伪随机探测再散列法处理冲突
                  sum=sum+1;                //查找次数加1
                  if(Hash[adr].m==0)
                     {
                        printf("\n没有想要查找的人!\n");
                        break;
                     }
                     if(Hash[adr].m==s&&!strcmp(Hash[adr].key,name))
                     {
                        printf("\n姓名:%s   关键字:%d   地址:%d   查找长度为:%d\n",Hash[adr].key,adr,s,sum);
                        break;
                     }
              }
       }
}

void view()//交互界面
{
     printf("=======================================================\n");
    printf("=                                                     =\n");
    printf("=                   人名哈希表                        =\n");
    printf("=                                                     =\n");
    printf("=  A: 打印姓名表                 B: 打印哈希表        =\n");
    printf("=                                                     =\n");
    printf("=  C: 查找                       D: 退出程序          =\n");
    printf("=                                                     =\n");
    printf("=======================================================\n");
}

int main()//主函数
{
    char c;
    int a=1;
    srand((int)time(0));//以当前时间对应的int值为随机序列起点，每次运行程序由于起点不同可以得到不同的随机数序列
    for(i=0;i<30;i++)//用随机函数求得伪随机数列d[i]（在1到50之间）
        d[i]=1+(int)(HASH_LEN*rand()/(RAND_MAX+1.0));
    InitName();//调用初始化姓名表函数
    CreateHash();//调用创建哈希表函数
    view();//调用交互界面函数
    while(a)
    {
       printf("\n输入选项:");
       scanf("%c",&c);
       getchar();
       switch(c)//根据选择进行判断，直到选择退出时才可以退出
       {
           case 'A':
           case 'a': DisplayName(); break;//打印姓名表
           case 'B':
           case 'b': DisplayHash(); break;//打印哈希表
           case 'C':
           case 'c': FindName(); break;//调用查找函数
           case 'D':
           case 'd': a=0; break;//退出循环，终止程序
           default: printf("\n请输入正确的选择!\n"); break;
       }
    }
    return 0;
}

