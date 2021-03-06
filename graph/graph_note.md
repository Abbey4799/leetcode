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

<img src="readmepic/image-20200422193320218.png" alt="image-20200422193320218" style="zoom:50%;" />

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

    > - 如果用邻接表表示图，==时间复杂度为O(n+e)==
    >   - 查找==所有顶点的邻接点所需时间为O(E)==，==访问顶点的邻接点所花时间为O（V）==
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

    > - 邻接表表示，==时间复杂度为O(n+e)==
    >   - 每个结点需要进队列一次
    >   - 每个结点的每条边要检查一次，共O(e)次
    > - 邻接矩阵表示， ==时间复杂度为O($n^2$)==

### 一些经典算法

### 连通性

##### tarjan算法

[强连通分量及缩点tarjan算法解析](https://blog.csdn.net/acmmmm/article/details/16361033)

[有向图强连通分量的Tarjan算法](https://byvoid.com/zhs/blog/scc-tarjan/)

- 关键词：dfs算法、栈维护父子关系

- 两类边：

  <img src="/Users/apple/Library/Application Support/typora-user-images/image-20200905093231121.png" alt="image-20200905093231121" style="zoom:33%;" />

  - 横叉边：红边——连接的两个点没有父子关系
  - 后向边：绿边——连接的两个点存在父子关系

- 注意：实际情况中可能存在**大环套小环**的情况，因此需要优化dfs过程。

- 关键变量：

  > int Time, DFN[N], Low[N];

  - DFN[i]：遍历到i点时是第几次dfs

  - Low[u]：u或u的子树通过最多一条反向边能够追溯到的最早的栈中节点的DFN值。

    ```
    Low[u] = min(DFN[u],Low[v],DFN[v])
    ```

  > int Stack[N], top;

- 算法伪代码

  ```pseudocode
  tarjan(u){
  	DFN[u] = Low[u] = ++Index
    Stack.push(u)
    for each (u,v) in E
    	if (v is not visited)
    		tarjan(v)
    		Low[u] = min(Low[u],Low[v])  //因为Low[v]一定比DFN[v]小
    	else if (v in Stack)
    		Low[u] = min(Low[u],DFN[v])
    
    if(DFN[u] == Low[u]){ 
    
    // 按理说Low[u]一般都会比DFN[u]小，因为初始化时两个值相等，前面又取了 Low[u] = min(Low[u],Low[v])
    // 而只有后续有节点重新指向它的时候，二者才会相等
    // 子节点最后会指向它，因此栈中的元素都在环上
    
    	repeat
    		c = S.pop
    		print v
    	until(u==v)
    }
  }
  ```

- 完整代码

  ```c++
  int DFN[N], Low[N], Stack[N], field[N]; //初始化：DFN[N] = {-1};
  int top, Time, taj;
  bool Instack[N];
  vector<int> bcc[N]; //每个连通分量中的节点
  
  void tarjan(int u){
  	DFN[u] = Low[u] = ++Time;
  	Stack[top++] = u;
  	
  	for each (u,v) in E:{ //不同表示方法均可以
  		if(DFN[v] == -1){
  			tarjan(v);
  			Low[u] = min(Low[u],Low[v]);
  			if(DFN[u] < Low[v]){
  				edge(u,v)是割桥
  			}
  		}
  		else if(Instack[v]) Low[u] = min(Low[u],DFN[v]);
  	}
  	
  	if(DFN[u] == Low[u]){
  		int now;
  		taj++;
  		bcc[taj].clear();
  		do{
  			now = Stack[--top];
  			Instack[now] = 0;
  			field[now] = taj;
  			bcc[taj].push_back(now);
  		}while(now != u);
  	}
  }
  ```

  

#### 最小生成树

##### (1)Prim算法

- 主要思想：

  <img src="readmepic/image-20200422201640455.png" alt="image-20200422201640455" style="zoom:50%;" />

  可用==反证法证明。==

  > 证明：用反证法证明上面提出的结论。
  >        设G中的任何一棵包含T的最小生成树都不包含边(u, v)，且设T’就是这样的生成树。因为T’是树，所以它是连通的，从u到v必有一条路径(u, ..., v)，路径(u, ..., v)中必有边(u’, v’)，满足u’∈U, v’∈V-U。
  >        把(u, v)加入T’中，就构成一条回路，由假设，边(u, v)的代价小于边(u’, v’)的代价（因为边(u, v)是连接U和V-U的最小代价的边），在回路中删去(u’, v’)，从而破坏了这个回路，剩下的边构成另一棵生成树T’’，T’’包含边(u, v)，且各边的代价总和小于T’各边的代价总和。因此，T’’是一棵包含边(u, v)的最小（代价）生成树。这样，T’不是G的最小（代价）生成树，这与假设相矛盾。

  ```c++
  #include <iostream>
  #include <algorithm>
  #include <string>
  #include <cstring>
  #include <vector>
  #include <queue>
  
  using namespace std;
  
  int N, M;
  int k,ans;
  const int MAXN = 5e3+5;
  const int LMAXN = 1e11;
  int Dist[MAXN], Clo[MAXN], visited[MAXN];
  
  struct LUOGU {
  	int endpoint;
  	int weight;
  	LUOGU(int a, int b):endpoint(a), weight(b){};
  };
  
  vector<vector<LUOGU> > G(MAXN, vector<LUOGU>());
  
  
  int prim(int u) {
  	visited[u] = 1;
  
  	for (int i = 1; i <= N; i++) {
  		if (i != u) Dist[i] = LMAXN;
  	}

  	for (int i = 0; i < G[u].size(); i++) {
		LUOGU tmp = G[u][i];
  		int nex = tmp.endpoint;
  		int wgt = tmp.weight;
  		// Attention please!注意重边
  		Dist[nex] = min(wgt,Dist[nex]);
  		Clo[nex] = u;
  	}
  	
  	for (int i = 0; i < N - 1; i++) {
  		int min = LMAXN;
  		for (int j = 1; j <= N; j++) {
  			if (Dist[j] != 0 && Dist[j] != LMAXN && Dist[j] < min) {
  				min = Dist[j];
  				k = j;
  			}
  		}
  		if (min == LMAXN) break;
  
  		ans += min;
  		Dist[k] = 0;
  		visited[k] = 1;
  
  		for (int j = 0; j < G[k].size(); j++) {
  			LUOGU tmp = G[k][j];
  			int nex = G[k][j].endpoint;
  			int wgt = G[k][j].weight;
  			if (Dist[nex] != 0 && wgt < Dist[nex]) {
  				Dist[nex] = wgt;
  				Clo[nex] = k;
  			}
  		}
  		
  	}
  
  	return ans;
  }
  
  int main()
  {
  	cin >> N >> M;
  	if (!N) {
  		cout << "orz";
  		return 0;
  	}
  
  	for (int i = 0; i < M; i++) {
  		int x, y, z;
  		cin >> x >> y >> z;
  		G[x].push_back(LUOGU(y, z));
  		G[y].push_back(LUOGU(x, z));
  	}
  	prim(1);
  
  	int flag = 0;
  	for (int i = 1; i <= N; i++) {
  		if (!visited[i]) {
  			flag = 1;
  			break;
  		}
  	}
  
  	if (!flag) cout << ans;
  	else cout << "orz";
  	return 0;
  }
  
  
  
  
  ```
  
  如果想求最后所有点在最小生成树中的最近距离，则将“lowcost[j]!=0”条件改为“closest[j]!=-1”代表在子集V中
  
  > 性能分析：
  >
  > - 分析以上算法，设连通网络有n个顶点，则该算法的时间复杂度为O(n2)，与e无关，它适用于==边稠密==的网络。
  > - 引入堆，可以优化时间复杂度。
  >   - 最外层的循环次数为O(n)
  >     - 每次循环将平均2e/n条边==插入==最小堆中【总共2e条边，循环n次】
  >   - 最多从堆中==删除==O(e)条边
  >   - 堆的一次插入或者==删除时间为O(log2e)==
  >   - 总时间为：Ｏ(n\* 2e/n \*log2e + elog2e)  即Ｏ(elog2e) 
  
- 堆优化版本：不用初始化、也不用考虑重边了

  ```c++
  #include <iostream>
  #include <algorithm>
  #include <string>
  #include <cstring>
  #include <vector>
  #include <queue>
  
  using namespace std;
  
  int N, M;
  int k,minm,ans;
  const int MAXN = 5e3+5;
  const int LMAXN = 1e11;
  int Dist[MAXN], Clo[MAXN], visited[MAXN];
  
  struct LUOGU {
  	int endpoint;
  	int weight;
  	LUOGU(int a, int b):endpoint(a), weight(b){};
  	bool operator < (const LUOGU a) const {
  		return weight > a.weight;
  	}
  };
  
  vector<vector<LUOGU> > G(MAXN, vector<LUOGU>());
  priority_queue<LUOGU> qq;
  
  int prim(int u) {
  	visited[u] = 1;
  
  	qq.push(LUOGU(u, 0));
  	for (int i = 1; i <= N; i++) {
  		if (i != u) Dist[i] = LMAXN;
  	}
  	
  	while (!qq.empty()) {
  		LUOGU top = qq.top();
  		qq.pop();
  
  		k = top.endpoint;
  		minm = top.weight;
  		if (minm != Dist[k]) continue;
  		
  		ans += minm;
  		Dist[k] = 0;
  		visited[k] = 1;
  
  		for (int j = 0; j < G[k].size(); j++) {
  			LUOGU tmp = G[k][j];
  			int nex = G[k][j].endpoint;
  			int wgt = G[k][j].weight;
  			if (Dist[nex] != 0 && wgt < Dist[nex]) {
  				Dist[nex] = wgt;
  				Clo[nex] = k;
  				qq.push(LUOGU(nex, Dist[nex]));
  			}
  		}
  	}
  
  
  	return ans;
  }
  
  int main()
  {
  	cin >> N >> M;
  	if (!N) {
  		cout << "orz";
  		return 0;
  	}
  
  	for (int i = 0; i < M; i++) {
  		int x, y, z;
  		cin >> x >> y >> z;
  		G[x].push_back(LUOGU(y, z));
  		G[y].push_back(LUOGU(x, z));
  	}
  	prim(1);
  
  	int flag = 0;
  	for (int i = 1; i <= N; i++) {
  		if (!visited[i]) {
  			flag = 1;
  			break;
  		}
  	}
  
  	if (!flag) cout << ans;
  	else cout << "orz";
  	return 0;
  }
  
  
  
  ```

##### (2) Kruskal算法

每次取权值最小的边

###### 并查集

- 功能：能快速判断元素所在集合、快速合并集合

- 支持操作：

  ```c++
  class Solution {
  public:
      
      int par[1001];
      int rank[1001];
      
      //初始化n个元素，使n个元素的根节点是自身
      void init(int n){
          for(int i=0;i<n;i++){
              par[i] = i;
              rank[i] = 0;
          }
      }
  
      //查询第x节点所在的树根
      int find(int x){
          if(par[x] == x){
              return x;
          }else{
              //return find(par[x]);
              //这一步很巧妙：通过递归找到根的同时，可以压缩路径
           		//Attention1：注意是find(par[x])，不是find(x)
              return par[x] = find(par[x]);
          }
      }
  
      void unite(int x,int y){
          x = find(x);
          y = find(y);
  
        //Attention2：如果相同，就return
          if(x==y) return;
  
          //将大的节点作为合并后的节点
          if(rank[x] < rank[y]){
              par[x] = y;
          }else{
              if(rank[x] == rank[y]) rank[x]++;
              par[y] = x;
          }
      }
  
      //判断x和y是否是同一个i集合
      bool same(int x,int y){
          return find(x) == find(y);
      }
      
      vector<int> findRedundantConnection(vector<vector<int>>& edges) {
          
          init(edges.size());
          
          for(int i=0;i<edges.size();i++){
              if(same(edges[i][0],edges[i][1])) return edges[i];
              else{
                  unite(edges[i][0],edges[i][1]);
              }
          }
          
          vector<int> ans;
          return ans;
      }
  };
  ```

  注意几点：

  - Attention1：注意是find(par[x])，不是find(x)
  - Attention2：如果相同，就return

- 表示：每个集合用一棵树表示，用**树的双亲表示**作为集合存储表示，集合元素编号为从0到n-1

  - **第i个数组元素**代表包含集合元素i的树结点
  - 根结点的双亲为负数，绝对值表示**集合中的元素个数**

  <img src="readmepic/image-20200722124620612.png" alt="image-20200722124620612" style="zoom:40%;" />

- 时间复杂度分析：

  - 执行一次Union操作所需时间O(1)，==n-1次Union操作所需时间O(n)==

  - Union(x,y)的前提是Find(x), Find(y)，因此完成n次搜索的总时间将达到

    <img src="readmepic/image-20200722125142399.png" alt="image-20200722125142399" style="zoom: 25%;" />

  - 改进方法：

    - **结点个数多的树的根结点作根**

- 利用Kruskal建立最小生成树：并查集+最小堆

  - 时间复杂度分析：

    - 建立最小堆：O(elog2e)
    - 建立最小生成树：O(e)次出堆、2e次Find()、n-1次Union
      - 时间复杂度为：O(elog2e)、O(elog2n)、O(n)
    - 总共时间：O(elog2e) + O(elog2n) + O(n) 

    - 对于连通图：O(elog2e)

    > 适用于：边系稀疏的情形

#### 最短路径

##### 边上权值==非负情形==的单源最短路径问题 —  Dijkstra算法

注意，每次找的是全局最短，不看是从哪个点出发的

<img src="readmepic/image-20200422210832665.png" alt="image-20200422210832665" style="zoom:50%;" />

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
        for(int j=0;j<size;j++){
          //找出最小的那一个，坐标为k
            if(S[j]==0&&Dist[j]!=0&&Dist[j]<min){//别忘记了判断指标里面还有一个S[i]
                min = Dist[j];
                k = j;
            }
        }
      //如果所有都已经被挑选过
        if(k==-1) break;
      //标记已被挑选
        S[k]=1;
        for(int j=0;j<size;j++){
          //基于k更新其他点的最近距离、以及距离最近的点
            if(S[j]==0&&Dist[j]!=0&&(Dist[k]+A[k][j]<Dist[j])){//别忘记了判断指标里面还有一个S[i]
                Dist[j] = Dist[k]+A[k][j];
                Pre[j] = k;//别忘了更新pre
            }
        }
    }
    for(int i=0;i<size;i++) cout << Pre[i] << "  ";
}

