## 图

### 定义

- `连通图与连通分量`： 在无向图中，若从顶点v1到顶点v2有路径，则称顶点v1与v2是连通的。如果图中任意一对顶点都是连通的，则称此图是【连通图】。非连通图的极大连通子图叫做【连通分量】。
- `生成树`：  一个连通图的生成树是其【极小连通子图】，在n个顶点的情形下，有n-1条边。

#### 无向图

- `连通图和非联通图`： 
  - 如果无向图 G 中==任意一对顶点都是连通==的，则称此图是连通图（connected graph）；
  - 相反，如果一个无向图不是连通图，则称为非连通图（disconnected graph）。
    - 对非连通图G，其极大连通子图称为`连通分量（connected component，或连通分支）`，连通分支数记为`w(G)`。
- `割顶集与连通度`： 设V’是连通图G 的一个顶点子集，在G 中删去V’及与V’关联的边后图==不连通==，则称 V’ 是 G 的`割顶集（vertex-cut set）`。
  - 如果割顶集V’的任何真子集都不是割顶集，则称V’为`极小割顶集`。
  - 顶点个数最小的极小割顶集称为`最小割顶集`。
  - 最小割顶集中顶点的个数，称作图G 的`顶点连通度（vertex connectivity degree）`，记做κ(G)，且称图G 是`κ–连通图（κ–connected graph）`。
- `割点（articulation node）`：如果割顶集中只有一个顶点，则该顶点可以称为割点（cut-vertex），或关节点。
- `割边集与边连通度`：设 E’ 是连通图 G 的边集的子集，在 G 中删去E’后图不连通，则称E’是G 的割边集 （edge-cut set）。
  - 如果割边集 E’ 的任何真子集都不是割边集，则称 E’ 为`极小割边集`。
  - 边数最小的极 小割边集称为`最小割边集`。
  - 最小割边集中边的个数，称作图G 的`边连通度（edge connectivity degree）`，记做λ(G)，且称图G 是`λ–边连通图（λ–edge–connected graph）`。
- `割边`：如果割边集中只有一条边，则该边可以称为`割边（bridge）`，或桥。

#### 有向图

- `强连通（strongly connected）`：若 G 是有向图，如果对图 G 中任意两个顶点 u 和 v，==既存在从 u 到 v 的路径，也存在从 v 到 u 的路径==，则称该有向图为强连通有向图。对于非强连通图，其极 大强连通子图称为其`强连通分量`。
- `单连通（simply connected）`：若 G 是有向图，如果对图 G 中任意两个顶点 u 和 v，==存在从 u 到 v 的路径或从 v 到 u 的路径==，则称该有向图为单连通有向图。
- `弱连通（weak connected）`：若 G 是有向图，如果忽略图 G 中每条有向边的方向，==得到的无向图（即有向图的基图）连通==，则称该有向图为弱连通有向图。

#### 表示

##### 邻接矩阵

<img src="/Users/apple/Library/Application Support/typora-user-images/image-20200422193320218.png" alt="image-20200422193320218" style="zoom:50%;" />

##### 邻接表

<img src="/Users/apple/Library/Application Support/typora-user-images/image-20200422193501725.png" alt="image-20200422193501725" style="zoom:50%;" />

##### 遍历

DFS+BFS代码见[文件](DFS+BFS.cpp)

- 性能分析

  - DFS

    ```c++
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
    ```

    > - 如果用邻接表表示图，时间复杂度为O(n+e)
    >   - 查找所有顶点的邻接点所需时间为O(E)，访问顶点的邻接点所花时间为O（V）
    >   - 沿邻接表可以依次取出顶点v的所有邻接顶点。
    >     - 共有2e个边结点
    >   - 每个顶点只访问一次
    > - 如果用邻接矩阵表示，时间复杂度为O($n^2$)
    >   - 查找一个顶点的所有边需要O(n)
    >   - 需要访问n个顶点

  - BFS

    ```c++
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
    ```

    > - 邻接表表示，时间复杂度为O(n+e)
    >   - 每个结点需要进队列一次
    >   - 每个结点的每条边要检查一次，共O(e)次
    > - 邻接矩阵表示， 时间复杂度为O($n^2$)

