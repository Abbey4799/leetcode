# Tree刷题记录

## 递归类题目积累

### 3.12 \24. [Swap Nodes in Pairs](https://leetcode.com/problems/swap-nodes-in-pairs/)

递推关系：swapPairs(head) = swapPairs(head.next.next)

```c++
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        if(head == NULL) return NULL;
        if(head->next == NULL) return head;
        
        ListNode* next;
        next = head->next;
        head->next = swapPairs(head->next->next);
        next->next = head;
        return next;
    }
};
```

- 通过递归可以避免找前一个节点，Magic！

#### \894. [All Possible Full Binary Trees](https://leetcode.com/problems/all-possible-full-binary-trees/)

递推关系: allPossibleFBT(N) = allPossibleFBT(i) + allPossibleFBT(N – 1 - i)，其中i为奇数，1<= i<N。

```c++
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    vector<TreeNode*> allPossibleFBT(int N) {
        vector<vector<TreeNode*>> child;
        vector<TreeNode*> ans;
        TreeNode* root;
        if(!(N%2)) return ans;
        
        if(N==1){
            root = new TreeNode(0);
            ans.push_back(root);
            return ans;
        }
        
        for(int i=1;i<N-1;i=i+2){
            child.push_back(allPossibleFBT(i));
        }
        for(int i=0,j = child.size()-1;i<child.size();i++,j--){
            for(int ii=0;ii<child[i].size();ii++){
                for(int jj=0;jj<child[j].size();jj++){
                    root = new TreeNode(0);
                    root->left = child[i][ii];
                    root->right = child[j][jj];
                    ans.push_back(root);
                }
            }
        }
        return ans;
    }
};
```

### 3.13 \416. [Partition Equal Subset Sum](https://leetcode.com/problems/partition-equal-subset-sum/)

一个系列的题：

状态转移方程：

```c++
dp[i][j] = dp[i - 1][j] || dp[i - 1][j - nums[i]]
```

如果是一个数组是否满足一个条件，思考是否能通过动态规划把它分散下去

其实题目给的有提示：数组的每个数都是整数，并且数字不会超过200，数组长度不超过100，这些说明了数字的和不会太大不会太多。

##### 动归做法一

```c++
class Solution {
public:
    bool canPartition(vector<int>& nums) {
        sort(nums.begin(),nums.end());
        int sum=0;
        for(int i=0;i<nums.size();i++){
            sum += nums[i];
        }
        if(sum%2) return false;
        int target = sum/2;
        bool dp[nums.size()+1][target+1];
        
        //初始化
        //注意：这里的dp[i][j]包括取这个数以及不取这个数，所以一定可以构成0的，只要不取这个数
        for(int i=0;i<nums.size()+1;i++)    dp[i][0] = true;
        for(int i=0;i<target+1;i++) dp[0][i] = false;
        
        dp[0][0] = true;
        for(int i=1;i<=nums.size();i++){
            for(int j=0;j<=target;j++){
                //注意这个地方，虽然dp是nums.size()+1，但是nums本身是从第0位就有效
                if(j >= nums[i-1])  dp[i][j] = dp[i-1][j] || dp[i-1][j-nums[i-1]];
                else    dp[i][j] = dp[i-1][j];
            }
        }
        return dp[nums.size()][target];
    }
```

##### 动归做法二：从空间复杂度上优化

```c++
class Solution {
public:
    bool canPartition(vector<int>& nums) {
        sort(nums.begin(),nums.end());
        int sum=0;
        //优化：每一个i都只与i-1有关，因此可以状态压缩为一维数组
        //但是注意需要从后往前更新：因为每一个dp[j]都用的是“尚未更新”的dp[j-nums[i-1]]
        sum = accumulate(nums.begin(), nums.end(), 0);
        if(sum%2) return false;
        int target = sum/2;
        bool dp[target+1];
        for(int i=1;i<target+1;i++) dp[i] = false;
        dp[0] = true;
        //时间复杂度并没有变
        for(int i=1;i<nums.size()+1;i++){
            for(int j=target;j>=0;j--){
                if(j >= nums[i-1])    dp[j] = dp[j] || dp[j-nums[i-1]];
            }
        }
        return dp[target];
    }
    
    
};
```