```

> DJ和Prim的区别仅仅在于最后一个循环，Prim是判断的是已访问集合到未访问集合中的各点的距离，而DJ判断的是**源点**到未访问集合中各点的距离

- 注意初始化：Pre[s] = -1;

###### 邻接表版本

```c++
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <math.h>

using namespace std;

const int MAXN = 1e5 + 5;
const int w_max = pow(2, 31) - 1;
int n, m, s;
int Dist[MAXN], visited[MAXN], Pre[MAXN];

struct LUOGU {
	int endpoint;
	int weight;
	LUOGU(int a, int b) :endpoint(a), weight(b) {}
	bool operator < (const LUOGU& b) const {
		return weight > b.weight;
	}
};

vector<vector<LUOGU> > G(MAXN, vector<LUOGU>());
priority_queue<LUOGU> qq;

void DJ() {
	int min = w_max;
	int k = -1;

	Pre[s] = -1;
	qq.push(LUOGU(s,0));

	while (!qq.empty()) {
		LUOGU k = qq.top();
		qq.pop();
		int v = k.endpoint;
		int w = k.weight;
		if (Dist[v]!=w) continue;
		visited[v] = 1;

		for (int j = 0; j < G[v].size(); j++) {
			int nex = G[v][j].endpoint;
			int wt = G[v][j].weight;
			if (!visited[nex] && Dist[v] + wt < Dist[nex]) {
				Dist[nex] = Dist[v] + wt;
				Pre[nex] = v;
				qq.push(LUOGU(nex,Dist[nex]));
			}
		}
	}

}