### 一些经典算法

#### 最小生成树

##### (1)Prim算法

- 主要思想：

  <img src="/Users/apple/Library/Application Support/typora-user-images/image-20200422201640455.png" alt="image-20200422201640455" style="zoom:50%;" />

  可用反证法证明。

  > 证明：用反证法证明上面提出的结论。
  >        设G中的任何一棵包含T的最小生成树都不包含边(u, v)，且设T’就是这样的生成树。因为T’是树，所以它是连通的，从u到v必有一条路径(u, ..., v)，路径(u, ..., v)中必有边(u’, v’)，满足u’∈U, v’∈V-U。
  >        把(u, v)加入T’中，就构成一条回路，由假设，边(u, v)的代价小于边(u’, v’)的代价（因为边(u, v)是连接U和V-U的最小代价的边），在回路中删去(u’, v’)，从而破坏了这个回路，剩下的边构成另一棵生成树T’’，T’’包含边(u, v)，且各边的代价总和小于T’各边的代价总和。因此，T’’是一棵包含边(u, v)的最小（代价）生成树。这样，T’不是G的最小（代价）生成树，这与假设相矛盾。

  ```c++
  void Prim(int u){
    //其中lowcost[j]用于存放距离j的代价，closest[j]用于存放连接j的、最短距离的点
      int lowcost[size],closeset[size];
      int k,min;
      for(int i=0;i<size;i++){
          lowcost[i] = Cost[u][i];
          closeset[i] = u;
      }
      for(int i=0;i<size-1;i++){
          min = MAXINT;
          for(int j=0;j<size;j++){
           		//lowcost[j]!=0代表在子集V中，挑出最小的
              if(lowcost[j]!=0&&lowcost[j]<min){
                  min = lowcost[j];
                  k = j;
              }
          }
          cout << closeset[k] << k << min;
        //把最小的加入子集U
          lowcost[k]=0;
        //更新集合V中所有点距离集合U的点最短的距离，从而为下一次挑选作准备
          for(int j=0;j<size;j++){
              if(lowcost[j]!=0&&Cost[k][j]<lowcost[j]){
                  lowcost[j] = Cost[k][j];
                  closeset[j] = k;
              }
          }
      }
  }
  ```

  如果想求最后所有点在最小生成树中的最近距离，则将“lowcost[j]!=0”条件改为“closest[j]!=-1”代表在子集V中

  > 性能分析：
  >
  > - 分析以上算法，设连通网络有n个顶点，则该算法的时间复杂度为O(n2)，与e无关，它适用于边稠密的网络。
  > - 引入堆，可以优化时间复杂度。
  >   - 最外层的循环次数为O(n)
  >     - 每次循环将平均2e/n条边插入最小堆中【总共2e条边，循环n次】
  >   - 最多从堆中删除O(e)条边
  >   - 堆的一次插入或者删除时间为O(log2e)
  >   - 总时间为：Ｏ(n\* 2e/n \*log2e + elog2e)  即Ｏ(elog2e) 

##### (2) Kruskal算法

###### 并查集

- 功能：能快速判断元素所在集合、快速合并集合

- 支持操作：

  ```c++
  //Union(Root1,Root2) 如果两个集合元素i和j不在同一集合中，可用Union(i,j)将它们合并到一个集合中
  
    void UFSets :: Union ( int Root1, int Root2 ) {
  //求两个不相交集合Root1与Root2的并
       parent[Root1] += parent[Root2];
       parent[Root2] = Root1;
   		//将Root2连接到Root1下面
  }
  
    
   
  //Find(x)  寻找元素x的根
  int UFSets :: Find ( int x ) {
       while ( parent [x] >= 0 ) x = parent [x];
       return x; 
  } 
  
    
  // UFSets(s) 将并查集中s个元素初始化为s个只有1个元素的子集合
  UFSets :: UFSets ( int s ) {     //构造函数
      size = sz;	         	   //集合元素个数
      parent = new int [size];	   //双亲指针数组
      for ( int i = 0; i < size; i++ ) parent[i] = -1;
            //每一个自成一个单元素集合
  }
  
  ```