### 3.14 \473. [Matchsticks to Square](https://leetcode.com/problems/matchsticks-to-square/)

Yes! 仔细想一定可以想出来的，好脑瓜子！

```c++
class Solution {
public:
    bool help(vector<int>& nums,int end,vector<int>& a){
        if(end<0){
            for(int i=0;i<a.size();i++){
                if(a[i]>0) return false;
            }
            return true;
        }
        
        int i = end;
        //一轮过去end一定放入了a中的某一个
        for(int j=0;j<a.size();j++){
            //放与不妨
            //放：
            //递归的关键是，放不放nums[i]，可以进一步精简判断从而实现剪枝
            if(a[j] == nums[i] || a[j]-nums[i]>=nums[0]){
                a[j] -= nums[i];
                if(help(nums,end-1,a))  return true;
                //不放：
                a[j] += nums[i];
            }
        }
        return false;
    }
    
    bool makesquare(vector<int>& nums) {
        if(nums.size() < 4) return false;
        //考察回溯法，所以每一个nums[i]面临的选择就是放与不放。用循环+递归依次考察放与不妨
        //通过排序实现剪枝
        sort(nums.begin(),nums.end());
        int sum = accumulate(nums.begin(),nums.end(),0);
        if(sum%4)   return false;
        int target = sum/4;
        vector<int> a;
        for(int i=0;i<4;i++)    a.push_back(target);
        return help(nums,nums.size()-1,a);
    }
};
```

Btw：神奇的剪枝！

<img src="/Users/apple/Library/Application Support/typora-user-images/image-20200314121456254.png" alt="image-20200314121456254" style="zoom:50%;" />

### \698. [Partition to K Equal Sum Subsets](https://leetcode.com/problems/partition-to-k-equal-sum-subsets/)

与473题同理

## 树类题积累

### 3.16 \101. [Symmetric Tree](https://leetcode.com/problems/symmetric-tree/)

```c++
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    bool help(TreeNode* r1,TreeNode* r2){
        if(r1==NULL&&r2==NULL)  return true;
        if(r1==NULL||r2==NULL)  return false;
        if(r1->val != r2->val) return false;
        if(help(r1->left,r2->right)&&help(r1->right,r2->left))  return true;
        return false;
        
    }
    
    bool isSymmetric(TreeNode* root) {
        if(root == NULL)    return true;
        if(help(root->left,root->right)) return true;
        return false;
    }
};
```

迭代版——我应该想的出来的，有事没事多思考！

```c++
    bool isSymmetric(TreeNode* root){
        queue<TreeNode*> qq;
        if(root==NULL) return true;
        TreeNode *t1,*t2;
        qq.push(root->left);
        qq.push(root->right);
      //记住是.empty()不是.isempty()
        while(!qq.empty()){
          //记住队列是这么用的，层次遍历必用queue
            t1 = qq.front();
            qq.pop();
            t2 = qq.front();
            qq.pop();
            if(t1==NULL&&t2==NULL)  continue;
            if(t1==NULL||t2==NULL)  return false;
            if(t1->val!=t2->val)    return false;
         	//原先想的是判断每一层是否对称，但是完全可以把应该相同的点放在一起，然后每次pop出两个比较
            qq.push(t1->left);
            qq.push(t2->right);
            qq.push(t1->right);
            qq.push(t2->left);
        }
        return true;
    }
```

#### [Binary Tree Pruning](https://leetcode.com/problems/binary-tree-pruning/)

```c++
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    TreeNode* pruneTree(TreeNode* root) {
        if(root==NULL)    return  NULL;
        root->left = pruneTree(root->left);
        root->right = pruneTree(root->right);
        if(root->left==NULL&&root->right==NULL){
            if(!root->val)  root = NULL;
        }
        return root;
    }
};
```

#### [Merge Two Binary Trees](https://leetcode.com/problems/merge-two-binary-trees/)