int main()
{

	cin >> n >> m >> s;

	for (int i = 1; i <= n; i++) {
		if (i != s) Dist[i] = w_max;
		Pre[i] = -1;
	}

	for (int i = 0; i < m; i++) {
		int u, v, w;
		cin >> u >> v >> w;
		if (u == v) continue;
		LUOGU temp(v,w);
		G[u].push_back(temp);
	}
	DJ();
	for (int i = 1; i < n; i++) {
		cout << Dist[i] << " ";
	}
	cout << Dist[n];
}


```

- 时间复杂度仍旧是O(V^2)，瓶颈在于==遍历Dist数组查找下一个节点==。

###### DJ算法的堆优化

为了优化DJ，则用堆对“遍历Dist数组查找下一个节点”进行优化，把每个顶点当前的最短距离用堆维护，每次从堆中取出的最小值就是下一次要用的顶点。

> 堆中的元素共有O(V)个，更新和取出的操作有O(E)次，因此整个算法的复杂度是==O(ElogV)==

常用堆来实现优先队列，因此此处用priority_queue实现即可。

```c++
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <math.h>

using namespace std;

const int MAXN = 1e5 + 5;
const int w_max = pow(2, 31) - 1;
int n, m, s;
int Dist[MAXN], visited[MAXN], Pre[MAXN];

