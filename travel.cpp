#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include"graph.h"

using namespace std;
int main(void) {

	ifstream inFile("input.txt");                                      //��input�ļ�
	if(!inFile) { cout<<"Failed in opening input.txt."<<endl; }

	ofstream outFile("output.txt");                                    //��output�ļ�
	if(!outFile) { cout<<"Failed in opening output.txt."<<endl; }

	int N, n;                                                          //N�ǲ������ݵ�����
	inFile>>N;
	inFile.ignore(1024,'\n');                                          //���Ի��з�

	while(N--) {                                                       //����N��ѭ��

		vector<string> s;                                              //s��¼ÿ���������
	    string temp;

		getline(inFile,temp);                                          //��ȡÿ�����ݵĵ�һ�в�ѹ��s
		s.push_back(temp);

		inFile>>n;                                                     //��ȡ�����ߵ�����                      
		inFile.ignore(1024,'\n');

		while(n--) {                                                   //��ȡn�������߲�ѹ��s
			getline(inFile,temp);
			s.push_back(temp);
		}

		graph route(s);                                                //���ն�ȡ��һ������s���ݸ�graph������ͼroute

		route.shortestpath();                                          //������СȨ·��

		route.print(outFile);                                          //�������ս�������

	}

	inFile.close();                                                    //�ر���������ļ�
	outFile.close();


	return 0;

}

