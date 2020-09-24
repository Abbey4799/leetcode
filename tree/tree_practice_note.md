# Tree刷题记录

递归（包括前序后序中序遍历的应用，往往是后序思想）、迭代、层次遍历

## 递归类题目积累

### 3.12 \24. [Swap Nodes in Pairs](https://leetcode.com/problems/swap-nodes-in-pairs/)

给定一个链表，两两交换其中相邻的节点，并返回交换后的链表。

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

返回包含 `N` 个结点的所有可能满二叉树的列表。 答案的每个元素都是一个可能树的根结点。

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

分割等和子集（给定一个**只包含正整数**的**非空**数组。是否可以将这个数组分割成两个子集，使得两个子集的元素和相等）

一个系列的题：

状态转移方程：不取这个数 || 取这个数

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
      
        //查看数组是否可以组成sum/2
        
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

是否能将火柴拼成正方形

递归 + 回溯

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

检查树是否是对称的

递归的思想，直接用递归公式做判断的决定

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

二叉树剪枝，移除所有不包含1的子树

递归的思想，先把子树都剪枝 + 再判断这是不是值为0的叶子结点

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

合并两个二叉树，合并的规则是如果两个节点重叠，那么将他们的值相加作为节点合并后的新值，否则**不为** NULL 的节点将直接作为新二叉树的节点。

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

用前序和后序恢复二叉树的

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

### ==3.21 \1028. [Recover a Tree From Preorder Traversal](https://leetcode.com/problems/recover-a-tree-from-preorder-traversal/)==

用先序遍历还原二叉树

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

### 3.30 

#### 236. [Lowest Common Ancestor of a Binary Tree](https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-tree/)

很经典的一道题，热热身

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
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if(root==NULL)  return NULL;
        if(root->val == p->val||root->val == q->val) return root;
        TreeNode *left,*right;
        left = lowestCommonAncestor(root->left,p,q);
        right = lowestCommonAncestor(root->right,p,q);
        if(left&&right) return root;
        if(left)  return left;
        return right;
    
    }
};
```

#### House Robber 系列

##### [House Robber 1](https://leetcode.com/problems/house-robber/)

基础动归

不能抢相邻两间屋子的钱

```c++
class Solution {
public:
    int rob(vector<int>& nums) {
        if(nums.size()==0) return 0;
        if(nums.size()==1) return nums[0];
        int a[nums.size()];
        a[0] = nums[0];
        a[1] = max(nums[0],nums[1]);
        for(int i=2;i<nums.size();i++){
            a[i] = max(nums[i]+a[i-2],a[i-1]);
        }
        return a[nums.size()-1];
    }
};
```

##### [House Robber 2](https://leetcode.com/problems/house-robber-ii/)

在1的基础上多了一个条件，即房子排列成一个环。所以对于取第一个和不取第一个分别讨论即可。

```c++
class Solution {
public:
    int rob(vector<int>& nums) {
        if(nums.size()==0) return 0;
        if(nums.size()==1) return nums[0];
        int a[nums.size()],b[nums.size()];
        
        a[0] = 0;
        a[1] = nums[1];
        for(int i=2;i<nums.size();i++){
            a[i] = max(nums[i]+a[i-2],a[i-1]);
        }
        
        b[0] = nums[0];
        b[1] = max(nums[0],nums[1]);
        for(int i=2;i<nums.size()-1;i++){
            b[i] = max(nums[i]+b[i-2],b[i-1]);
        }
        return max(a[nums.size()-1],b[nums.size()-2]);
        
    }
};
```

### 3.31

#### ==[House Robber 3](https://leetcode.com/problems/house-robber-iiI/)==

光看题容易误解把每一层加在一起，再使用House Robber II的方法即可。但是相邻两层其实可以同时取的，只要不在一棵子树上即可。例如[2,1,3,null,4]的结果是7 。

##### 解法一：递归

与House Robber I、II想法相似，取这个节点与不取这个节点。注意这种动归思想是两个方向，I是过去的一点一点累积上去，用一个一维数组记住。而III是用递归去搜索未来。

最开始的做法：超时了；因为每一次递归都递归了很多次子节点，重复的工作。可以把它们都记录下来。可以用hashmap记住每个根节点的最大rob。

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
    int rob(TreeNode* root) {
        if(root==NULL) return 0;
        int val = 0;
        //之前否定递归是因为觉得需要考虑节点的相邻状况
        //但这种相邻状况其实是可以通过手动表示出来的。
        if(root->left){
            val += rob(root->left->left) + rob(root->left->right);
        }
        if(root->right){
            val += rob(root->right->left) + rob(root->right->right);
        }
        return max(val + root->val,rob(root->left)+rob(root->right));
    }
    
};
```