```c++
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    TreeNode* mergeTrees(TreeNode* t1, TreeNode* t2) {
        if(t1==NULL)    return t2;
        if(t2==NULL)    return t1;
        TreeNode* root = new TreeNode(0);
        root->val = t1->val+t2->val;
        root->left = mergeTrees(t1->left,t2->left);
        root->right = mergeTrees(t1->right,t2->right);
        return root;
    }
};
```

### 3.17 \199. [Binary Tree Right Side View](https://leetcode.com/problems/binary-tree-right-side-view/)

层次遍历新视角

```c++
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    vector<int> rightSideView(TreeNode* root) {
        //tree类题：除了递归、迭代就是层次遍历
        queue<TreeNode*> qq;
        TreeNode* t;
        vector<int> ans;
        if(root == NULL)    return ans;
        
        qq.push(root);
        while(!qq.empty()){
            //不用数字来判断是哪一层，而是用for保证循环一次就是一层
            ans.push_back(qq.back()->val);
            int size = qq.size();
            for(int i=0;i<size;i++){
                //因此，每次的size都是一层的node的数量，其他的都已经被弹出去了
                t = qq.front();
                qq.pop();
                if(t->left) qq.push(t->left);
                if(t->right) qq.push(t->right);
            }
        }
        return ans;
    }
};


```

#### \111. [Minimum Depth of Binary Tree](https://leetcode.com/problems/minimum-depth-of-binary-tree/)

我最初的做法，虽然是对的但是较繁琐

```c++
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    //一定要自己想出来！
    int minDepth(TreeNode* root) {
        //确保叶子节点
        int minl = -1,minr = -1;
        if(root==NULL)  return 0;
        if(root->left==NULL&&root->right==NULL) return 1;
        if(root->left)  minl = minDepth(root->left);
        if(root->right) minr = minDepth(root->right);
        if(minl>=0&&minr>=0)  return (minl<minr?minl:minr)+1;
        if(minl>=0) return minl+1;
        return minr+1;
    }
};
```

正解简洁版

```c++
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    //一定要自己想出来！
    int minDepth(TreeNode* root) {
        //可以再简洁一点
        if(root==NULL)  return 0;
        if(root->left!=NULL&&root->right!=NULL) return (minDepth(root->left)<minDepth(root->right)?minDepth(root->left):minDepth(root->right))+1;
        else  return 1 + minDepth(root->left) + minDepth(root->right);
        //跟我最开始的思路一样，但是注意怎么才能保证终止于leaf node? 那NULL的那一边为0直接加上来就可，巧妙！
    }
};
```

### 3.18 \662. [Maximum Width of Binary Tree](https://leetcode.com/problems/maximum-width-of-binary-tree/)

关键：防止溢出 小s脸

原来的做法，思路对的，但是极端数据溢出了。

```c++
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    int widthOfBinaryTree(TreeNode* root) {
        if(root==NULL) return 0;
        TreeNode* t;
        int ans = -1;
        int first,last,flag;
        queue<TreeNode*>  qq;
        qq.push(root);
        while(!qq.empty()){
            //每一次循环一层
            int size = qq.size();
            flag = 0;
            first = -1;
            last = -1;
            //寻找左右两边最大
            for(int i=0;i<size;i++){
                t = qq.front();
                qq.pop();
                if(t){
                    if(!flag){
                        first = i;
                        last = i;
                        flag = 1;
                    }else{
                        last = i;
                    }
                }
                if(t){
                    qq.push(t->left);
                    qq.push(t->right);
                }else{
                    qq.push(NULL);
                    qq.push(NULL);
                }
            }
            if(first == -1) break;
            if((last - first + 1)>ans)  ans = last - first + 1;
            cout << first << " " << last << endl;
            //应该是溢出了，所以不用把所有的NULL都push进来
        }
    }
};
```

正解：