struct LUOGU {
	int endpoint;
	int weight;
	LUOGU(int a, int b) :endpoint(a), weight(b) {}
	bool operator < (const LUOGU& b) const {
		return weight > b.weight;
	}
};

vector<vector<LUOGU> > G(MAXN, vector<LUOGU>());
priority_queue<LUOGU> qq;

void DJ() {
	int min = w_max;
	int k = -1;

	Pre[s] = -1;
	qq.push(LUOGU(s,0));

	while (!qq.empty()) {
		LUOGU k = qq.top();
		qq.pop();
		int v = k.endpoint;
		int w = k.weight;
    
    //Attention1
		if (Dist[v]!=w) continue;
		visited[v] = 1;

		for (int j = 0; j < G[v].size(); j++) {
			int nex = G[v][j].endpoint;
			int wt = G[v][j].weight;
			if (!visited[nex] && Dist[v] + wt < Dist[nex]) {
				Dist[nex] = Dist[v] + wt;
				Pre[nex] = v;
        //Attention2
				qq.push(LUOGU(nex,Dist[nex]));
			}
		}
	}

}

int main()
{

	cin >> n >> m >> s;

	for (int i = 1; i <= n; i++) {
		if (i != s) Dist[i] = w_max;
		Pre[i] = -1;
	}

	for (int i = 0; i < m; i++) {
		int u, v, w;
		cin >> u >> v >> w;
		if (u == v) continue;
		LUOGU temp(v,w);
		G[u].push_back(temp);
	}
	DJ();
	for (int i = 1; i < n; i++) {
		cout << Dist[i] << " ";
	}
	cout << Dist[n];
}