改进版：

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
    int help(TreeNode* root,unordered_map<TreeNode*,int>& mm){
        if(root==NULL) return 0;
        if(mm.count(root))  return mm[root];
        int val = 0;
        //之前否定递归是因为觉得需要考虑节点的相邻状况
        //但这种相邻状况其实是可以通过手动表示出来的。
        if(root->left){
            val += help(root->left->left,mm) + help(root->left->right,mm);
        }
        if(root->right){
            val += help(root->right->left,mm) + help(root->right->right,mm);
        }
        mm[root] = max(val + root->val,help(root->left,mm)+help(root->right,mm));
        return mm[root];
    }
    
    
    int rob(TreeNode* root) {
        unordered_map<TreeNode*,int> mm;
        return help(root,mm);
    }
    
};
```

##### 解法二：递归+数组保存状态

用二维数组表示取该值和不取该值的两种情况。一开始写错了：

虽然乍一看跟解法一相似，其实不然。解法一中自动选取了取与不取中的最大值。

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
  //错误做法
    vector<int> help(TreeNode* root){
        if(root==NULL) return vector<int>(2,0);
        vector<int> left = help(root->left);
        vector<int> right = help(root->right);
        vector<int> ans;
      //错误，有时候当然不取比取更大
        ans.push_back(left[1]+right[1]);
        ans.push_back(left[0]+right[0]+root->val);
        return ans;
        
    }

    int rob(TreeNode* root) {
        if(root == NULL) return 0;
        vector<int> ans = help(root);
        return max(ans[0],ans[1]);
    }
    
};
```

更改：

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
    vector<int> help(TreeNode* root){
        if(root==NULL) return vector<int>(2,0);
        vector<int> left = help(root->left);
        vector<int> right = help(root->right);
        vector<int> ans(2,0);
        ans[0] = max(left[0],left[1])+max(right[0],right[1]);
        ans[1] = left[0]+right[0]+root->val;
        return ans;
        
    }

    int rob(TreeNode* root) {
        if(root == NULL) return 0;
        vector<int> ans = help(root);
        return max(ans[0],ans[1]);
    }
    
};
```

### 4.2. 

#### \863. [All Nodes Distance K in Binary Tree](https://leetcode.com/problems/all-nodes-distance-k-in-binary-tree/)

巧用霍夫曼tree，可以表示每个节点的信息

学到了！对于树中间的一个点相关判断，可以用==霍夫曼tree==的方法给每个点赋予一个string，这个string里面隐含了子树情况。最后判断string之间的差别即可。

同时注意，可以用map记录，比一开始想的vector好用很多。

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
    unordered_map<TreeNode*, string> map;
    string path;
    
    vector<int> distanceK(TreeNode* root, TreeNode* target, int K) {
        vector<int> ans;
        if(root==NULL)  return ans;
        string p;
        
        help(root,target,p);
      //学会遍历map key：v.first value：v.second
        for(auto& v : map){
            TreeNode* key = v.first;
            
            int i;
            for(i=0;i<map[key].size()&&i<path.size()&&map[key][i]==path[i];i++);
            if((path.size()-i)+(map[key].size()-i)==K) ans.push_back(key->val);
        }
        
        return ans;
        
    }
    
  //霍夫曼标注
    void help(TreeNode* root,TreeNode* target,string p){
        if(root==NULL) return;
        if(root == target) path = p;
        map[root] = p;
        
        help(root->left,target,p+"0");
        help(root->right,target,p+"1");
    }
    
};
```

### 4.3

####==\968. [Binary Tree Cameras](https://leetcode.com/problems/binary-tree-cameras/)==

最开始我想着用迭代来做，从上到下，每一个节点都放或不放，需要不断地回退，最终还是有例子无法通过。迭代这块需要加强。

> 小何思考：其实回过头来发现，很多题都是用递归版的后序遍历的思想（House robber III, binary tree cameras,Distribute Coins in Binary Tree），因为父亲节点总是子节点的状态集合。以后一道题没思路，不妨想想后序遍历，每一个父亲节点的状态被子节点影响的状况有哪些？
>
> - House robber III：取父亲节点+孩子的孩子节点、不取父亲节点+孩子节点
> - binary tree cameras：如下所述
> - Distribute Coins in Binary Tree：总是要接收孩子节点的出入

