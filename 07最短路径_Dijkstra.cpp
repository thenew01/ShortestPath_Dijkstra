#include "stdafx.h"
#include "stdio.h"    
#include "stdlib.h"   
#include "io.h"  
#include "math.h"  
#include "time.h"

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

#define MAXEDGE 20
#define MAXVEX 20
#define INFINITY 65535

typedef int Status;	/* Status是函数的类型,其值是函数结果状态代码，如OK等 */ 


typedef struct
{
	int vexs[MAXVEX];
	int arc[MAXVEX][MAXVEX];
	int numVertexes, numEdges;
}MGraph;

typedef int Patharc[MAXVEX];    /* 用于存储最短路径下标的数组 */
typedef int ShortPathTable[MAXVEX];/* 用于存储v0到各点最短路径的权值和 */

/* 构件图 */
void CreateMGraph(MGraph *G)
{
	int i, j;

	/* printf("请输入边数和顶点数:"); */
	G->numEdges=16;
	G->numVertexes=9;

	for (i = 0; i < G->numVertexes; i++)/* 初始化图 */
	{
		G->vexs[i]=i;
	}

	for (i = 0; i < G->numVertexes; i++)/* 初始化图 */
	{
		for ( j = 0; j < G->numVertexes; j++)
		{
			if (i==j)
				G->arc[i][j]=0;
			else
				G->arc[i][j] = G->arc[j][i] = INFINITY;
		}
	}

	G->arc[0][1]=1;
	G->arc[0][2]=5; 
	G->arc[1][2]=3; 
	G->arc[1][3]=7; 
	G->arc[1][4]=5; 

	G->arc[2][4]=1; 
	G->arc[2][5]=7; 
	G->arc[3][4]=2; 
	G->arc[3][6]=3; 
	G->arc[4][5]=3;

	G->arc[4][6]=6;
	G->arc[4][7]=9; 
	G->arc[5][7]=5; 
	G->arc[6][7]=2; 
	G->arc[6][8]=7;

	G->arc[7][8]=4;


	for(i = 0; i < G->numVertexes; i++)
	{
		for(j = i; j < G->numVertexes; j++)
		{
			G->arc[j][i] =G->arc[i][j];
		}
	}

}

int findShortest(MGraph G, int book[], ShortPathTable *weights, int& min)
{
	int k = -1;
	min=INFINITY;    /* 当前所知离v0顶点的最近距离 */        
	for(int w=0; w<G.numVertexes; w++) /* 寻找离v0最近的顶点 */    
	{            
		if(!book[w] && (*weights)[w]<min)             
		{                   
			k=w;                    
			min = (*weights)[w];    /* w顶点离v0顶点更近 */            
		}        
	}        
	return k;
}

/*  Dijkstra算法，求有向网G的v0顶点到其余顶点v的最短路径Path[v]及带权长度weights[v] */    
/*  Path[v]的值为前驱顶点下标,weights[v]表示v0到v的最短路径长度和 */  
void ShortestPath_Dijkstra(MGraph G, int v0, Patharc *Path, ShortPathTable *weights)
{    
	int v,w,k,min;    
	int book[MAXVEX];/* book[w]=1表示求得顶点v0至vw的最短路径 */
	for(v=0; v<G.numVertexes; v++)    /* 初始化数据 */
	{        
		book[v] = 0;			/* 全部顶点初始化为未知最短路径状态 */
		(*weights)[v] = G.arc[v0][v];/* 将与v0点有连线的顶点加上权值 */
		(*Path)[v] = -1;				/* 初始化路径数组Path为-1  */       
	}

	(*weights)[v0] = 0;  /* v0至v0路径为0 */  
	book[v0] = 1;    /* v0至v0不需要求路径 */        
	/* 开始主循环，每次求得v0到某个v顶点的最短路径 */   
	for(v=1; v<G.numVertexes; v++)   
	{
		int min = 0;
		int k = findShortest(G, book, weights, min); //查找到起点v0的距离最短的（未访问过的）节点
		book[k] = 1;    /* 将目前找到的最近的顶点置为1 */

		//查找从顶点k到其他（未访问过的）点的最短距离
		for(w=0; w<G.numVertexes; w++) /* 修正当前最短路径及距离 */
		{
			/* 如果经过v顶点的路径比现在这条路径的长度短的话 */
			if(!book[w] && (min+G.arc[k][w]<(*weights)[w]))   
			{ /*  说明找到了更短的路径，修改weights[w]和Path[w] */
				(*weights)[w] = min + G.arc[k][w];  /* 修改当前路径长度 */               
				(*Path)[w]=k;    //顶点w经停顶点k，顶点w的前驱是顶点k    
			}       
		}   
	}
}

int main(void)
{   
	int i,j,v0;
	MGraph G;    
	Patharc Path;    
	ShortPathTable weights; /* 求某点到其余各点的最短路径 */   
	v0=0;
	
	CreateMGraph(&G);
	
	ShortestPath_Dijkstra(G, v0, &Path, &weights);  

	printf("最短路径倒序如下:\n");    
	for(i=1;i<G.numVertexes;++i)   
	{       
		printf("v%d - v%d : ",v0,i);
		j=i;
		while(Path[j]!=-1)
		{
			printf("%d ",Path[j]);
			j=Path[j];
		}
		printf("\n");
	}    
	printf("\n源点到各顶点的最短路径长度为:\n");  
	for(i=1;i<G.numVertexes;++i)        
		printf("v%d - v%d : %d \n",G.vexs[0],G.vexs[i],weights[i]);     
	return 0;
}