```

值得注意的是：

- Attention1部分的优化，放入堆中的Dist可能滞后些，当它pop出来的时候，可能Dist已经改变了，那用旧的Dist是浪费时间。因此，加一个判断条件，否则会出现两个数据点TLE的情况

- 堆中的LUOGU，和最开始构建点的LUOGU的weight是不一样的。放入堆中的weight是Dist[]
- 注意初始化ss时，LUOGU的第二个值为0，代表Dist[s] = 0
- 注意先判断再改Visited

##### 边上权值为==任意值==的单源最短路径问题 — Bellman和Ford算法

<img src="readmepic/image-20200423190003077.png" alt="image-20200423190003077" style="zoom:50%;" />

<img src="readmepic/image-20200423190014007.png" alt="image-20200423190014007" style="zoom:50%;" />

<img src="readmepic/image-20200423190105661.png" alt="image-20200423190105661" style="zoom:50%;" />

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
  //DJ是只更新最新选中的点u附近的距离，但是BF是在n-1轮中，每一轮都用附近的点更新自己的距离
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

因此如果是邻接表的版本，则存储边

```c++
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include<math.h>

using namespace std;

const int MAXN = 1e4+5;
const int w_max = pow(2, 31) - 1;
int n, m, s;
int Dist[MAXN],  Pre[MAXN];

