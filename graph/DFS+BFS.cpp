#include <iostream>
#include <queue>
using namespace std;
// 定义边表结点
struct ArcNode
{
    int dest;// 邻接点域
    ArcNode* link;
};
// 定义顶点表结点
template<class T>
struct VertexNode
{
    T data;
    ArcNode* adj;
};

// 基于邻接表存储结构的图的类实现
const int MaxSize = 10;
int visited[MaxSize] = { 0 };// 顶点是否被访问的标记



//typedef VertexNode AdjList[MaxSize];    //邻接表
template<class T>
class ALGraph
{
public:
    ALGraph(T a[], int n, int e);// 构造函数建立具有N个定点e条边的图
    ~ALGraph() {}// 析构函数
    void DFSTraaverse(int v);// 深度优先遍历图
    void BFSTraverse(int v);// 广度优先遍历图
private:
    VertexNode<T> adjlist[MaxSize];// 存放顶点的数组
    int vertexNum, arcNum;// 图中顶点数和边数
};


template<class T>
ALGraph<T>::ALGraph(T a[], int n, int e)
{
    vertexNum = n;
    arcNum = e;
    for (int i = 0; i <vertexNum; i++)
    {
        adjlist[i].data = a[i];
        adjlist[i].adj = NULL;
    }
    for (int k = 0; k < arcNum; k++)
    {
        int i, j;
        cout << "please input the line:";
        cin >> i >> j;
        ArcNode* s = new ArcNode;
        s->dest = j;
        s->link = adjlist[i].adj;
        adjlist[i].adj = s;
        ArcNode* p = new ArcNode;
        p->dest = i;
        p->link = adjlist[j].adj;
        adjlist[j].adj = p;
    }
}

template<class T>
inline void ALGraph<T>::DFSTraaverse(int v)
{
    std::cout << adjlist[v].data;
    visited[v] = 1;
    ArcNode* p = adjlist[v].adj;
    while (p != NULL)
    {
        int j = p->dest;
        if (visited[j] == 0)
            DFSTraaverse(j);
        p = p->link;
    }
}

template<class T>
inline void ALGraph<T>::BFSTraverse(int v)
{
    int visited[MaxSize] = { 0 };// 顶点是否被访问的标记
    queue<T> Q;  // 初始化队列
    cout << adjlist[v].data;
    visited[v] = 1;
    Q.push(v);// 被访问顶点入队
    while (!Q.empty())
    {
        v= Q.front();
        Q.pop();    // 对头元素出队
        ArcNode* p = adjlist[v].adj;
        while (p != NULL)
        {
            int j = p->dest;
            if (visited[j] == 0)
            {
                cout << adjlist[j].data;
                visited[j] = 1;
                Q.push(j);
            }
            p = p->link;
        }
    }
}
// main.cpp



using namespace std;
int main()
{
    char a[10];
    char c;
    int vertexNum,arcNum;
    cout << "please input the number of your vertex:";
    cin >> vertexNum;
    cout << "please input the name of your vertex:";
    for(int i=0;i<vertexNum;i++){
        cin >> c;
        a[i] = c;
    }
    cout << "please input the number of your arc:";
    cin >> arcNum;
    ALGraph<char> graph(a, vertexNum, arcNum);
    graph.BFSTraverse(0);
    cout << endl;
    graph.DFSTraaverse(0);
    
    return 0;
}