- 如果子节点有一个不可观，该节点的状态一定要安装摄像头。

- 如果子节点有一个安装摄像头了，该节点都可以不安装摄像头。
- 如果子节点已经保住自身，也没安装摄像头，则该节点的状态可以由父亲节点来拯救。

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
    int ans=0;
    
    int minCameraCover(TreeNode* root) {
        if(root==NULL) return 0;
        if(dfs(root)==2) ans++;
        return ans;
    }
    
    // 0:该节点安装了摄像头 1:该节点未安装摄像头但是可观 2:该节点不可观
    int dfs(TreeNode* root){
        if(root==NULL) return 1;
        int left = dfs(root->left);
        int right = dfs(root->right);
        if(left==2 || right==2){
            ans++;
            return 0;
        }else if(left==0 || right==0){
            return 1;
        }else{
            return 2;
        }
    }
};
```

#### \1145. [Binary Tree Coloring Game](https://leetcode.com/problems/binary-tree-coloring-game/)

注意切分为了三个部分以后就很好想了。值得注意的就是涉及到**数树的节点数**的题一律用==递归==。

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
    int left = 0;
    int right = 0;
    
    bool btreeGameWinningMove(TreeNode* root, int n, int x) {
        count(root,x);
        int remain = n - 1 - left - right;
        int max1 = max(max(left,right),remain);
        int max2 = left + right + remain - max1;
        if(max1 > max2) return true;
        return false;
    }
    
    //记住数数用递归
    int count(TreeNode* root,int x){
        if(root==NULL) return 0;
        int l = count(root->left,x);
        int r = count(root->right,x);
        if(root->val == x){
            left = l;
            right = r;
        }
        return 1 + l + r;
    }
};
```

#### \257.[Binary Tree Paths](https://leetcode.com/problems/binary-tree-paths/)

解决路径相关问题的必须掌握的一道题。

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
    vector<string> ans;
    
    vector<string> binaryTreePaths(TreeNode* root) {
        if(root==NULL) return ans;
        dfs(root,to_string(root->val));
        return ans;
    }
    
    void dfs(TreeNode* root,string x){
        string ss;
        if(root->left == NULL&&root->right == NULL){
            ans.push_back(x);
            return;
        }
        if(root->left)  dfs(root->left,x + "->" + to_string(root->left->val));
        if(root->right) dfs(root->right,x + "->" + to_string(root->right->val));
    }
};
```

### 4.4. 

#### \979. [Distribute Coins in Binary Tree](https://leetcode.com/problems/distribute-coins-in-binary-tree/)

感觉跟\968. [Binary Tree Cameras](https://leetcode.com/problems/binary-tree-cameras/)的思路有点相似，不妨总是从==后序遍历+判断父亲节点与孩子节点的关系==入手。

几个状态：1 0 -1

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
    int ans = 0;
    
    int distributeCoins(TreeNode* root) {
        if(root==NULL) return 0;
        help(root);
        return ans;
    }
    
    int help(TreeNode* root){
        if(root==NULL) return 0;
        //承接孩子的变化，孩子的变化可能有正有负
        root->val += help(root->left);
        root->val += help(root->right);
        //只留1个。注意有可能正有可能负，但是移动的次数是绝对值
        ans += abs(root->val-1);
        return root->val-1;
    }
};
```

#### \987. [Vertical Order Traversal of a Binary Tree](https://leetcode.com/problems/vertical-order-traversal-of-a-binary-tree/)