```c++
        TreeNode* t;
        int ans = -1;
        int left,right;
        queue<pair<TreeNode*,int>>  qq;
        qq.push({root,1});
        while(!qq.empty()){
            //额滴个神....
            if (qq.size() == 1) qq.front().second = 1;
            //防止溢出，如果这一行只有一个节点，就不断初始化这个节点为根节点
            int size = qq.size(),left = qq.front().second,right = left;
            for(int i=0;i<size;i++){
                auto t = qq.front().first;
                right = qq.front().second;
                qq.pop();
                //此处不能用i，因为push的只有非NULL的节点
                if(t->left) qq.push({t->left,right*2});
                if(t->right) qq.push({t->right,right*2+1});
            }
            if((right-left+1)>ans) ans = right-left+1;
        }
        return ans;
```

### 3.19. \889. [Construct Binary Tree from Preorder and Postorder Traversal](https://leetcode.com/problems/construct-binary-tree-from-preorder-and-postorder-traversal/)

```c++
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    TreeNode* help(vector<int>& pre,int pb,int pe,vector<int>&post,int tb,int te){
        int first = pre[pb];
        TreeNode *root = new TreeNode(first);
        
        if(pb==pe)  return root;
        
        int flag = pb+1;
        int cleft = pre[pb+1];
        int cright = post[te-1];
        
      //注意思考特殊情况
        if(cleft==cright){
            root->left = help(pre,flag,pe,post,tb,te-1);
            root->right = NULL;
            return root;
        }
        
        int i;
        for(i=flag;pre[i]!=cright;i++);
        root->left = help(pre,flag,i-1,post,tb,tb+i-flag-1);
        root->right = help(pre,i,pe,post,tb+i-flag,te-1);
        return root;
    }
    
    
    TreeNode* constructFromPrePost(vector<int>& pre, vector<int>& post) {
        if(pre.size()==0) return NULL;
        TreeNode *root;
        root = help(pre,0,pre.size()-1,post,0,post.size()-1);
        return root;
    }
};
```

### 3.21 \1028. [Recover a Tree From Preorder Traversal](https://leetcode.com/problems/recover-a-tree-from-preorder-traversal/)

##### 解法一：自己想的递归

递归的想法也不错！挺直观，赞美自己，就是有点慢。

```c++
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    //小何的递归方法
    
    string help(string S){
        int flag = 0;
        string s1;
        string temp;
        for(int i=0;i<S.size();i++){
            if(S[i]=='-'&&flag==0){
                flag = 1;
                continue;
            }else if(S[i]!='-'){
                flag = 0;
            }
            temp += S[i];
        }
        return temp;
    }
    
    
    TreeNode* recoverFromPreorder(string S) {
        if(S == "")  return NULL;
        int flag1 = 0,flag2 = 0;
        int count = 0;
        int value = 0;
        int i=0;
        string s1,s2;
        TreeNode *r;
        for(;i<S.size();i++){
            //注意字符里面的数字需要减去的48
            if(S[i]!='-')   value = value*10 + S[i] - 48;
            else break;
        }
        r = new TreeNode(value);
        for(;i<S.size();i++){
            if(S[i] == '-'){
                count++;
                continue;
            }else{
                if(count == 1){
                    if(flag1 == 0)  flag1 = i;
                    else flag2 = i;
                }
                count = 0;
            }
        }
        
        if(flag1&&flag2){
            s1 = S.substr(flag1,flag2-flag1-1);
            s2 = S.substr(flag2);
            //每遇到'-'就剪掉
            s1 = help(s1);
            s2 = help(s2);
            r->left = recoverFromPreorder(s1);
            r->right = recoverFromPreorder(s2);
        }else if(flag1){
            s1 = S.substr(flag1);
            s1 = help(s1);
            r->left = recoverFromPreorder(s1);
            r->right = NULL;
        }else{
            r->left = NULL;
            r->right = NULL;
        }
        return r;
    }
  
};
```

##### 解法二：效果比较好的递归

理论上效率跟我的递归差不多，但是无疑简洁许多。类似层次遍历，每一次遍历一层，通过全局变量cur来控制当前位子，所以是可以连着写node->left和node->right的因为curr已经改变

