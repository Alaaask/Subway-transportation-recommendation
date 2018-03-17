#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<iterator>
#include<iomanip>

#define verticesmaxnum 1000

const float MAX=150000.0;

class graph;                                                              //graph��ǰ����������

class vertice {                                                           //vertice��ʾ����
	friend class graph;
private:
	int x;                                                                //x,yΪ�������
	int y;
public:
	vertice(int x=-1, int y=-1) {x=x; y=y;}
};

class graph {

private:
	vertice *verarray;                                                    //�������ʾ����վ�����㡢�յ�
	float **cost;                                                         //�ڽӾ��󣬱�ʾi,j����֮��ıߵ�Ȩֵ
	float *dist;                                                          //���㵽������СȨֵ·������
	int *S;                                                               //�жϸ����Ƿ��Ѿ����뼯��S

	int currentnum;                                                       //��ǰ��ĸ���
	int start, end;                                                       //�����յ���verarray�е��±�
	float result;                                                         //�������СȨ·�����ȵĽ��
	int tag;                                                              //��ʶ�������ʱ�仹����̲���

	int insert(vertice v);                                                //������ӵ�v����һ��������
	int find(vertice v);                                                  //���ҵ�v��λ��
	float getcost(int i, int j, bool b);                                  //�õ���ͬ����µ�Ȩֵcost

public:
	graph(std::vector<std::string> s);                                    //���캯��
	~graph();                                                             //��������
	void shortestpath();                                                  //����СȨ·������result
	void print(std::ofstream &outfile);                                   //���ݲ�ͬ���������ս��
};


graph::graph(std::vector<std::string> s) {                                //���캯��

	verarray=new vertice [verticesmaxnum];                                //����ָ������ڴ�
	S=new int [verticesmaxnum];                                           //��Ϊverticesmaxnum=1000̫��
	dist=new float [verticesmaxnum];
	cost=new float* [verticesmaxnum];

	for(int i=0; i<verticesmaxnum; i++)                                   //������ָ��cost����ռ�ڶ���
		cost[i]=new float [verticesmaxnum];

			currentnum=0;                                                 //��ʼ��currentnum

			std::stringstream ss;                                         //������
			std::vector<std::string>::iterator iter=s.begin();            //��ȡ��һ��

			int x, y, z, w;
			   
			ss<<*iter++;                                                  //��һ�����ݸ�ֵ��x,y,z,w,tag
			ss>>x>>y>>z>>w>>tag;                                          //(x,y)��(z,w)�ֱ�����㣬�յ�

			vertice v;                                                    //��������v

			while(iter!=s.end()) {                                        //��ȡ���еĵ�����

				int i,j;                                                  

				ss<<*iter++;                                              //������

				ss>>v.x>>v.y;                                             //��ȡ�����ߵĵ�һ��������
				i=insert(v);                                              //��(v.x,v.y)��verarray�е��±�Ϊi

				ss>>v.x>>v.y;                                             //��ȡ�ڶ���������
				while(v.x!=-1&&v.y!=-1) {                                 //ѭ��������(-1,-1)ֹͣ
					j=insert(v);                                          //j�ǵ�"2"�����±�
					cost[j][i]=cost[i][j]=getcost(i,j,1);                 //����getcost������cost��ά���鸳ֵ
					i=j;                                                  
					ss>>v.x>>v.y;
				}

				ss.clear();                                               //��Ҫ�������
			}

			v.x=x, v.y=y;                                                 //��ȡ�������
			start=insert(v);                                              //�õ�����±�start
			 
			v.x=z, v.y=w;                                                 //��ȡ�յ�����
			end=insert(v);                                                //�õ��յ��±�end

			for(int i=0; i<currentnum; i++)                               //����cost��ά����
				for(int j=i; j<currentnum; j++) {
					if(i==j) cost[i][j]=0.0;                              //�Խ���Ϊ0
					else if(cost[i][j]<0.0||cost[i][j]>MAX) 
						cost[i][j]=cost[j][i]=getcost(i,j,0);             //�õ��ǵ������ӵ�����Ĳ��о���
				}

}

graph::~graph () {                                                        //��������

	for(int i=0; i<verticesmaxnum; i++)                                   //�ͷ�cost��ά����ռ�
		delete cost[i];                     

	delete cost,dist,S,verarray;                                          //�ͷ���������ռ�
}

                                                                          //insert�������Խ�����v����verarray������
	                                                                      //���v�Ѿ����ڣ������ظ����
	                                                                      //���������verarray��������
                                                                          //�����Ƿ���������ʼ�շ���v��verarray�е��±�
int graph::insert(vertice v) {                                           

	int i=find(v);                                                        //����find��������v�Ƿ���verarray���Ѵ���

	if(i!=currentnum) return i;                                           //�Ѵ��ڣ�����v�±�

	else {                                                                //�����ڣ���������
		verarray[currentnum]=v;
		currentnum++;
	}

	return currentnum-1;                                                  //����v�±�
}

int graph::find(vertice v) {                                              //find������verarray��Ѱ�Ҳ���v

	for(int i=0; i<currentnum; i++) 

		if(verarray[i].x==v.x&&verarray[i].y==v.y)
			return i;                                                     //�ҵ��������±�

	return currentnum;                                                    //���򷵻��±�

}


float graph::getcost(int i, int j, bool b) {                              //b=1 ������ǵ������ӵ�����

	if(tag&&b) return 0.0;                                                //���������㣬����������ʱ�䣬costΪ0

	else {

	int sq=(verarray[i].x-verarray[j].x)*(verarray[i].x-verarray[j].x)    //��ƽ����
		    +(verarray[i].y-verarray[j].y)*(verarray[i].y-verarray[j].y);
	float distance=sqrt(float(sq));                                       //����Ϊ����

	if(!b) return distance;                                               //���о��뼴Ϊdistance

	else return distance/(float)4.0;                                      //����վ���ȨֵΪdistance��1\4

	}

}


void graph::shortestpath() {                                              //����СȨ·���ĳ���

	int i, j, k; 
	float min;

	for(i=0; i<currentnum; i++) { 

		dist[i]=cost[start][i];                                           //���ƾ����start�и�dist����

		S[i]=0;                                                           //S��ʼ��

	}

	S[start]=1;                                                           //����start���뼯��S

	for(i=0; i<currentnum; i++) {                                         //ѭ�������currentnum��
		                                                                  //�Ӷ���startȷ��·��
		min=MAX;
		k=-1;

		for(j=0; j<currentnum; j++)

			if(S[j]==0)                                                   //ѡ��ǰ���ڼ���S�о������·���Ķ���Ϊk
				if(dist[j]!=0 && dist[j]<min) {
					min=dist[j];
					k=j;
				}

				if(k==end)  break;                                        //���kΪ�յ㣬������ѭ��

				S[k]=1;                                                   //��k���뼯��S����ʾ���������·����

				for (j=0; j<currentnum; j++)

					if (S[j]==0)                                          //�޸ĸ�dist
						if (dist[k]+cost[k][j]<dist[j]) 
							dist[j]=dist[k]+cost[k][j];

	}

	result=dist[k];                                                       //�õ���㵽�յ����СȨ·���ĳ���

}



void graph::print(std::ofstream &outfile) {

	if(!tag)                                                              //������ʱ�䣬��ȷ������
		outfile<<std::setprecision(0)<<std::setiosflags(std::ios::fixed)
			   <<result*60.0/10000.0<<std::endl;

	else                                                                  //�����̲��о��룬��ȷ������
		outfile<<std::setprecision(0)<<std::setiosflags(std::ios::fixed)
			   <<result<<std::endl;

}