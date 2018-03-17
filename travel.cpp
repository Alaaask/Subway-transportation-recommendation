#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include"graph.h"

using namespace std;
int main(void) {

	ifstream inFile("input.txt");                                      //打开input文件
	if(!inFile) { cout<<"Failed in opening input.txt."<<endl; }

	ofstream outFile("output.txt");                                    //打开output文件
	if(!outFile) { cout<<"Failed in opening output.txt."<<endl; }

	int N, n;                                                          //N是测试数据的组数
	inFile>>N;
	inFile.ignore(1024,'\n');                                          //忽略换行符

	while(N--) {                                                       //进行N次循环

		vector<string> s;                                              //s记录每组测试数据
	    string temp;

		getline(inFile,temp);                                          //读取每组数据的第一行并压入s
		s.push_back(temp);

		inFile>>n;                                                     //读取地铁线的条数                      
		inFile.ignore(1024,'\n');

		while(n--) {                                                   //读取n条地铁线并压入s
			getline(inFile,temp);
			s.push_back(temp);
		}

		graph route(s);                                                //将刚读取的一组数据s传递给graph并建立图route

		route.shortestpath();                                          //计算最小权路程

		route.print(outFile);                                          //计算最终结果并输出

	}

	inFile.close();                                                    //关闭输入输出文件
	outFile.close();


	return 0;

}