- 表示：每个集合用一棵树表示，用**树的双亲表示**作为集合存储表示，集合元素编号为从0到n-1

  - **第i个数组元素**代表包含集合元素i的树结点
  - 根结点的双亲为负数，绝对值表示**集合中的元素个数**

  <img src="/Users/apple/Library/Application Support/typora-user-images/image-20200722124620612.png" alt="image-20200722124620612" style="zoom:40%;" />

- 时间复杂度分析：

  - 执行一次Union操作所需时间O(1)，n-1次Union操作所需时间O(n)

  - Union(x,y)的前提是Find(x), Find(y)，因此完成n次搜索的总时间将达到

    <img src="/Users/apple/Library/Application Support/typora-user-images/image-20200722125142399.png" alt="image-20200722125142399" style="zoom: 25%;" />

  - 改进方法：

    - **结点个数多的树的根结点作根**

- 利用Kruskal建立最小生成树：并查集+最小堆

  - 时间复杂度分析：

    - 建立最小堆：O(n^2)
    - 建立最小生成树：O(e)次出堆、2e次Find()、n-1次Union
      - 时间复杂度为：O(elog2e)、O(elog2n)、O(n)
    - 总共时间：O(elog2e) + O(elog2n) + O(n) + O(n^2)

    - 对于连通图：O(elog2e+n2)

    > 适用于：边系稀疏的情形

#### 最短路径

##### 边上权值==非负情形==的单源最短路径问题 —  Dijkstra算法

<img src="/Users/apple/Library/Application Support/typora-user-images/image-20200422210832665.png" alt="image-20200422210832665" style="zoom:50%;" />

可以得到v0到所有节点的最短距离

```c++
template <class NameType, class DistType>
void Graph<NameType, DistType> :: ShortestPath(int v)
{
  	int S[size],Pre[size];
    int min = MAXINT;
    int k=-1;
    for(int i=0;i<size;i++){
        Dist[i] = A[v][i];
        if(Dist[i]<MAXINT) Pre[i]=v;
        else Pre[i]=-1;
    }
    S[v]=1;
    Pre[v]=-1;//别忘了初始化
    for(int i=0;i<size-1;i++){
        min = MAXINT;
        k=-1;
        for(int i=0;i<size;i++){
          //找出最小的那一个，坐标为k
            if(S[i]==0&&Dist[i]!=0&&Dist[i]<min){//别忘记了判断指标里面还有一个S[i]
                min = Dist[i];
                k = i;
            }
        }
      //如果所有都已经被挑选过
        if(k==-1) break;
      //标记已被挑选
        S[k]=1;
        for(int i=0;i<size;i++){
          //基于k更新其他点的最近距离、以及距离最近的点
            if(S[i]==0&&Dist[i]!=0&&(Dist[k]+A[k][i]<Dist[i])){//别忘记了判断指标里面还有一个S[i]
                Dist[i] = Dist[k]+A[k][i];
                Pre[i] = k;//别忘了更新pre
            }
        }
    }
    for(int i=0;i<size;i++) cout << Pre[i] << "  ";
}

```

> DJ和Prim的区别仅仅在于最后一个循环，Prim是判断只要距离k最近，而DJ判断的是距离v最近

##### 边上权值为==任意值==的单源最短路径问题 — Bellman和Ford算法

<img src="/Users/apple/Library/Application Support/typora-user-images/image-20200423190003077.png" alt="image-20200423190003077" style="zoom:50%;" />

<img src="/Users/apple/Library/Application Support/typora-user-images/image-20200423190014007.png" alt="image-20200423190014007" style="zoom:50%;" />

<img src="/Users/apple/Library/Application Support/typora-user-images/image-20200423190105661.png" alt="image-20200423190105661" style="zoom:50%;" />