> 虽然博主最开始想的递归方法不太容易实现，但其实这道题也是可以用递归来做的，这里我们需要一个全局变量 cur，表示当前遍历字符串S的位置，递归函数还要传递个当前的深度 level。在递归函数中，首先还是要提取短杠的个数，但是这里有个很 tricky 的地方，==我们在统计短杠个数的时候，不能更新 cur==，因为 cur 是个全局变量，当统计出来的短杠个数跟当前的深度不相同，就不能再继续处理了，如果此时更新了 cur，而没有正确的复原的话，就会出错。博主成功入坑，检查了好久才找出原因。当短杠个数跟当前深度相同时，我们继续提取出结点值，然后新建出结点，对下一层分别调用递归函数赋给新建结点的左右子结点，最后返回该新建结点即可，参见代码如下：

```c++
class Solution {
public:
    TreeNode* recoverFromPreorder(string S) {
        int cur = 0;
        return helper(S, cur, 0);
    }
    TreeNode* helper(string& S, int& cur, int level) {
        int cnt = 0, n = S.size(), val = 0;
      //统计当前深度
      //注意此处不能更新cur，因为若不是当前层但是cur改变了就会出错
        while (cur + cnt < n && S[cur + cnt] == '-') ++cnt;
      //当前枝干不是我在寻找的那一层
        if (cnt != level) return nullptr;
        cur += cnt;
        for (; cur < n && S[cur] != '-'; ++cur) {
            val = 10 * val + (S[cur] - '0');
        }
        TreeNode *node = new TreeNode(val);
        node->left = helper(S, cur, level + 1);
        node->right = helper(S, cur, level + 1);
        return node;
    }
};
```

##### 解法三：stack

最开始的想法跟createBinTree有点像，但是不完全相似。createBinTree是遇到')'就pop，所以是实时更新的，但是本质都一样，只是这道题不是随时pop，而是发现vv的==size大于level==的时候就说明到右子树了（因为先序遍历），要把左子树全部都pop掉。这一点实在不好想。

```c++
    TreeNode* recoverFromPreorder(string S) {
        //关键在于 - 就是level
        vector<TreeNode*> vv;
        int level = 0,i = 0,val = 0,n=S.size();
        while(i<n){
            for (level = 0; i < n && S[i] == '-'; ++i) {
                ++level;
            }
            for (val = 0; i < n && S[i] != '-'; ++i) {
                val = 10 * val + (S[i] - '0');
            }
            TreeNode *t = new TreeNode(val);
            
            //注意，如果变成右孩子，左子树的结点都要pop
            while(vv.size()>level)  vv.pop_back();
            //直接根据有木有左孩子判断，而不是根据flag
            if(!vv.empty()){
                if(vv.back()->left)    vv.back()->right = t;
                else vv.back()->left = t;
            }
            
            vv.push_back(t);
        }
        return vv[0];
    }
```

## 其他

- **尾递归**：尾递归是指在返回时，==直接返回递归函数调用的值，不做额外的运算==。比如，第一节中斐波那契数列的递归是返回: return fib(N-1) + fib(N-2);。返回时，需要做加法运算，这样的递归调用就==不属于尾递归==。

  - 好处：
    - 避免==递归调用期间==栈空间开销的累积，因为系统可以为每个递归调用重用栈中的固定空间。
    - 降低算法的==空间复杂度==，由原来应用栈存储中间状态，变换为不断直接返回最终值。

- **剪枝操作**：在递归调用过程中，通过==添加相关判断条件==，==减少不必要的递归操作==，从而提高算法的运行速度。一般来说，良好的剪枝操作能够降低算法的==时间复杂度==，提高程序的健壮性。

- STL算法之accumulate函数

  accumulate(开始迭代器，结束迭代器，初始值，Binary_op)
  
  ```c++
  total = accumulate ( v1.begin ( ) , v1.end ( ) , 0 );
  ptotal = accumulate ( v3.begin ( ) , v3.end ( ) , 1 , multiplies<int>( ) );
  ```
