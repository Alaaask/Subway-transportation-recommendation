#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<iterator>
#include<iomanip>

#define verticesmaxnum 1000

const float MAX=150000.0;

class graph;                                                              //graph的前向引用声明

class vertice {                                                           //vertice表示顶点
	friend class graph;
private:
	int x;                                                                //x,y为点的坐标
	int y;
public:
	vertice(int x=-1, int y=-1) {x=x; y=y;}
};

class graph {

private:
	vertice *verarray;                                                    //用数组表示地铁站点和起点、终点
	float **cost;                                                         //邻接矩阵，表示i,j两点之间的边的权值
	float *dist;                                                          //各点到起点的最小权值路径长度
	int *S;                                                               //判断各点是否已经加入集合S

	int currentnum;                                                       //当前点的个数
	int start, end;                                                       //起点和终点在verarray中的下标
	float result;                                                         //求出的最小权路径长度的结果
	int tag;                                                              //标识是求最短时间还是最短步行

	int insert(vertice v);                                                //尝试添加点v，不一定真的添加
	int find(vertice v);                                                  //查找点v的位置
	float getcost(int i, int j, bool b);                                  //得到不同情况下的权值cost

public:
	graph(std::vector<std::string> s);                                    //构造函数
	~graph();                                                             //析构函数
	void shortestpath();                                                  //求最小权路径长度result
	void print(std::ofstream &outfile);                                   //根据不同情况输出最终结果
};


graph::graph(std::vector<std::string> s) {                                //构造函数

	verarray=new vertice [verticesmaxnum];                                //给各指针分配内存
	S=new int [verticesmaxnum];                                           //因为verticesmaxnum=1000太大
	dist=new float [verticesmaxnum];
	cost=new float* [verticesmaxnum];

	for(int i=0; i<verticesmaxnum; i++)                                   //给二级指针cost分配空间第二步
		cost[i]=new float [verticesmaxnum];

			currentnum=0;                                                 //初始化currentnum

			std::stringstream ss;                                         //建立流
			std::vector<std::string>::iterator iter=s.begin();            //读取第一行

			int x, y, z, w;
			   
			ss<<*iter++;                                                  //第一行数据赋值给x,y,z,w,tag
			ss>>x>>y>>z>>w>>tag;                                          //(x,y)，(z,w)分别是起点，终点

			vertice v;                                                    //辅助变量v

			while(iter!=s.end()) {                                        //读取各行的地铁线

				int i,j;                                                  

				ss<<*iter++;                                              //建立流

				ss>>v.x>>v.y;                                             //读取地铁线的第一个点坐标
				i=insert(v);                                              //点(v.x,v.y)在verarray中的下标为i

				ss>>v.x>>v.y;                                             //读取第二个点坐标
				while(v.x!=-1&&v.y!=-1) {                                 //循环，遇到(-1,-1)停止
					j=insert(v);                                          //j是第"2"个点下标
					cost[j][i]=cost[i][j]=getcost(i,j,1);                 //调用getcost函数对cost二维数组赋值
					i=j;                                                  
					ss>>v.x>>v.y;
				}

				ss.clear();                                               //重要，清空流
			}

			v.x=x, v.y=y;                                                 //读取起点坐标
			start=insert(v);                                              //得到起点下标start
			 
			v.x=z, v.y=w;                                                 //读取终点坐标
			end=insert(v);                                                //得到终点下标end

			for(int i=0; i<currentnum; i++)                               //补充cost二维数组
				for(int j=i; j<currentnum; j++) {
					if(i==j) cost[i][j]=0.0;                              //对角线为0
					else if(cost[i][j]<0.0||cost[i][j]>MAX) 
						cost[i][j]=cost[j][i]=getcost(i,j,0);             //得到非地铁连接的两点的步行距离
				}

}

graph::~graph () {                                                        //析构函数

	for(int i=0; i<verticesmaxnum; i++)                                   //释放cost二维数组空间
		delete cost[i];                     

	delete cost,dist,S,verarray;                                          //释放其他数组空间
}

                                                                          //insert函数尝试将参数v加入verarray数组中
	                                                                      //如果v已经存在，则不再重复添加
	                                                                      //否则添加在verarray数组的最后
                                                                          //不管是否添加在最后，始终返回v在verarray中的下标
int graph::insert(vertice v) {                                           

	int i=find(v);                                                        //调用find函数查找v是否在verarray中已存在

	if(i!=currentnum) return i;                                           //已存在，返回v下标

	else {                                                                //不存在，添加在最后
		verarray[currentnum]=v;
		currentnum++;
	}

	return currentnum-1;                                                  //返回v下标
}

int graph::find(vertice v) {                                              //find函数在verarray中寻找参数v

	for(int i=0; i<currentnum; i++) 

		if(verarray[i].x==v.x&&verarray[i].y==v.y)
			return i;                                                     //找到，返回下标

	return currentnum;                                                    //否则返回下标

}


float graph::getcost(int i, int j, bool b) {                              //b=1 计算的是地铁连接的两点

	if(tag&&b) return 0.0;                                                //地铁上两点，且求的是最短时间，cost为0

	else {

	int sq=(verarray[i].x-verarray[j].x)*(verarray[i].x-verarray[j].x)    //求平方和
		    +(verarray[i].y-verarray[j].y)*(verarray[i].y-verarray[j].y);
	float distance=sqrt(float(sq));                                       //开方为距离

	if(!b) return distance;                                               //步行距离即为distance

	else return distance/(float)4.0;                                      //地铁站点间权值为distance的1\4

	}

}


void graph::shortestpath() {                                              //求最小权路径的长度

	int i, j, k; 
	float min;

	for(i=0; i<currentnum; i++) { 

		dist[i]=cost[start][i];                                           //复制矩阵第start行给dist数组

		S[i]=0;                                                           //S初始化

	}

	S[start]=1;                                                           //顶点start加入集合S

	for(i=0; i<currentnum; i++) {                                         //循环，最多currentnum次
		                                                                  //从顶点start确定路径
		min=MAX;
		k=-1;

		for(j=0; j<currentnum; j++)

			if(S[j]==0)                                                   //选择当前不在集合S中具有最短路径的顶点为k
				if(dist[j]!=0 && dist[j]<min) {
					min=dist[j];
					k=j;
				}

				if(k==end)  break;                                        //如果k为终点，则跳出循环

				S[k]=1;                                                   //点k加入集合S，表示它已在最短路径上

				for (j=0; j<currentnum; j++)

					if (S[j]==0)                                          //修改各dist
						if (dist[k]+cost[k][j]<dist[j]) 
							dist[j]=dist[k]+cost[k][j];

	}

	result=dist[k];                                                       //得到起点到终点的最小权路径的长度

}



void graph::print(std::ofstream &outfile) {

	if(!tag)                                                              //输出最短时间，精确到整数
		outfile<<std::setprecision(0)<<std::setiosflags(std::ios::fixed)
			   <<result*60.0/10000.0<<std::endl;

	else                                                                  //输出最短步行距离，精确到整数
		outfile<<std::setprecision(0)<<std::setiosflags(std::ios::fixed)
			   <<result<<std::endl;

}