```c++
void BellmanFord(Graph <float> &G, int v,float dist[ ], int path[ ]) {
    //在带权有向图中有的边具有负的权值
    //从顶点v找到所有其它顶点的最短路径
    int n=G.NumberOfVertices( );
    for (int i=0; i<n; i++) {
        dist[i]=G.GetWeight(v, i);
        if (i!=v && dist[i]<MaxValue)
            path[i]=v;
        else path[i]=-1;
    }
  //DJ是只更新最新选中的点u附近的距离，但是BF不选最小的点，而是每一轮所有的点都更新一次，总共n轮
    for (int k=2; k<n; k++)
        for (int u=0; u<n; u++)
            if (u!=v)
                for (i=0; i<n; i++) {
                    Type w=G.GetWeight(i, u);
                    if (w<MaxValue && dist[u]>dist[i]+w) {
                        dist[u]=dist[i]+w;
                        path[u]=i;
                    }
                }
}

```

> 性能分析：
>
> - 用邻接矩阵作为存储结构，为O(n3)
> - 用邻接表作为存储结构，可以通过优化，把内层的两个for循环变成一个循环，每次访问e条边，这样只要O(e)，则总时间为O(n*e)
>   - 邻接表要略做修改，把各个边链表连接成一个链表，原来内层的两个for循环变成一个遍历链表的循环。

##### ==所有顶点==之间的最短路径 — Floyd算法

本质就是把每个点应用BF+改成一个矩阵

<img src="/Users/apple/Library/Application Support/typora-user-images/image-20200423191047935.png" alt="image-20200423191047935" style="zoom:50%;" />



```c++
void Graph<NameType, DistType>::Floyd(){
    vector<float> market;
    int min=0,max=0,flag=0;
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            L[i][j] = Cost[i][j];
            if(Cost[i][j]!=0&&Cost[i][j]<MAXINT) Pre[i][j] = i;
            else Pre[i][j] = -1;
        }
    }
    
    for(int k=0;k<size;k++){
        for(int i=0;i<size;i++){
            for(int j=0;j<size;j++){
                if(L[i][k]+L[k][j]<L[i][j]){
                    L[i][j] = L[i][k]+L[k][j];
                    Pre[i][j] = Pre[k][j];
              }
            }
        }
    }
}
```

> 性能分析：
>
> - 函数的执行时间为O(n3)。
>
>   - Dijkstra外加一层循环也是O(n3)，有什么区别？
>
>     答：DJ因为贪心，因此不能有值为负数的边。但是Floyd因为使用动态规划，所以可以
>
>     **一定能用贪心算法解的问题肯定可以由动态规划解。但是可以用动态规划来解的问题，不一定能用贪心算法来解。**
>
> - Floyd算法允许图中有带负权值的边，但不许有权值总和为负的回路。
>
>   - 负值回路不存在最短路径，floyd只能判断权值总和为负的回路的存在

> 其他：
>
> - 求解最短路径的算法不仅适用于带权有向图，对带权无向图也可适用。因为带权无向图可以看作是有往返二重边的有向图。

- `闭包矩阵`

  <img src="/Users/apple/Library/Application Support/typora-user-images/image-20200423192020781.png" alt="image-20200423192020781" style="zoom:50%;" />

#### 拓扑排序

##### 背景

- `AOV网络`：用有向图表示一个工程，方向代表工作顺序

  > 因此首先判断是否存在有向环。方法：拓扑排序

##### 拓扑排序

###### 方法

- 输入AOV网络，令n为顶点个数
- 在AOV网络中选择一个**没有直接前驱**的顶点，输出、删除该顶点、删除它所有边
- 直到所有全部顶点已输出，拓扑有序序列形成

<img src="/Users/apple/Library/Application Support/typora-user-images/image-20200722130421404.png" alt="image-20200722130421404" style="zoom:30%;" />

###### 实现

<img src="/Users/apple/Library/Application Support/typora-user-images/image-20200722130520614.png" alt="image-20200722130520614" style="zoom:50%;" />

- count数组：代表前驱节点数量；

  需要根据count数组建立**入度为0的顶点栈**，设立一个**栈顶指针top**指示当前栈顶位置，即一个入度为0的顶点。栈初始化，置top=-1

  - 进栈：顶点i进栈，count[i] = top，top = i // 这是为了索引
  - 出栈：j = top; top = count[top] //相当于倒退回了一步，输出j

  <img src="/Users/apple/Library/Application Support/typora-user-images/image-20200722131634971.png" alt="image-20200722131634971" style="zoom:50%;" />

  ​                          			建栈									2、4入栈			4出栈，0入栈			0出栈，3入栈