并不典型的一道题，纯练练手。关键技术点为给每个点分配横纵坐标。

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
    unordered_map<int,pair<int,int>> M;
    int min = 0;
    int max = 0;
    
    vector<vector<int>> verticalTraversal(TreeNode* root) {
        help(root,make_pair(0,0));
        //先对x进行分类
        vector<vector<pair<int,int>>> temp(max-min+1,vector<pair<int,int>>(0,make_pair(0,0)));
        //再根据y进行分类
        vector<vector<int>> ans(max-min+1,vector<int>(0,0));
        for(auto& v : M){
            int val = v.first;
            pair<int,int> p = v.second;
            //pair: first为val，second为y值
            temp[p.first-min].push_back(make_pair(val,p.second));
        }
        //排序
        for(int i=0;i<temp.size();i++){
            sort(temp[i].begin(),temp[i].end(),[](pair<int,int> a,pair<int,int> b){
                //如果y值相同返回val小的那个
                if(a.second==b.second)  return a.first < b.first;
                //返回y值大的
                return a.second > b.second;
            });
            for(int j=0;j<temp[i].size();j++){
                ans[i].push_back(temp[i][j].first);
            }
        }
        return ans;
    }
    
    //给每个点编横纵坐标
    void help(TreeNode* root,pair<int,int> num){
        if(root==NULL) return;
        M[root->val] = num;
        //记录最终x的范围，方便构建矩阵
        if(num.first-1 < min) min = num.first;
        if(num.first+1 > max) max = num.first;
        help(root->left,make_pair(num.first-1,num.second-1));
        help(root->right,make_pair(num.first+1,num.second-1));
    }
};
```

#### \124. [Binary Tree Maximum Path Sum](https://leetcode.com/problems/binary-tree-maximum-path-sum/)

这道题并不像hard难度的题。值得注意的是如果用递归做“取与不取根节点”的题，要么像==[House Robber 3](https://leetcode.com/problems/house-robber-iiI/)==、==\968. [Binary Tree Cameras](https://leetcode.com/problems/binary-tree-cameras/)==一样显式地表现出两种情况，要么像这道题一样只定义“取”，然后用全局变量记录最大值。

> 小何思考：最大值相关的题都比较好想，但最优情况类的题不是那么好想。

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
    int ans = -100;

    int maxPathSum(TreeNode* root) {
        help(root);
        return ans;
    }
    
    int help(TreeNode* root) {
        if(root==NULL) return 0;
        int left = help(root->left);
        int right = help(root->right);
        int temp = max(max(max(right+root->val,left+root->val),left+right+root->val),root->val);
        if(temp > ans) ans = temp;
        return max(max(right+root->val,left+root->val),root->val);
    }
    
};
```

### 4.5. 

#### Path Sum系列

##### [\112. Path Sum](https://leetcode.com/problems/path-sum/)

基础题,Path Sum II 差不多，用vector<vector<int>>全局变量保存即可。

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
    bool ans = false;
    int target = 0;
    
    bool hasPathSum(TreeNode* root, int sum) {
        if(root==NULL) return false;
        target = sum;
        help(root,0);
        return ans;
    }
    
    void help(TreeNode* root,int num){
        if(root->left==NULL&&root->right==NULL&&root->val+num==target){
            ans = true;
            return;
        }
        num += root->val;
        if(root->left)  help(root->left,num);
        if(root->right)  help(root->right,num);
    }
};
```

##### [\437. Path Sum III](https://leetcode.com/problems/path-sum-iii/)

最开始和前面思考的类似，对于每一个节点有三种情况：延续前面的root，从该节点开始，以及不要该节点。但是这样做递归无疑会超时，因为重复了太多次情况。

其实最关键的点为：如果是从根节点开始计算是非常简单的，那就通过递归让每一个节点都当一次根节点。所以一道题用了两重递归。学到了！

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
    int ans = 0;
    
    int pathSum(TreeNode* root, int sum) {
        if(root==NULL) return 0;
        help(root,sum);
        pathSum(root->left,sum);
        pathSum(root->right,sum);
        return ans;
    }
    
    void help(TreeNode* root,int sum){
        if(root==NULL) return;
        sum -= root->val;
        if(!sum)    ans++;
        help(root->left,sum);
        help(root->right,sum);
    }
};
```

### 4.7.

#### [\96. Unique Binary Search Trees](https://leetcode.com/problems/unique-binary-search-trees/)

给定一个整数 *n*，求以 1 ... *n* 为节点组成的二叉搜索树有多少种？

一个常规动态规划。可以把内层循环单独看作一个 int help(int n)的函数，来专门计算每一个值的numTrees。

```c++
class Solution {
public:
    int numTrees(int n) {
        unordered_map<int,int> M;
        M[0] = 1;
        M[1] = 1;
        int left = 0;
        int right = 0;
        for(int i=2;i<=n;i++){
            int sum = 0;
            for(int j=1;j<=i;j++){
                left = M[j-1];
                right = M[i-j];
                sum += left*right;
            }
            M[i] = sum;
        }
        return M[n];
    }
    
};
```

