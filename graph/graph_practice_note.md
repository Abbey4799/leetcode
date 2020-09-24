# 图论刷题记录

> 图的题注意：可能有**自环和重边**

### 4.23

#### [\133. Clone Graph](https://leetcode.com/problems/clone-graph/)

##### DFS版本

回顾一下DFS，最主要的是：visitied数组，递归

```c++
/*
// Definition for a Node.
class Node {
public:
    int val;
    vector<Node*> neighbors;
    
    Node() {
        val = 0;
        neighbors = vector<Node*>();
    }
    
    Node(int _val) {
        val = _val;
        neighbors = vector<Node*>();
    }
    
    Node(int _val, vector<Node*> _neighbors) {
        val = _val;
        neighbors = _neighbors;
    }
};
*/

class Solution {
public:
    unordered_map<int,Node*> M;
    Node* cloneGraph(Node* node) {
        if(node==NULL) return NULL;
        M[node->val] = new Node(node->val);
        vector<Node*> vec = node->neighbors;
        if(!vec.size())   return M[node->val];
        
        int flag = 0;
        Node* p = node->neighbors[flag];
        vector<Node*> tpneighbors;
        while(1){
            if(!M.count(p->val))    p = cloneGraph(p);  
            else p = M[p->val];
            tpneighbors.push_back(p);
            if(flag+1==vec.size()) break;
            flag++;
            p = node->neighbors[flag];
        }
        M[node->val]->neighbors = tpneighbors;
        return M[node->val];
    }
};
```

第二次做：

```c++
class Solution {
public:
    unordered_map<int,Node*> M;
    
    Node* cloneGraph(Node* node) {
        if(node == NULL) return NULL;
        if(M.count(node->val)) return M[node->val];
        M[node->val] = new Node(node->val);
        
        vector<Node*> nwneighbors;
        int size = node->neighbors.size();
        M[node->val]->neighbors = nwneighbors;
        if(!size) return M[node->val];
        
        int i = 0;
        while(1){
            Node* p = node->neighbors[i];
            nwneighbors.push_back(cloneGraph(p));
            i = i+1;
            if(nwneighbors.size() == size)  break;
        }
        M[node->val]->neighbors = nwneighbors;
        return M[node->val];
    }
};
```

##### BFS版本

```c++
/*
// Definition for a Node.
class Node {
public:
    int val;
    vector<Node*> neighbors;
    
    Node() {
        val = 0;
        neighbors = vector<Node*>();
    }
    
    Node(int _val) {
        val = _val;
        neighbors = vector<Node*>();
    }
    
    Node(int _val, vector<Node*> _neighbors) {
        val = _val;
        neighbors = _neighbors;
    }
};
*/

class Solution {
public:
    unordered_map<int,Node*> M;
    Node* cloneGraph(Node* node) {
        if(node==NULL) return NULL;
        queue<Node*> qq;
        M[node->val] = new Node(node->val);
        qq.push(node);
        while(!qq.empty()){
            vector<Node*> tpneighbors;
            Node* temp = qq.front();
            qq.pop();
            if(!M.count(temp->val))  M[temp->val] = new Node(temp->val);
            vector<Node*> vec = temp->neighbors;
            
            for(int i=0;i<vec.size();i++){
                if(!M.count(vec[i]->val)){
                    qq.push(vec[i]);
                    M[vec[i]->val] = new Node(vec[i]->val);
                }
                tpneighbors.push_back(M[vec[i]->val]);
            }
            M[temp->val]->neighbors = tpneighbors;
        }
        return M[node->val];
    }
};
```

### 4.24

[\127. Word Ladder](https://leetcode.com/problems/word-ladder/)

乍一看是个字符串的题，但是可以用图论的知识点来解决：差一个字母就连一条边，最后用BFS找最短路径即可。并且你并不需要直接构建图，只是判断有边的方式是看是否相差一个字母。最开始我的做法如下，最终**超时了**：

```c++
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        //巧妙！差一个字母就连一条边；并且你并不需要直接构建图，只是判断有边的方式是看是否相差一个字母
        unordered_map<string,int>   M;
        int size = wordList.size();
        vector<int> visited(wordList.size(),0);
        queue<string>   qq;
        qq.push(beginWord);
        M[beginWord] = 1;
        while(!qq.empty()){
            string S = qq.front();
            qq.pop();
            for(int i=0;i<wordList.size();i++){
                if(help(wordList[i],S)){
                    if(!M.count(wordList[i])||M[wordList[i]] > M[S]+1)   M[wordList[i]] = M[S]+1;
                    if(!visited[i]){
                        visited[i] = 1;
                        qq.push(wordList[i]);
                    }
                }
            }
        }
        if(!M.count(endWord))   return 0;
        else return M[endWord];
    }
    
    bool help(string& A,string& B){
        int i=0;
        int flag=0;
        for(;i<A.size()&&i<B.size();i++){
            if(A[i]!=B[i])  flag++;
        }
        if(flag>1) return false;
        return true;
    }
};
```

分析时间复杂度为O(n\*n\*L) 其中L为字符串的长度，这个代价太大了。一个优化的方法就是，每次不是去遍历wordList里面的所有词进行对比，而是对S每一位进行篡改（26种），再去wordList中寻找，并且用wordSet对时间复杂度进行优化。因此最终的时间复杂度可以降低为O(n\*L\*26) 

```c++
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        //巧妙！差一个字母就连一条边；并且你并不需要直接构建图，只是判断有边的方式是看是否相差一个字母
        unordered_set<string> wordSet(wordList.begin(), wordList.end());
        if(!wordSet.count(endWord)) return 0;
        queue<string>   qq;
        qq.push(beginWord);
        
        //层数
        int flag = 0;
        while(!qq.empty()){
            int size = qq.size();
            for(int i=0;i<size;i++){
                string S = qq.front();
                qq.pop();
                if(S == endWord)    return flag+1;
                for(int j=0;j<S.size();j++){
                    string newS = S;
                    for(char c='a';c<='z';c++){
                        newS[j] = c;
                        if(wordSet.count(newS)&&newS!=S){
                            wordSet.erase(newS);
                            qq.push(newS);
                        }
                    }
                }
            }
            flag++;
        }
        return 0;
    }
    
};
```

第二次做：用了unordered_map来当visited数组，但是其实把wordset中抹掉即可，见上面的block，但是BFS的方式需要改动，每一次需要遍历qq.size()的节点，否则flag不对

```c++
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> wordSet(wordList.begin(), wordList.end());
        unordered_map<string,int> M;
        
        queue<string> Q;
        Q.push(beginWord);
        M[beginWord] = 1;
        
        while(!Q.empty()){
            string temp = Q.front();
            Q.pop();
            
            for(int i=0;i<temp.size();i++){
                for(char j = 'a';j<='z';j++){
                    string temp1 = temp;
                    temp1[i] = j;
                    if(wordSet.count(temp1) && !M.count(temp1)){
                        Q.push(temp1);
                        M[temp1] = M[temp] + 1;
                        if(temp1 == endWord)    return M[temp1];
                    }
                }
            }
        }
        
        return 0;
    }
    
};
```

### 4.25

##### [\126. Word Ladder II](https://leetcode.com/problems/word-ladder-ii/)

基本思路和I很像，但是实现非常繁琐而且容易超时，据说是leetcode AC率最低的题，因此在此放弃:)