struct LUOGU {
	int beginpoint;
	int endpoint;
	int weight;
};

vector<LUOGU> E;

void BF() {
	Dist[s] = 0;
	for (int i = 1; i <= n; i++) {
		for (int j = 0; j < E.size(); j++) {
			int u = E[j].beginpoint;
			int v = E[j].endpoint;
			int w = E[j].weight;
			if (Dist[u] < w_max && Dist[v] > Dist[u] + w) {
				Dist[v] = Dist[u] + w;
				Pre[v] = u;
			}
		}
	}
}

int main()
{

	cin >> n >> m >> s;

	for (int i = 1; i <= n; i++) {
		if (i != s) Dist[i] = w_max;
		Pre[i] = -1;
	}

	for (int i = 0; i < m; i++) {
		int u, v, w;
		cin >> u >> v >> w;
		if (u == v) continue;
		LUOGU temp = {u,v,w};
		E.push_back(temp);
	}
	BF();
	for (int i = 1; i < n; i++) {
		cout << Dist[i] << " ";
	}
	cout << Dist[n];
}

```

> 性能分析：
>
> - 用==邻接矩阵==作为存储结构，为==O(n3)==
> - 用邻接表作为存储结构，可以通过优化，把内层的两个for循环变成一个循环，每次访问e条边，这样只要O(e)，则总时间为==O(n*e)==
>   - 邻接表要略做修改，把各个边链表连接成一个链表，原来内层的两个for循环变成一个遍历链表的循环。

###### 优化：SPFA算法

- 注：无法输出负圈，但是可以处理负边；BF可以处理负环，并且输出负圈

  > - SPFA处理负边：原因在于，对于DJ而言，只能得到局部最优解；而SPFA可以在多次松弛某边的时候更新该边
  > - SPFA检测负环：当存在一个点入队列大于等于V次，则有负环

- 重点：设立一个==先进先出的队列==保存待优化的节点，
  - 初始时将源加入队列

  - 每次从队列中取出一个元素，并对所有与他相邻的点进行松弛，若某个相邻的点松弛成功且不在队列中，则将其入队

    > 思想与BFS非常相似，但是BFS会用visited数组保证进入过队列的节点不能再次进入队列，而SPFA中一个点可能在出队列之后再次被放入队列

  - 直到队列为空

```c++
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <math.h>
#include <unordered_set>

using namespace std;

const int MAXN = 1e4 + 5;
const int w_max = pow(2, 31) - 1;
int n, m, s;
int Dist[MAXN], Visited[MAXN], Pre[MAXN];

struct LUOGU {
	int endpoint;
	int weight;
};

vector<vector<LUOGU> > G(MAXN, vector<LUOGU>());
queue<int> qq;