#### 4.8. 

#### [\95. Unique Binary Search Trees II](https://leetcode.com/problems/unique-binary-search-trees-ii/)

不同的二叉搜索树，这次要返回这样的根节点

闻者伤心，因为一个愚蠢的错误卡了我半个多小时。

```c++
        vector<TreeNode*> ans;
        for(int i=bg;i<ed;i++){
            vector<TreeNode*> left = help(vec,bg,i);
            vector<TreeNode*> right = help(vec,i+1,ed);
          //老天爷，不能这么写...虽然在循环里面改变了指向，但是你push进去的时候是一个指针啊悲伤逆流成河
          	TreeNode *T = new TreeNode(vec[i]);
            for(int j=0;j<left.size();j++){
                for(int k=0;k<right.size();k++){
                    T->left = left[j];
                    T->right = right[k];
                    ans.push_back(T);
                }
            }
        }
```

更改后：当然也可以用数组保存状态实现动态规划，不过这里应该是个三维数组。

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
    vector<TreeNode*> generateTrees(int n) {
        vector<vector<vector<TreeNode*>>> memo(n+1, vector<vector<TreeNode*>>(n+1));
        vector<int> vec;
        vector<TreeNode*> ans;
        if(n==0) return ans;
        for(int i=1;i<=n;i++){
            vec.push_back(i);
        }
        return help(vec,0,n,memo);
    }
    
    vector<TreeNode*> help(vector<int> vec,int bg,int ed,vector<vector<vector<TreeNode*>>> &memo){
        if(memo[bg][ed].size())   return memo[bg][ed];
        int size = ed - bg;
        vector<TreeNode*> temp;
        temp.push_back(NULL);
        if(size == 0){
            memo[bg][ed] = temp;
            return temp;
        }
        temp.pop_back();
        
        vector<TreeNode*> ans;
        for(int i=bg;i<ed;i++){
            vector<TreeNode*> left = help(vec,bg,i,memo);
            vector<TreeNode*> right = help(vec,i+1,ed,memo);
            for(int j=0;j<left.size();j++){
                for(int k=0;k<right.size();k++){
                    TreeNode *T = new TreeNode(vec[i]);
                    T->left = left[j];
                    T->right = right[k];
                    ans.push_back(T);
                }
            }
        }
        return memo[bg][ed] = ans;
    }
};
```

## 二叉树

### 8.22

##### [P1087 FBI树](https://www.luogu.com.cn/problem/P1087)

已知最底层的节点，根据规则恢复整个tree，并后续遍历。

最开始就是最质朴的方式：用队列的方式进行倒着的层次遍历，构建整个tree，然后再后续遍历。

```c++
#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

int N;
string S;

struct TreeNode {
	char val;
	TreeNode* left;
	TreeNode* right;
	TreeNode(char x) : val(x), left(NULL), right(NULL) {}
};

queue<TreeNode*> qq;

void post(TreeNode* root) {
	if (root == NULL) return;
	post(root->left);
	post(root->right);
	cout << root->val;
}


int main()
{
	cin >> N >> S;
	for (int i = 0; i < S.size(); i++) {
		char ch;
		if (S[i] == '1')	ch = 'I';
		else ch = 'B';
		TreeNode* T = new TreeNode(ch);
		qq.push(T);
	}

	TreeNode* root = NULL;
	
	while (!qq.empty()) {
		int size = qq.size();
		if (size == 1) {
			root = qq.front();
			break;
		}
		for (int i = 0; i < size; i = i+2) {
			char ch;
			TreeNode* t1 = qq.front();
			qq.pop();
			TreeNode* t2 = qq.front();
			qq.pop();
			if (t1->val == 'I' && t2->val == 'I') ch = 'I';
			else if (t1->val == 'B' && t2->val == 'B') ch = 'B';
			else ch = 'F';
			TreeNode* T = new TreeNode(ch);
			T->left = t1;
			T->right = t2;
			qq.push(T);
		}
	}

	post(root);

	return 0;
}



```

也可以用递归来做（总算对递归熟练多了555），证明了一个论点：**很多树的题目事实上根本不用把树建立出来**。由于是后序遍历，先输出左树再输出右树，很容易想到两个并列的递归语句可以实现。通过return来保障对上一层的节点值判断：

```c++
#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

int N;
string S;


