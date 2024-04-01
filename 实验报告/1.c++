#include <iostream>
#include <string.h>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;
#define chusize 0 //初始的表长
#define a 0.9     //装载因子0.1, 0.3, 0.5, 0.7, 0.9
struct Node       //存储学号和姓名
{
    string ID, name;
    Node() { ID = "\0", name = "\0"; };
    Node(string id, string na)
    {
        ID = id;
        name = na;
    };
};
class student
{
private:
public:
    int size = 0;                     //表的固定大小
    int lenth = 0;                    //存储的人数
    void Add();                       // 0是插入
    void Delete();                    // 1是删除
    Node *person = new Node[chusize]; //动态分配,一次一起NEW的分配的内存是连续的
};
string idqian[4] = {"1018", "1019", "1020", "1021"};
string ID_(int lenth);
string name_(int lenth);
int main()
{
    srand((unsigned)time(NULL));
    student stu;
    int N; //操作数的个数，即数据规模
    cin >> N;
    int CaoZuo; //插入操作还是删除操作
    double start1 = clock();
    for (int i = 0; i < 100; i++)
    {
        ifstream in;
        in.open("caozuo.txt");
        if (!in)
        {
            cout << "can not open" << endl;
        }
        else
        {
            for (int i = 1; i <= N; i++)
            {
                in >> CaoZuo;
                if (CaoZuo == 0)
                {
                    stu.Add();
                }
                else if (CaoZuo == 1)
                {
                    stu.Delete();
                }
            }
            // cout << "open Successfuly" << endl;
        }
        in.close();
    }
    double end1 = clock();
    double diff1 = (double)(end1 - start1) / CLOCKS_PER_SEC;
    cout << "Time:" << diff1 / 100 << endl
         << "to the end";
    return 0;
}
string ID_(int lenth)
{
    string str = idqian[(rand() % 4 + 0)]; //{"1018","1019","1020","1021"}前缀选一个
    string c;
    for (int i = 1; i <= lenth; i++)
    {
        c = to_string((rand() % 9 + 0));
        str += c;
    }
    return str;
}
string name_(int lenth)
{
    string str;
    char c;
    for (int i = 0; i < lenth; i++)
    {
        c = 'a' + rand() % 26; //随即生成字符
        str.push_back(c);      //拼接成字符串
    }
    return str;
}
void student::Add()
{
    if (size == 0) //表的大小为0，分配一个槽
    {
        size = 1;
        person = new Node[size];
    }
    if (size == lenth) //表的槽全部占满，分配新表
    {
        Node *per = person;          //定义一个新指针指向原来的满表
        person = new Node[size * 2]; //将原来class里的旧指针指向新分配的表
        for (int i = 0; i < lenth; i++)
        {
            person[i] = per[i]; //将原来的表赋值给新表
        }
        delete[] per; 
        size *= 2;    //新表的尺度是原来的2倍
    }
    person[lenth].ID = ID_(7); 
    person[lenth].name = name_(11);
    lenth++;
}
void student::Delete()
{
    //注意delete中lenth的变化，每次lenth-1存储的才是最后一个值
    if (lenth == 0 || size == 0) // lenth-1 = -1没有表，不用释放
    {
        lenth = 0;
        size = 0;
        return;
    }
    if ((lenth - 1) == 0) //表为空，释放表
    {
        delete[] person;
        lenth = 0;
        size = 0;
        return;
    }
    else if (((double)(lenth)) / size <= a) //表的槽闲置太多，分配新表
    {
        Node *per = person; //定义两个新指针指向原来的有很多闲置空间的表
        int newsize = ceil(size * a);
        person = new Node[newsize]; //将原来class里的旧指针指向新分配的表
        for (int i = 0; i < lenth; i++)
        {
            person[i] = per[i]; //将原来的表赋值给新表
        }
        delete[] per;
        size = newsize; //新表的尺度是原来的1/2
    }
    person[lenth - 1] = Node(); //置为空，完成删除
    lenth--;
}