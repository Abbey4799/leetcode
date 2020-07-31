# 图论刷题记录

## BFS&DFS

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
            return par[x] =find(par[x]);
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