char help(int s, int e) {
	if (s == e) {
		cout << S[s];
		return S[s];
	}

	int mid = (s + e) / 2;
	char c1 = help(s, mid);
	char c2 = help(mid + 1, e);

	char final;
	if (c1 == 'I' && c2 == 'I') final =  'I';
	else if (c1 == 'B' && c2 == 'B') final = 'B';
	else final = 'F';

	cout << final;
	return final;
}


int main()
{
	cin >> N >> S;
	for (int i = 0; i < S.size(); i++) {
		if (S[i] == '1')	S[i] = 'I';
		else S[i] = 'B';
	}
	help(0, S.size()-1);

	return 0;
}



```

### 8.23

##### [P1030 求先序排列](https://www.luogu.com.cn/problem/P1030)

根据中序+后序求出先序，经典题，复习一遍。

```c++
#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

string mid,post;


void help(int ms,int me,int ps,int pe) {

  //Attention: 左右子树可能为空
	if (me < ms || pe < ps) return;
	if (ms == me) {
		cout << mid[ms];
		return;
	}

	char ch = post[pe];
	int pos = mid.find(ch);
	cout << ch;

	help(ms, pos - 1, ps, ps + pos - 1 - ms);
	help(pos + 1, me, ps + pos - ms, pe - 1);
  
}


int main()
{
	cin >> mid >> post;
	help(0,mid.size()-1,0,post.size()-1);

	return 0;
}



```

注意Attention处，**左右子树可能为空**。

## 树的直径

可以通过两次DFS解决

##### [SP1437 PT07Z - Longest path in a tree](https://www.luogu.com.cn/problem/SP1437)

无权无向树，找到整个树中的最长路径的长度。

```c++
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <queue>

using namespace std;


const int MAXN = 1e4 + 5;
int N,o,mx,node;
int visited[MAXN];
vector<vector<int> > G(MAXN, vector<int>());

void dfs(int s,int l) {
	visited[s] = 1;

	if (G[s].size() == 1 && s != o) {
		if (l > mx) {
			mx = l;
			node = s;
		}
		return;
	}
	for (int i = 0; i < G[s].size(); i++) {
		int v = G[s][i];
		if (!visited[v]) {
			dfs(v,l+1);
		}
	}
}

int main()
{
	cin >> N;
	for (int i = 0; i < N - 1; i++) {
		int u, v;
		cin >> u >> v;
		G[u].push_back(v);
		G[v].push_back(u);
	}

	o = 1;
	dfs(1, 0);
  
	memset(visited, 0, sizeof(visited));
	o = node;
	dfs(node, 0);

	cout << mx;
	return 0;
}



```

## 最近公共祖先

##### [P3379 【模板】最近公共祖先（LCA）](https://www.luogu.com.cn/problem/P3379)

"求解最近公共祖先，常用的方法是**树上倍增或者树链剖分**。"

之前做过的题是求指定两个节点的最近公共祖先，用递归，如果左右两个子树返回的值正好是这两个节点的值，该节点就是最近公共祖先。

先用这种暴力解法，时间复杂度为O(Mn)，得到30分，剩下的都TLE了。

```c++
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <queue>

using namespace std;


int N, M, S;
int u, v;
const int MAXN = 5e5 + 5;
int visited[MAXN];
vector<vector<int> > G(MAXN, vector<int>());

int LCA(int s) {
	if (s == u || s == v) return s;
	
	int flag = 0;
	int mark = 0;
	for (int i = 0; i < G[s].size(); i++) {
		int nex = G[s][i];
		if (visited[nex]) continue;
		visited[nex] = 1;
    
		int tmp = LCA(nex);
		if (tmp) {
			if(tmp == u || tmp == v) flag++;
			mark = tmp;
		}
	}

	if (flag < 2) return mark;
	return s;
}

int main()
{
	cin >> N >> M >> S;
	
	for (int i = 0; i < N - 1; i++) {
		int n1, n2;
		cin >> n1 >> n2;
		G[n1].push_back(n2);
		G[n2].push_back(n1);
	}

	for (int i = 0; i < M; i++) {
		memset(visited, 0, sizeof(visited));
		cin >> u >> v;
		cout << LCA(S) << endl;
	}

	return 0;
}
```

第二种暴力解法，由两个结点在同一高度的一起往上爬，每次同时爬**一步**，相遇的时候就是LCA。时间复杂度为O(Mlogn)，最终70分，TLE了三个点。

因此最开始需要：

- D[MAXN]：记录每个节点的深度
-  F[MAXN]：记录每个节点的父亲节点

<img src="/Users/apple/Library/Application Support/typora-user-images/image-20200824092837540.png" alt="image-20200824092837540" style="zoom:50%;" />

随后将二者跳到同一高度，每次同时爬一步，相遇的时候就是LCA。

```c++
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <queue>