##### [\128. Longest Consecutive Sequence](https://leetcode.com/problems/longest-consecutive-sequence/)

最长连续串

梦回google面试，这样的复杂度是O(nlogn)，结果也不错，但是题目要求时间复杂度为O(n)

==注意：连续相同也算连续== 如0，1，1，2，但是只算3

```c++
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        if(nums.size()==0) return 0;
        if(nums.size()==1) return 1;
        sort(nums.begin(),nums.end());
        for(int i=1;i<nums.size();i++)  nums[i-1] = nums[i]-nums[i-1];
        
        int max = 1;
        int temp = 1;
        for(int i=0;i<nums.size()-1;i++){
          //注意这里可能有连续相同的情况
            if(nums[i]==1||nums[i]==0){
                temp += nums[i];
                if(temp>max)    max = temp;
            }
            else temp = 1;
        }
        return max;
    }
};
```

所以需要用到图的思想。跟word ladder相似，借助BFS的思想。学到就是赚到鸭。

```c++
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        if(!nums.size()) return 0;
        unordered_set<int> numSet(nums.begin(), nums.end());
        int max = 1;
        while(numSet.size()){
            queue<int>  qq;
            unordered_set<int>::iterator i = numSet.begin();
            qq.push(*i);
            int flag = 0;
            while(!qq.empty()){
                int temp = qq.front();
                qq.pop();
                flag++;
                numSet.erase(temp);
                if(numSet.count(temp-1))    qq.push(temp-1);
                if(numSet.count(temp+1))    qq.push(temp+1);
            }
            if(flag>max) max = flag; 
        }
        return max;
    }
};
```

### 7.22 

##### [\207. Course Schedule](https://leetcode.com/problems/course-schedule/)

常规拓扑排序，可以用二维vector快速建图。

注意：先用一个循环构建入度为0的顶点栈

- 进栈：count[i] = top; top = i;
- 出栈：j = top; top = count[top];

```c++
class Solution {
public:
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        vector<int> count(numCourses,0);
        vector<vector<int>> G(numCourses,vector<int>(0,0));
        
        for(int i=0;i<prerequisites.size();i++){
            count[prerequisites[i][1]]++;
            G[prerequisites[i][0]].push_back(prerequisites[i][1]);
        }
        
        int top = -1;
        for(int i=0;i<numCourses;i++){
            if(count[i]==0){
                count[i] = top;
                top = i;
            }
        }
        if(top==-1) return false;
        
        int output = 0;
        for(int i=0;i<numCourses;i++){
            int temp = top;
            top = count[top];
            for(int j=0;j<G[temp].size();j++){
                if((--count[G[temp][j]])==0){
                    count[G[temp][j]] = top;
                    top = G[temp][j];
                }
            }
            output++;
            if(top == -1)   break;
        }
        if(output<numCourses)   return false;
        return true;
    }
};
```

##### [\210. Course Schedule II](https://leetcode.com/problems/course-schedule-II/)

只用把上一题的temp存入vector即可

### 7.28

##### [\684. Redundant Connection](https://leetcode.com/problems/redundant-connection/)

这题考点，并查集。