void SPFA() {

	qq.push(s);
	Visited[s] = 1;
	while (!qq.empty()) {
		int top = qq.front();
		qq.pop();
		Visited[top] = 0;

		for (int i = 0; i < G[top].size(); i++) {
			int v = G[top][i].endpoint;
			int w = G[top][i].weight;
			if (Dist[v] > Dist[top] + w) {
				Dist[v] = Dist[top] + w;
				Pre[v] = top;
				if (!Visited[v]) {
					Visited[v] = 1;
					qq.push(v);
				}
			}
		}
	}
}

int main()
{

	cin >> n >> m >> s;

	for (int i = 1; i <= n; i++) {
		if (i != s) Dist[i] = w_max;
		Pre[i] = -1;
	}

	for (int i = 0; i < m; i++) {
		int u, v, w;
		cin >> u >> v >> w;
		if (u == v) continue;
		LUOGU temp = { v,w };
		G[u].push_back(temp);
	}
	SPFA();
	for (int i = 1; i < n; i++) {
		cout << Dist[i] << " ";
	}
	cout << Dist[n];
}



```

- 时间复杂度：

  - O(KE) 其中==K为每条边平均进入队列的次数==

  - ==在最差情况下，时间复杂度会退化到O(NE)==，与BF算法相同

##### ==所有顶点==之间的最短路径 — Floyd算法

本质就是把每个点应用BF+改成一个矩阵

<img src="readmepic/image-20200423191047935.png" alt="image-20200423191047935" style="zoom:50%;" />



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
> - Floyd算法==允许图中有带负权值的边==，但==不许有权值总和为负==的回路。
>
>   - 负值回路不存在最短路径，floyd只能判断权值总和为负的回路的存在

> 其他：
>
> - 求解最短路径的算法不仅适用于带权有向图，对带权无向图也可适用。因为带权无向图可以看作是有往返二重边的有向图。

- `闭包矩阵`

  <img src="readmepic/image-20200423192020781.png" alt="image-20200423192020781" style="zoom:50%;" />

#### 拓扑排序

##### 背景

- `AOV网络`：用有向图表示一个工程，方向代表工作顺序

  > 因此首先判断是否存在有向环。方法：拓扑排序

##### 拓扑排序

###### 方法

- 输入AOV网络，令n为顶点个数
- 在AOV网络中选择一个**没有直接前驱**的顶点，输出、删除该顶点、删除它所有边
- 直到所有全部顶点已输出，拓扑有序序列形成

<img src="readmepic/image-20200722130421404.png" alt="image-20200722130421404" style="zoom:30%;" />

###### 实现

<img src="readmepic/image-20200722130520614.png" alt="image-20200722130520614" style="zoom:50%;" />

- count数组：代表前驱节点数量；

  需要根据count数组建立**入度为0的顶点栈**，设立一个**栈顶指针top**指示当前栈顶位置，即一个入度为0的顶点。栈初始化，置top=-1

  - 进栈：顶点i进栈，count[i] = top，top = i // 这是为了索引
  - 出栈：j = top; top = count[top] //相当于倒退回了一步，输出j

  <img src="readmepic/image-20200722131634971.png" alt="image-20200722131634971" style="zoom:50%;" />

  ​                          			建栈									2、4入栈			4出栈，0入栈			0出栈，3入栈



#### AOE

**关键路径是指能影响项目整体时间的活动和事件的集合，是项目中最长的路径**

- **结点最早达成时间VE(Vertex Early)**：**VE(j) = max{VE(i) + W(i,j)}**
- **结点最晚达成时间VL(Vertex Late)：**VL(i) = min{VL(j) – W(i,j)}
- **活动最早开始时间AE(Activity Early)**：**AE(当前活动) = VE(当前活动的起始结点)**
- **活动最晚开始时间AL(Activity Late)：****AL(当前活动) = VL(当前活动的结束结点) – W(当前活动)*

**步骤1：求解所有结点的VE和VL；**

**步骤2：根据VE和VL求解所有边的AE和AL；**

**步骤3：找到AE和AL相同的边，连接起来及为关键路径。**