using namespace std;


int N, M, S;
int u, v;
const int MAXN = 5e5 + 5;
int visited[MAXN], D[MAXN], F[MAXN];
vector<vector<int> > G(MAXN, vector<int>());


void depth(int node, int l) {
	D[node] = l;
	for (int i = 0; i < G[node].size(); i++) {
		int nex = G[node][i];

		if (visited[nex]) continue;
		visited[nex] = 1;

		F[nex] = node;
		depth(nex, l + 1);
	}
}

int dfs(int u, int v) {
	//假设u的深度小于v的深度
	if (D[u] != D[v]) {
		if (D[u] > D[v]) swap(u, v);
		return dfs(u, F[v]);
	}

	if (u == v) return u;
	return dfs(F[u], F[v]);
}

int main()
{
	cin >> N >> M >> S;
	
	for (int i = 0; i < N - 1; i++) {
		int n1, n2;
		cin >> n1 >> n2;
		G[n1].push_back(n2);
		G[n2].push_back(n1);
	}

	visited[S] = 1;
	depth(S, 1);

	for (int i = 0; i < M; i++) {
		int n1, n2;
		cin >> n1 >> n2;
		cout << dfs(n1, n2) << endl;
	}

	return 0;
}

```

第三种方法，**树上倍增**，是基于第二种方法的改进。之前第二种方法每次只跳一步，而树上倍增每次跳$2^i$步，并从大到小跳，例如32,16,8,4,2,1，大的跳不过去，再调小。

> 这是因为从小开始跳，可能会出现“悔棋”的现象。拿 55 为例，从小向大跳，5≠1+2+45\\=1+2+4,所以我们还要回溯一步，然后才能得出5=1+45=1+4；而从大向小跳，直接可以得出5=4+5=4+1。

因此，不再用F[]数组记录父亲节点，而是用F[]\[]记录每个节点$2^i$级别的祖先。

算法核心之一——**关键的递推式**：$2^i = 2^{i-1} + 2^{i-1}$

```
F[now][i] = F[F[now][i-1]][i-1]
```

代码如下，90分，TLE了一个点，暂时先放下，记住这种优化方式。

```c++
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <queue>

using namespace std;


int N, M, S;
int u, v;
const int MAXN = 5e5 + 5;
int visited[MAXN], D[MAXN], F[MAXN][20], lg[MAXN];
vector<vector<int> > G(MAXN, vector<int>());


void depth(int node, int l) {

	for (int i = 0; i < G[node].size(); i++) {
		int nex = G[node][i];

		if (visited[nex]) continue;
		visited[nex] = 1;

		D[nex] = l + 1;
		F[nex][0] = node;
		for (int j = 1; j <= lg[D[nex]]; j++) {
			F[nex][j] = F[F[nex][j - 1]][j - 1];
		}

		depth(nex, l + 1);
	}
}

int dfs(int u, int v) {
	//规定u的深度小于v的深度
	if (D[u] > D[v]) swap(u, v);


	while (D[u] < D[v]) {
		v = F[v][lg[D[v] - D[u]] - 1];
	}

	if (u == v) return u;

	for (int k = lg[D[u]]-1; k >= 0; k--) {
		if (F[u][k] != F[v][k]) {
			u = F[u][k];
			v = F[v][k];
		}
	}

	return F[u][0];
}

int main()
{
	cin >> N >> M >> S;
	
	for (int i = 0; i < N - 1; i++) {
		int n1, n2;
		cin >> n1 >> n2;
		G[n1].push_back(n2);
		G[n2].push_back(n1);
	}

	// 常数优化：预先算出log[i]+1的值
	for (int i = 1; i <= N; i++) {
		lg[i] = lg[i-1] + (1 << lg[i - 1] == i);
	}

	visited[S] = 1;
	D[S] = 0;
	depth(S, 0);

	for (int i = 0; i < M; i++) {
		int n1, n2;
		cin >> n1 >> n2;
		cout << dfs(n1, n2) << endl;
	}

	return 0;
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