> 但凡遇到**构建树+不想产生环**的题，并查集都是最好的选择
>
> [一文搞定并查集](https://blog.csdn.net/dingdingdodo/article/details/104290972)

对于无向图：

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
            return par[x] = find(par[x]);
        }
    }

    void unite(int x,int y){
        x = find(x);
        y = find(y);

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

##### [\785. Is Graph Bipartite?](https://leetcode.com/problems/is-graph-bipartite/)

判断图是否是二部图（2着色）：

- 如果无向图没有回路，一定可二分，利用BFS着色

- 如果无向图有回路，图是二部图当且仅当无奇圈（P287）：利用DFS着色

  [交叉染色](https://www.cnblogs.com/wenruo/p/5243034.html)

c为一种颜色，-c为另一种颜色

```c++
class Solution {
public:
    vector<vector<int>> G;
    int Cl[101];
    
    bool color(int v,int c){
        Cl[v] = c;
        for(int i=0;i<G[v].size();i++){
            if(Cl[G[v][i]]==c) return false;
            if(Cl[G[v][i]]==0 && !color(G[v][i],-c)) return false;
        }
        return true;
    }
    
    bool isBipartite(vector<vector<int>>& graph) {
        for(int i=0;i<101;i++){
            G.push_back(vector<int>(0,0));
        }
        
        for(int i=0;i<graph.size();i++){
            for(int j=0;j<graph[i].size();j++){
                G[i].push_back(graph[i][j]);
            }
        }
        
        for(int i=0;i<G.size();i++){
            if(Cl[i]==0 && !color(i,1)) return false;
        }
        
        return true;
    }
};
```

##### [\997. Find the Town Judge](https://leetcode.com/problems/find-the-town-judge/)

练手，只用记录出度和入度即可

```c++
class Solution {
public:
    int findJudge(int N, vector<vector<int>>& trust) {
        vector<int> degreein(N+1,0);
        vector<int> degreeout(N+1,0);

        for(int i=0;i<trust.size();i++){
            degreeout[trust[i][0]]++;
            degreein[trust[i][1]]++;
        }
        for(int i=1;i<N+1;i++){
            if(degreein[i]==N-1&&degreeout[i]==0) return i;
        }
        return -1;
    }
};
```

##### [\841. Keys and Rooms](https://leetcode.com/problems/keys-and-rooms/)

利用dfs或者bfs都可以

```c++
class Solution {
public:
    unordered_set<int> vertex;
    int MAXN = 0;
    
    bool dfs(vector<vector<int>>& rooms,int v){
        if(!vertex.count(v)) vertex.insert(v);
        if(vertex.size()==MAXN) return true;
        for(int i=0;i<rooms[v].size();i++){
            if(vertex.count(rooms[v][i])) continue;
            if(dfs(rooms,rooms[v][i])) return true;
        }
        return false;
    }
    
    bool canVisitAllRooms(vector<vector<int>>& rooms) {
        if(rooms.size()==0) return true;
        MAXN = rooms.size();
        return dfs(rooms,0);
    }
};
```

### 7.30

##### [\55. Jump Game](https://leetcode.com/problems/jump-game/?utm_source=LCUS&utm_medium=ip_redirect_q_uns&utm_campaign=transfer2china)

最开始的做法，想用bfs来做，最终超时了。

```c++
class Solution {
public:
    bool canJump(vector<int>& nums) {
        if(nums.size()==0) return false;
        if(nums.size()==1) return true;
        queue<int> Q;
        Q.push(0);
        while(!Q.empty()){
            int tmp = Q.front();
            Q.pop();
            for(int i=1;i<=nums[tmp];i++){
                if(tmp+i>=nums.size()-1)    return true;
                if(tmp+i<nums.size()-1)     Q.push(tmp+i);
            }
        }
        return false;
    }
};
```

因为不用考虑恰好到，而是超过就行，因此每次求**最大能跳**的地方即可。

###### 贪婪算法

既然每次都求最大能跳的范围，则维护一个reach变量，代表能跳的最远位置。

```c++
class Solution {
public:
    bool canJump(vector<int>& nums) {
        if(nums.size()==0) return false;
        if(nums.size()==1) return true;
        int reach = 0;
        for(int i=0;i<nums.size();i++){
            if(i > reach) return false;
            reach = max(i+nums[i],reach);
            if(reach > nums.size()-1) return true;
        }
        return true;
    }
};
```

###### 动态规划

考虑用**动态规划**来做，每次考虑是继续用前面的没用完的跳力，还是用这个位置带来的新跳力。维护一个一维数组dp，其中dp[i]代表第i位置的剩余跳力，若跳力为负，则无法到达。

**状态转移方程**：当前位置的剩余跳力与前一个位置的剩余跳力（dp值【继承了之前一个较大的跳力】）和前一个位置的新跳力（nums数组中的值）有关。dp[i] = max(dp[i-1],nums[i-1])-1

```c++
class Solution {
public:
    bool canJump(vector<int>& nums) {
        if(nums.size()==0) return false;
        if(nums.size()==1) return true;
        vector<int> dp(nums.size(),0);
        dp[0] = nums[0];
        for(int i=1;i<nums.size();i++){
            dp[i] = max(dp[i-1],nums[i-1])-1;
            if(dp[i]<0) return false;
        }
        return true;
    }
};
```

> 小感悟：
>
> - 遇到求最大就可以的题，一般都用贪婪算法，需要不断更新一个值
> - 贪婪算法与动态规划一般一起出现，动态规划需要找到状态转移方程

##### [\45. Jump Game II](https://leetcode.com/problems/jump-game-ii/)

与第一题相比，多了一个要求跳的次数最少。

利用bfs的思想，每一层更新一次最远的地方。bfs不一定需要queue，只要保存每一层的状态。

无论是否是先跳1个比较短、再跳1个比较长，还是直接跳到这个范围的最远距离，再接着跳，跳的次数是一样的。

```c++
class Solution {
public:
    int jump(vector<int>& nums) {
        if(nums.size()==0 || nums.size()==1) return 0;
        int curMax = 0;//这一层最远的地方
        int reach = 0;//下一层最远的地方
        int i=0;
        int layer = 0;
        while(i <= curMax){
            for(;i<=curMax;i++){
                reach = max(reach,i+nums[i]);
                if(reach >= nums.size()-1)   return layer+1;
            }
            curMax = reach;
            layer++;
        }
        return -1;
    }
};
```

##### [\45. Jump Game III](https://leetcode.com/problems/jump-game-iii/)

该题指定了最开始跳的位置start，想到达值为0的地方，且规定了每个位置可以跳的长度。

利用bfs即可。

```c++
class Solution {
public:
    bool canReach(vector<int>& arr, int start) {
        if(arr.size()==0||arr.size()==1) return true;
        
        vector<int> M(arr.size(),0);
        queue<int> Q;
        Q.push(start);
        M[start] = 1;
        while(!Q.empty()){
            int tmp = Q.front();
            if(arr[tmp]==0) return true;
            Q.pop();
            if(tmp+arr[tmp]<arr.size() && M[tmp+arr[tmp]]==0) {
                Q.push(tmp+arr[tmp]);
                M[tmp+arr[tmp]] = 1;
            }
            if(tmp-arr[tmp]>=0 && M[tmp-arr[tmp]]==0){
                Q.push(tmp-arr[tmp]);
                M[tmp-arr[tmp]] = 1;
            }
        }
        return false;
    }
};
```

## 图的存储与遍历

### 8.10 

##### ==[P2661 信息传递](https://www.luogu.com.cn/problem/P2661)==

本质是求有向图里面的最短回路。

血泪...我的想法是先用拓扑排序的方法删除入度为0的节点，然后剩下的环一定是独立的，就用dfs删除。值得注意的是，拓扑排序的出栈：

```c++
j = top; top = count[top] //相当于倒退回了一步，输出j
```

出栈后，==count[top]还是保留的是对下一个top的索引==。为了后续的判断，需要把count[top]置为0。最终成功AC。

```c++
#include <iostream>
#include <string>
#include <vector>
#include <queue>


using namespace std;


int num;
int backup;
const int MAXN = 2e5 + 5;
int ans = MAXN;
int G[MAXN];
int cnt[MAXN];


void dfs(int start, int& num) {
	//cout << "start:" << start << " " << num << endl;
	if (cnt[start] == 0) return;

	int next = G[start];
	cnt[start] = 0;
	num++;
	dfs(next, num);
}


int main()
{
	cin >> num;


	for (int i = 1; i <= num; i++) {
		int tmp = 0;
		cin >> tmp;

		G[i] = tmp;
		cnt[tmp]++;
	}

	int top = -1;
	for (int i = 1; i <= num; i++) {
		if (!cnt[i]) {
			cnt[i] = top;
			top = i;
		}
	}


	if (top != -1) {
		for (int i = 1; i <= num; i++) {
			int temp = top;
			int newtop = cnt[top];
			cnt[top] = 0;
			top = newtop;
			
			if ((--cnt[G[temp]]) == 0) {
				cnt[G[temp]] = top;
				top = G[temp];
			}
			if (top == -1) break;
		}
	}

	// 注意，因为赋值top，因此入度为0的点的值为-1
	for (int i = 1; i <= num; i++) {
		if (cnt[i] == -1) cnt[i] = 0;
	}


	for (int i = 1; i <= num; i++) {
		if (cnt[i]) {
			int tmp = 0;
			dfs(i, tmp);
			if (tmp < ans)	ans = tmp;
		}
	}

	cout << ans;


}
```

往简单的想，【走路法，类似用快慢指针判断单链表中有没有环】，由于出度为1，因此每个子图只有两种结果：单纯一个环链，一条单链+一个环链。因此，用cnt数组记录下每一次走的步数，并用visited_one数组记录下（局部，也即找寻这一个环的过程中）是否走过该节点，如果第二次碰到该节点，则用当前步数减去cnt数组记录下的上一次步数，则为环的长度。遍历所有的节点（visited_all数组记录是否全局被访问过，如果再次遇到被访问过的节点，直接跳过。因为每个点只能在一个环上）。

最终80分，TLE了两个点。

```c++
#include <iostream>
#include <string>
#include <vector>
#include <queue>


using namespace std;


int num;
const int MAXN = 2e5 + 1;
int G[MAXN];
int visited_all[MAXN];
int visited_one[MAXN];
int cnt[MAXN];
int ans = MAXN;

int main()
{	
	cin >> num;


	for (int i = 1; i <= num; i++) {
		int tmp = 0;
		cin >> tmp;
		G[i] = tmp;
	}
	int cnttmp = 0;

	int i = 1;
	int now = 1;
	for (; i <= num; i++) {
		if (!visited_all[i]) {
			now = i;
			cnttmp = 0;
			
			while (!visited_one[now]) {
				visited_one[now] = 1;
				visited_all[now] = 1;
				cnt[now] = ++cnttmp;
				now = G[now];
			}
			if ((cnttmp - cnt[now] + 1) < ans)	ans = cnttmp - cnt[now] + 1;
			for (int j = 1; j <= num; j++) visited_one[j] = 0;
		}
		else continue;
	}

	cout << ans;
	
	
}
```

## 最短路专题

### 8.17

##### [P3371 【模板】单源最短路径（弱化版）](https://www.luogu.com.cn/problem/P3371)

- 被初始化坑了...注意不能直接用{}初始化数组。

- 要考虑自己到自己的边...

用[邻接表版本的BF算法](graph_note)，70分，TLE了三个数据点。

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

优化：SPFA算法

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

### 8.18

##### [P4779 【模板】单源最短路径（标准版）](https://www.luogu.com.cn/problem/P4779)

因为SPFA算法实际上不够稳定，如果卡SPFA算法的话，16分，大部分都会TLE。

可以看到本道题明显点出是非负权图，所以很明显一定要用DJ算法。要学会堆优化版本：

为了优化DJ，则用堆对“遍历Dist数组查找下一个节点”进行优化，把每个顶点当前的最短距离用堆维护，每次从堆中取出的最小值就是下一次要用的顶点。

> 堆中的元素共有O(V)个，更新和取出的操作有O(E)次，因此整个算法的复杂度是O(ElogV)

常用堆来实现优先队列，因此此处用priority_queue实现即可。

<img src="/Users/apple/Library/Application Support/typora-user-images/image-20200819130726057.png" alt="image-20200819130726057" style="zoom:50%;" />

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

如果不带Visited：

<img src="/Users/apple/Library/Application Support/typora-user-images/image-20200819130828557.png" alt="image-20200819130828557" style="zoom:50%;" />

大部分测试点变慢了，但还是100分

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

		for (int j = 0; j < G[v].size(); j++) {
			int nex = G[v][j].endpoint;
			int wt = G[v][j].weight;
			if (Dist[v] + wt < Dist[nex]) {
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

### 8.19

##### [P1144 最短路计数](https://www.luogu.com.cn/problem/P1144)

用堆优化的DJ，60分，超时了两个点。因为总是把nex压进队列，则出现了成倍增加的计算量（多次压入同一个子节点，子节点的子节点的计算量则翻倍）。

```c++
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <math.h>

using namespace std;

int N, M;
const int MAXN = 1e6+5;
const int w_max = pow(2, 31) - 1;
int Dist[MAXN], Visited[MAXN], Cnt[MAXN];

struct LUOGU {
	int endpoint;
	int weight;
	LUOGU(int a, int b): endpoint(a), weight(b){}
	bool operator < (const LUOGU& a) const{
		return weight > a.weight;
	}
};

vector<vector<LUOGU> > G(MAXN, vector<LUOGU>());
priority_queue<LUOGU> qq;

void DJ() {
	qq.push(LUOGU(1,0));
	while (!qq.empty()) {
		LUOGU k = qq.top();
		qq.pop();

		int v = k.endpoint;
		int w = k.weight;
		if (w != Dist[v]) continue;
		Visited[v] = 1;

		for (int i = 0; i < G[v].size(); i++) {
			int nex = G[v][i].endpoint;
			int wgt = G[v][i].weight;
			
			if (!Visited[nex] && Dist[nex] >= Dist[v] + wgt) {

				if (Dist[nex] > Dist[v] + wgt)	Cnt[nex] = 1;
				else if (Dist[nex] == Dist[v] + wgt) {
					Cnt[nex]++;
					if (Cnt[nex] == 100003)	Cnt[nex] = 0;
				}
				Dist[nex] = Dist[v] + wgt;

				qq.push(LUOGU(nex, Dist[nex]));
			}
		}
	}
}

int main()
{
	cin >> N >> M;

	if (N == 1) {
		cout << 1;
		return 0;
	}

	for (int i = 0; i < M; i++) {
		int u, v;
		cin >> u >> v;
		if (u != 1 && u == v) continue;

		G[u].push_back(LUOGU(v, 1));
		G[v].push_back(LUOGU(u, 1));

	}

	for (int i = 2; i <= N; i++) {
		Dist[i] = w_max;
	}

	DJ();
	
	cout << 1 << endl;
	for (int i = 2; i < N; i++) {
		cout << Cnt[i] << endl;
	}
	cout << Cnt[N];
	
	return 0;
}


```

经过改进，AC代码如下：

```c++
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <math.h>

using namespace std;

int N, M;
const int MAXN = 1e6+5;
const int w_max = pow(2, 31) - 1;
int Dist[MAXN], Visited[MAXN], Cnt[MAXN];

struct LUOGU {
	int endpoint;
	int weight;
	LUOGU(int a, int b): endpoint(a), weight(b){}
	bool operator < (const LUOGU& a) const{
		return weight > a.weight;
	}
};

vector<vector<LUOGU> > G(MAXN, vector<LUOGU>());
priority_queue<LUOGU> qq;

void DJ() {
	qq.push(LUOGU(1,0));
	while (!qq.empty()) {
		LUOGU k = qq.top();
		qq.pop();

		int v = k.endpoint;
		int w = k.weight;
		if (w != Dist[v]) continue;
		Visited[v] = 1;

		for (int i = 0; i < G[v].size(); i++) {
			int nex = G[v][i].endpoint;
			int wgt = G[v][i].weight;
			
      //Attention1
			if (Dist[nex] > Dist[v] + wgt) {
				Dist[nex] = Dist[v] + wgt;
        //Attention2
				Cnt[nex] = Cnt[v];
				qq.push(LUOGU(nex, Dist[nex]));
			}
			else if (Dist[nex] == Dist[v] + wgt) {
        //Attention3
				Cnt[nex] += Cnt[v];
				Cnt[nex] %= 100003;
			}
		}
	}
}

int main()
{
	cin >> N >> M;

	if (N == 1) {
		cout << 1;
		return 0;
	}

	for (int i = 0; i < M; i++) {
		int u, v;
		cin >> u >> v;
		if (u != 1 && u == v) continue;

		G[u].push_back(LUOGU(v, 1));
		G[v].push_back(LUOGU(u, 1));

	}

	Cnt[1] = 1;
	for (int i = 2; i <= N; i++) {
		Dist[i] = w_max;
	}

	DJ();
	
	
	for (int i = 1; i < N; i++) {
		cout << Cnt[i] << endl;
	}
	cout << Cnt[N];
	
	return 0;
}


```

- 注意Attention1处：不需要判断!Visited[i]，因为虽然不可能变短，但是有可能相同；
- 注意Attention2处：如果探索到更好的道路，不是将Cnt置为1，而是将Cnt置为跟新的父节点的Cnt一样；
- 注意Attention3处：如果得到同样长度的路径，不是将Cnt++，而是将新节点Cnt加上该父节点的Cnt，并且不再将新节点压入队列；

##### [P1522 [USACO2.4]牛的旅行 Cow Tours](https://www.luogu.com.cn/problem/P1522)

输入节点数、节点的坐标、邻接矩阵代表连通的情况。现有多个连通图，从其中两个图中各选一个节点，使得最后的唯一连通图直径最小（相距最远的两个节点的距离）。

这道题有点绕，但是理解下来用暴力解法就能做。主要思路是：

- 用floyd求出每个牧场中每个点距离其他点的最大距离
- 仍取其中两个牧场的任意2个点计算直线距离，与这两个点在自己的牧场中的最远距离相加，得到距离A
- 但是**距离A还不是最后的结果**，将距离A与所有牧场原来的直径对比，取其中最大的作为新牧场的直径

```c++
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <math.h>
#include <cmath>
#include <iomanip> 

using namespace std;

const int MAXN = 150;
const double LMAXN = 1e20;
int N;
int Field[MAXN];
double Lmax[MAXN];
double L[MAXN][MAXN];

struct LUOGU {
	int endpoint;
	double weight;
	LUOGU(int a, double b) :endpoint(a), weight(b) {}
};

vector<vector<LUOGU> > G(MAXN, vector<LUOGU>());



void DFS(int s,int num) {
	Field[s] = num;
	for (int i = 0; i < G[s].size(); i++) {
		int v = G[s][i].endpoint;
		if (!Field[v]) {
			DFS(v,num);
		}
	}
}

void floyd() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i != j) {
				L[i][j] = LMAXN;
			}
			// 注意: double的初始化不会自动置为0，需要自己手写
			else L[i][j] = 0;
		}

		for (int j = 0; j < G[i].size(); j++) {
			int v = G[i][j].endpoint;
			double w = G[i][j].weight;
			L[i][v] = w;
		}
	}


	for (int k = 0; k < N; k++) {
		for (int i = 0; i < N; i++) {
			double max = 0;
			for (int j = 0; j < N; j++) {
				if (L[i][k] + L[k][j] < L[i][j]) {
					L[i][j] = L[i][k] + L[k][j];
				}
				if (max < L[i][j] && L[i][j] != LMAXN) {
					max = L[i][j];
				}
			}
			Lmax[i] = max;
		}
	}


}


double cal(int x1, int y1, int x2, int y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

int main()
{
	cin >> N;

	vector<vector<int> > GPU(N, vector<int>(2, 0));

	for (int i = 0; i < N; i++) {
		int num1, num2;
		cin >> num1 >> num2;
		GPU[i][0] = num1;
		GPU[i][1] = num2;
	}

	for (int i = 0; i < N; i++) {
		string a;
		cin >> a;
		for (int j = i+1; j < a.size(); j++) {
			if (a[j] == '1') {
				double wgt = cal(GPU[i][0], GPU[i][1], GPU[j][0], GPU[j][1]);
				G[i].push_back(LUOGU(j, wgt));
				G[j].push_back(LUOGU(i, wgt));
			}
		}
	}
	
	int num = 1;
	for (int i = 0; i < N; i++) {
		if (!Field[i]) {
			DFS(i, num);
			num++;
		}
	}
	

	floyd();
	double mx = 0;
  // 注意：找出所有牧场的最大直径
	for (int i = 0; i < N; i++) {
		if (Lmax[i] > mx)	mx = Lmax[i];
	}


	double min = LMAXN;
	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			if (Field[i] == Field[j])	continue;
			double wgt = cal(GPU[i][0], GPU[i][1], GPU[j][0], GPU[j][1]);
			double tmp = Lmax[i] + Lmax[j] + wgt;
			if (tmp < mx)	tmp = mx;
			if (tmp < min)	min = tmp;
		}
	}

	cout << setiosflags(ios::fixed) << setprecision(6);
	cout << min;

	return 0;
}



```

其中需要注意的小点：

- 最开始的结果，80分，WA了两个数据点。要认真看题的..虽然前面的背景是正好有两个牧区，但是后来的的又说**至少**包括两个不连通的牧区，所以可能150个牧区全都不连通。用Field数组存储所有牧点的牧区号，DFS的时候顺便标号。
- double的初始化不会自动置为0，需要自己手写，如果涉及double一定要小心，防止冷不丁就顺手转成int了
- 对于double的比较最好不要用<，而是!=，否则好像报了奇奇怪怪的错误
- 对于用邻接表表示的图，一定要注意是i和j还是G[i].endpoint，G[j].endpoint

## 最小生成树

> 常用kruskal算法，总是选第几大的边

##### [P3366 【模板】最小生成树](https://www.luogu.com.cn/problem/P3366)

###### prim算法版本

**==注意重边！！！！！！！！！！！！！！！！！！！！也即attention please的地方==**

未加入堆优化：

<img src="/Users/apple/Library/Application Support/typora-user-images/image-20200827150400698.png" alt="image-20200827150400698" style="zoom:50%;" />

代码如下：

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
		// Attention please!
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

加入堆优化：

<img src="/Users/apple/Library/Application Support/typora-user-images/image-20200827150749419.png" alt="image-20200827150749419" style="zoom:50%;" />

代码：

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

###### Kruskal算法版本

并查集，老朋友。

```c++
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <vector>
#include <queue>

using namespace std;

const int MAXN = 5e3 + 5;
int N, M;
int par[MAXN], rk[MAXN];

struct LUOGU {
	int u, v, w;
	LUOGU(int a, int b, int c) :u(a), v(b), w(c) {}
	bool operator < (const LUOGU& a) const {
		return w > a.w;
	}
};

priority_queue<LUOGU> qq;

void init(int n) {
	for (int i = 0; i < n; i++) {
		par[i] = i;
		rk[i] = 0;
	}
}

int find(int x) {
	if (par[x] == x)	return x;
	else return par[x] = find(par[x]);
}


void unite(int x, int y) {
	x = find(x);
	y = find(y);

	if (x == y) return;

	if (rk[x] < rk[y]) {
		par[x] = y;
	}
	else {
		if (rk[x] == rk[y]) rk[x]++;
		par[y] = x;
	}
}

bool same(int x, int y) {
	return find(x) == find(y);
}


int main()
{
	cin >> N >> M;
	
	init(N);

	for (int i = 0; i < M; i++) {
		int u, v, w;
		cin >> u >> v >> w;
		qq.push(LUOGU(u, v, w));
	}

	int cnt = 0, ans = 0;
	while (!qq.empty()) {
		LUOGU tmp = qq.top();
		qq.pop();
		int u = tmp.u;
		int v = tmp.v;
		int w = tmp.w;
		
		if (same(u, v)) continue;
		unite(u, v);
		ans += w;
		cnt++;
	}

	if (cnt < N - 1)	cout << "orz";
	else cout << ans;

	return 0;
}


```

##### [P2872 [USACO07DEC]Building Roads S](https://www.luogu.com.cn/training/209#problems)

现有几个连通图，求能把它们连起来的边的和的最小值。

一定一定一定注意double

```c++
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <cmath>
#include <math.h>
#include <vector>
#include <queue>
#include <iomanip>

using namespace std;

const int MAXN = 1e3 + 5;
int N, M;
int par[MAXN], rk[MAXN];

int dict[MAXN][2];

struct LUOGU {
	int u, v;
	double w;
	LUOGU(int a, int b, double c) :u(a), v(b), w(c) {}
	bool operator < (const LUOGU& a) const {
		return w > a.w;
	}
};

priority_queue<LUOGU> qq;

void init(int n) {
	for (int i = 0; i < n; i++) {
		par[i] = i;
		rk[i] = 0;
	}
}

int find(int x) {
	if (par[x] == x)	return x;
	else return par[x] = find(par[x]);
}


void unite(int x, int y) {
	x = find(x);
	y = find(y);

	if (x == y) return;

	if (rk[x] < rk[y]) {
		par[x] = y;
	}
	else {
		if (rk[x] == rk[y]) rk[x]++;
		par[y] = x;
	}
}

bool same(int x, int y) {
	return find(x) == find(y);
}

double cal(int ax, int ay, int bx, int by) {
	return sqrt(pow(bx - ax, 2) + pow(by - ay, 2));
}


int main()
{
	cin >> N >> M;
	
	init(N);

	for (int i = 1; i <= N; i++) {
		int x, y;
		cin >> x >> y;
		dict[i][0] = x;
		dict[i][1] = y;
	}


	for (int i = 0; i < M; i++) {
		int u, v;
		cin >> u >> v;
		unite(u, v);
	}


	for (int i = 1; i <= N; i++) {
		for (int j = i + 1; j <= N; j++) {
			if (same(i, j)) continue;
			double w = cal(dict[i][0], dict[i][1], dict[j][0], dict[j][1]);
			qq.push(LUOGU(i, j, w));
		}
	}


	double ans = 0;
	while (!qq.empty()) {
		LUOGU tmp = qq.top();
		qq.pop();

		int u = tmp.u;
		int v = tmp.v;
		double w = tmp.w;
		if (same(u, v)) continue;
		unite(u, v);
		ans += w;
	}

	cout << setiosflags(ios::fixed) << setprecision(2);
	cout << ans;

	return 0;
}



```

##### [P1991 无线通讯网](https://www.luogu.com.cn/problem/P1991)

有P个点，S个卫星电话；装有卫星电话的点任意距离都可以通话，没有卫星电话的点通信距离不能超过D。求最小的通话距离D。

思路是：先找到最小生成树，再将其中最长的边安上电话，求剩下节点之间的最远距离。

###### 最开始版本：60分

```c++
#include <iostream>
#include <cmath>
#include <math.h>
#include <iomanip>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;

int S, P;
const int MAXN = 505;
int point[MAXN][2];
int pre[MAXN], rk[MAXN];


struct LUOGU {
	int u, v;
	double w;
	LUOGU(int a, int b, double c) :u(a), v(b), w(c) {};
	bool operator < (const LUOGU& a) const {
		return w > a.w;
	}
};


struct LUOGU_rev {
	int u, v;
	double w;
	LUOGU_rev(int a, int b, double c) :u(a), v(b), w(c) {};
	bool operator < (const LUOGU_rev& a) const {
		return w < a.w;
	}
};

priority_queue<LUOGU> qq;
priority_queue<LUOGU_rev> QQ;
unordered_set<int> dic;

double cal(int ax, int ay, int bx, int by) {
	return sqrt(pow((ax - bx), 2) + pow((ay - by), 2));
}


void init() {
	for (int i = 0; i < P; i++) {
		pre[i] = i;
		rk[i] = 0;
	}
}

int find(int u) {
	if (pre[u] == u) return u;

	return pre[u] = find(pre[u]);
}

void unite(int u, int v) {
	int x = find(u);
	int y = find(v);

	if (x == y) return;

	if (rk[x] < rk[y]) swap(x, y);

	pre[y] = x;
	if (rk[x] == rk[y]) rk[x]++;
}

bool same(int u, int v) {
	return find(u) == find(v);
}


int main()
{
	cin >> S >> P;

	init();

	for (int i = 0; i < P; i++) {
		int x, y;
		cin >> x >> y;
		point[i][0] = x;
		point[i][1] = y;
	}

	for (int i = 0; i < P; i++) {
		for (int j = i + 1; j < P; j++) {
			double weight = cal(point[i][0], point[i][1], point[j][0], point[j][1]);
			qq.push(LUOGU(i, j, weight));
		}
	}

	while (!qq.empty()) {

		LUOGU tmp = qq.top();
		qq.pop();
		int u = tmp.u;
		int v = tmp.v;
		double w = tmp.w;

		if (same(u, v)) continue;

		unite(u, v);

		QQ.push(LUOGU_rev(u, v, w));
	}

	
	while (!QQ.empty()) {
		LUOGU_rev tmp = QQ.top();
		QQ.pop();
		
		int u = tmp.u;
		int v = tmp.v;
		double w = tmp.w;

		dic.insert(u);
		dic.insert(v);
		if (dic.size() > S) {
			cout << setiosflags(ios::fixed) << setprecision(2);
			cout << w;
			return 0;
		}
	}
	
	cout << 0.00;

	return 0;
}

```

这种的思想是：先用的kruskal算法构成最小生成树，再按边由长至短依次安装卫星电话，但安装的节点数>卫星电话的数量，则break。

但是**值得注意**的是：

- 最小生成树并不一定唯一
- n个点一定可以用n-1条边形成生成树

因此，上述方法存在的问题是，可能最后n个点可以覆盖的边小于n-1条边。因此，无需先生成最小生成树，而是将边由小到大的取，直到最后n-1条边直接交给卫星电话处理。因此，实际上取了(P-1)-(S-1) = P-S条边，也即下述方法，最终成功AC。

```c++
#include <iostream>
#include <cmath>
#include <math.h>
#include <iomanip>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;

int S, P;
const int MAXN = 505;
int point[MAXN][2];
int pre[MAXN], rk[MAXN];


struct LUOGU {
	int u, v;
	double w;
	LUOGU(int a, int b, double c) :u(a), v(b), w(c) {};
	bool operator < (const LUOGU& a) const {
		return w > a.w;
	}
};



priority_queue<LUOGU> qq;
unordered_set<int> dic;

double cal(int ax, int ay, int bx, int by) {
	return sqrt(pow((ax - bx), 2) + pow((ay - by), 2));
}


void init() {
	for (int i = 0; i < P; i++) {
		pre[i] = i;
		rk[i] = 0;
	}
}

int find(int u) {
	if (pre[u] == u) return u;

	return pre[u] = find(pre[u]);
}

void unite(int u, int v) {
	int x = find(u);
	int y = find(v);

	if (x == y) return;

	if (rk[x] < rk[y]) swap(x, y);

	pre[y] = x;
	if (rk[x] == rk[y]) rk[x]++;
}

bool same(int u, int v) {
	return find(u) == find(v);
}


int main()
{
	cin >> S >> P;

	init();

	for (int i = 0; i < P; i++) {
		int x, y;
		cin >> x >> y;
		point[i][0] = x;
		point[i][1] = y;
	}

	for (int i = 0; i < P; i++) {
		for (int j = i + 1; j < P; j++) {
			double weight = cal(point[i][0], point[i][1], point[j][0], point[j][1]);
			qq.push(LUOGU(i, j, weight));
		}
	}

	int cnt = 0;
	while (!qq.empty()) {

		LUOGU tmp = qq.top();
		qq.pop();
		int u = tmp.u;
		int v = tmp.v;
		double w = tmp.w;

		if (same(u, v)) continue;

		unite(u, v);
		cnt++;
		if (cnt == P - S) {
			cout << setiosflags(ios::fixed) << setprecision(2);
			cout << w;
			return 0;
		}

	}


	cout << setiosflags(ios::fixed) << setprecision(2);
	cout << 0.00;

	return 0;
}

```

##### [P4047 [JSOI2010]部落划分](https://www.luogu.com.cn/problem/P4047)

总共有n个居住点，将n个居住点划分为k个部落，使得部落间的最短距离（定义为部落间居住点最近的两个距离）尽可能地大

最开始想的是的选倒数第k-1大的边，但是这当然不对，因为这条边并不是部落之间的最短的边，因此应该选第n-k+1小的边。总共要连n-1-(k-1) = n-k条边，待求的距离则为第n-k+1小的边（这个边一定是部落之间的最短距离）。

```c++
#include <iostream>
#include <cmath>
#include <math.h>
#include <iomanip>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;

int n, k;
const int MAXN = 1e3 + 5;

int point[MAXN][2];
int pre[MAXN], rk[MAXN];

struct LUOGU {
	int u, v;
	double w;
	LUOGU(int a, int b, double c) :u(a), v(b), w(c) {};
	bool operator < (const LUOGU& a) const {
		return w > a.w;
	};
};

priority_queue<LUOGU> qq;

double cal(int ax, int ay, int bx, int by) {
	return sqrt(pow((ax - bx), 2) + pow((ay - by), 2));
}

void init() {
	for (int i = 0; i < n; i++) {
		pre[i] = i;
		rk[i] = 0;
	}
}

int find(int u) {
	if (pre[u] == u) return u;
	return pre[u] = find(pre[u]);
}

int unite(int u, int v) {
	int x = find(u);
	int y = find(v);
	
	if (x == y) return x;
	
	if (rk[x] < rk[y])	swap(x, y);
	
	pre[y] = x;
	if (rk[x] == rk[y]) rk[x]++;
	return x;
}

bool same(int u, int v) {
	return find(u) == find(v);
}

int main()
{
	cin >> n >> k;

	init();

	for (int i = 0; i < n; i++) {
		int x, y;
		cin >> x >> y;
		point[i][0] = x;
		point[i][1] = y;
	}

	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			double weight = cal(point[i][0], point[i][1], point[j][0], point[j][1]);
			qq.push(LUOGU(i, j, weight));
		}
	}
	
	int cnt = 0;
	double ans = 0;
	while (!qq.empty()) {
		LUOGU tmp = qq.top();
		qq.pop();

		int u = tmp.u;
		int v = tmp.v;
		double w = tmp.w;

		if (same(u, v))	 continue;

		unite(u, v);

		ans = w;
		cnt++;
		if (cnt == n - k + 1) break;
	}

	cout << setiosflags(ios::fixed) << setprecision(2);
	cout << ans;

	
	return 0;
}

```

##### [P1396 营救](https://www.luogu.com.cn/problem/P1396)

复习了一下prim算法，注意要构建无向图！

```c++
#include <iostream>
#include <cmath>
#include <math.h>
#include <iomanip>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;


int n, m, s, t;

const int MAXN = 1e4 + 5;
const int LMAXN = 1e5;
int Dist[MAXN], Clo[MAXN], visited[MAXN];

struct LUOGU {
	int v, w;
	LUOGU(int b, int c) :v(b), w(c) {};
	bool operator < (const LUOGU& a) const{
		return w > a.w;
	}
};

vector<vector<LUOGU>> vv(MAXN, vector<LUOGU>());
priority_queue<LUOGU> qq;

void Prim(int u) {
	qq.push(LUOGU(u, 0));
	for (int i = 1; i <= n; i++) {
		if (i != u) Dist[i] = LMAXN;
	}

	while (!qq.empty()) {
		LUOGU tmp = qq.top();
		qq.pop();

		int k = tmp.v;
		int minm = tmp.w;
		if (minm != Dist[k]) continue;
		if (k == t) break;

		visited[k] = 1;

		for (int i = 0; i < vv[k].size(); i++) {
			int nex = vv[k][i].v;
			int w = vv[k][i].w;
			if (!visited[nex] && w < Dist[nex]) {
				Dist[nex] = w;
				Clo[nex] = k;
				qq.push(LUOGU(nex, Dist[nex]));
			}
		}

	}
}


int main()
{
	cin >> n >> m >> s >> t;
	for (int i = 0; i < m; i++) {
		int u, v, w;
		cin >> u >> v >> w;
		vv[u].push_back(LUOGU(v, w));
		vv[v].push_back(LUOGU(u, w));
	}
	Prim(s);

	int bg = t;
	int mx = 0;
	while (bg != s) {
		if (Dist[bg] > mx) mx = Dist[bg];
		bg = Clo[bg];
	}

	cout << mx;
	return 0;
}

```

## 连通性问题

##### [P3387 【模板】缩点](https://www.luogu.com.cn/problem/P3387)

给定一个 n*n* 个点 m*m* 条边有向图，每个点有一个权值，**求一条路径，使路径经过的点权值之和最大**。你只需要求出这个权值和。

允许多次经过一条边或者一个点，但是，重复经过的点，权值只计算一次。



该题涉及的知识点较多——缩点+拓扑+DP；

**缩点**：将有向有环图中的环缩成一一个个点，形成一个有向无环图。

给定边和点，有向图，每个点都有权值，求一条路径使经过的点的权值最大。求出权值和。重复经过的点只计算一次权值

> 为求点权最大，**每个环都需要取**。因此每个环可以看作一个点 ——> 缩点

[tarjan算法](graph_note)缩点后，整张图变为无环图，在拓扑排序的代码中使用DP从而得到最大值点权。状态转移方程为：

```c++
dp[v] = max(dp[v],dp[u] + p[v])
```

代码：

```c++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;



int n, m;
const int MAXN = 1e4 + 5;

int p[MAXN];
int DFN[MAXN], Low[MAXN], field[MAXN], Stack[MAXN];
bool InStack[MAXN];
int top, tm;

int in[MAXN], dp[MAXN];

vector<vector<int> > G(MAXN, vector<int>());
vector<vector<int> > nwG(MAXN, vector<int>());

void tarjan(int u) {
	DFN[u] = Low[u] = ++tm;
	Stack[++top] = u;
	InStack[u] = 1;

	for (int i = 0; i < G[u].size(); i++) {
		int v = G[u][i];

		if (DFN[v] == -1) {
			tarjan(v);
			Low[u] = min(Low[u], Low[v]);
		}
		else if (InStack[v]) {
			Low[u] = min(Low[u], DFN[v]);
		}
	}

	if (Low[u] == DFN[u]) {
		int now;
		do {
			now = Stack[top--];
			field[now] = u;
			InStack[now] = 0;
			if(u != now)	p[u] += p[now];
		} while (now != u);
	}

}


void tuopu() {
	top = -1;

	for (int i = 1; i <= n; i++) {
		if (!in[i]) {
			in[i] = top;
			top = i;
		}
		dp[i] = p[i];
	}

	while (top != -1) {
		int tmp = top;
		top = in[top];
		in[tmp] = 0;

		for (int i = 0; i < nwG[tmp].size(); i++) {
			int v = nwG[tmp][i];
			dp[v] = max(dp[v], dp[tmp] + p[v]);
			if ((--in[v]) == 0) {
				in[v] = top;
				top = v;
			}
		}

	}
}

int main()
{
	cin >> n >> m;
	for (int i = 1; i < n + 1; i++) {
		cin >> p[i];
		field[i] = i;
	}

	for (int i = 0; i < m; i++) {
		int u, v;
		cin >> u >> v;
		G[u].push_back(v);
	}

	memset(DFN, -1, sizeof(DFN));

	for (int i = 1; i <= n; i++) {
		if (DFN[i] == -1) tarjan(i);
	}

	// 重新构建图
	for (int u = 1; u <= n; u++) {
		for (int j = 0; j < G[u].size(); j++) {
			int v = G[u][j];
      //Attention please!
			if (field[u] == field[v]) continue;
			nwG[field[u]].push_back(field[v]);
			in[field[v]]++;
		}
	}

	tuopu();


	int mx = 0;
	for (int i = 1; i <= n; i++) {
		if (dp[i] > mx)	mx = dp[i];
	}

	cout << mx;
	return 0;
}
```

注意：重新构建图的时候需要判断点是否在同一个field中

##### [P3388 【模板】割点（割顶）](https://www.luogu.com.cn/problem/P3388)

n个点，m条边的无向图，求图的割点

连通性问题都是用trajan算法解决，但是无向图稍有不同：

- 不需要再判断是否在栈中了；对于有向图而言，因为要判断强连通，所以得确定连通分量开始的点。
- 根节点和非根节点需要分开讨论：
  - 根节点：只要子树超过1
  - 非根节点：只要之后遍历的节点的Low值无法超过DFN[u]，就说明u点是割点

```c++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;


const int MAXN = 2e4 + 5;
int n, m;

int DFN[MAXN], Low[MAXN];
int cnt;

vector<vector<int> > G(MAXN, vector<int>());
unordered_set<int> ans;

bool mrk[MAXN];


void tarjan(int u,int fa) {
	DFN[u] = Low[u] = ++cnt;
	
	int child = 0;
	for (int i = 0; i < G[u].size(); i++) {
		int v = G[u][i];
		if (DFN[v] == -1) {
			tarjan(v,fa);
			Low[u] = min(Low[u], Low[v]);
			// 但是u可能是根节点
			if (DFN[u] <= Low[v] && u != fa)	mrk[u] = 1;
			if (u == fa) child++;
		}
		// 不需要再判断是否在栈中了；对于有向图而言，因为要判断强连通，所以得确定连通分量开始的点
		// 而对于无向图而言，只要之后遍历的节点的Low值无法超过DFN[u]，就说明u点是割点
		Low[u] = min(Low[u], DFN[v]);
	}

	if (child >= 2) mrk[u] = 1;
}


int main()
{
	cin >> n >> m;

	for (int i = 0; i < m; i++) {
		int u, v;
		cin >> u >> v;
		G[u].push_back(v);
		G[v].push_back(u);
	}
	
	memset(DFN, -1, sizeof(DFN));

	for (int i = 1; i <= n; i++) {
		if (DFN[i] == -1) tarjan(i,i);
	}
	
	//cout << ans.size() << endl;
	/*for (unordered_set<int>::iterator i = ans.begin(); i != ans.end();) {
		cout << *i;
		if ((++i) != ans.end()) cout << " ";
	}*/

	int times = 0;
	for (int i = 1; i <= n; i++) {
		if (mrk[i]) times++;
	}
	cout << times << endl;
	for (int i = 1; i <= n; i++) {
		if (mrk[i]) {
			cout << i << " ";
		}
	}

	return 0;
}
```

注意：题目要求从小到大输出

##### [P2863 [USACO06JAN]The Cow Prom S](https://www.luogu.com.cn/problem/P2863)

近乎模版题，tarjan算法。

```c++
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;


const int MAXN = 2e4 + 5;
int n, m;

int DFN[MAXN], Low[MAXN], Stack[MAXN], field[MAXN];
bool InStack[MAXN];
int cnt,top,taj;

vector<vector<int> > G(MAXN, vector<int>());
unordered_set<int> ans;

bool mrk[MAXN];


void tarjan(int u) {
	DFN[u] = Low[u] = ++cnt;
	Stack[++top] = u;
	InStack[u] = 1;
	
	int child = 0;
	for (int i = 0; i < G[u].size(); i++) {
		int v = G[u][i];
		if (DFN[v] == -1) {
			tarjan(v);
			Low[u] = min(Low[u], Low[v]);
		}
		else if (InStack[v]) {
			Low[u] = min(Low[u], DFN[v]);
		}
	}

	if (Low[u] == DFN[u]) {

		int now;
		int cnt = 0;
		do {
			now = Stack[top--];

			InStack[now] = 0;
			cnt++;
		} while (now != u);

		if (cnt > 1)	taj++;
	}

}


int main()
{
	cin >> n >> m;

	for (int i = 0; i < m; i++) {
		int u, v;
		cin >> u >> v;
		G[u].push_back(v);
	}
	
	memset(DFN, -1, sizeof(DFN));

	for (int i = 1; i <= n; i++) {
		if (DFN[i] == -1) tarjan(i);
	}
	
	cout << taj;

	return 0;
}
```

