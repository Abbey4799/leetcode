> 记住，研究好方法比直接做新的题效果更好，消费脑力也更少

### 4.7.

#### [\99. Recover Binary Search Tree](https://leetcode.com/problems/recover-binary-search-tree/)

空间复杂度为O(n)的方法是很简单的。值得注意的是，中序遍历搜索树最后是个递增数组。如此可以解决任意节点错乱的情况。

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
    vector<TreeNode*> tmp;
    vector<int> vals;
    
    void recoverTree(TreeNode* root) {
        if(root==NULL) return;
        help(root);
        sort(vals.begin(),vals.end());
        for(int i=0;i<tmp.size();i++){
            tmp[i]->val = vals[i];
        }
    }
    
    void help(TreeNode* root){
        if(root==NULL) return;
        help(root->left);
        tmp.push_back(root);
        vals.push_back(root->val);
        help(root->right);
    }
};
```

重点是需要常数级别的空间复杂度。关键是[Morris算法](https://www.cnblogs.com/anniekim/archive/2013/06/15/morristraversal.html)。这是一个可以实现O(1)空间复杂度的遍历算法。用O(1)空间复杂度遍历难点在于遍历到子节点的时候，如何找到该节点的父节点。为了解决这个问题，Morris借助了线索二叉树的概念。

- 如果当前节点A->left==NULL，则输出A，A = A->right

  > 代表这就是中序遍历该输出的节点，考察中序遍历下一个应该考察的节点

- 如果A->left != NULL，在当前节点的==左子树==中找到当前节点在中序遍历下的==前驱节点B==

  > 代表还没有轮到中序遍历该输出的节点，需要标记该节点的位置

  - 如果前驱节点B->right==NULL，则B->right = A。A = A->left

    > 标记A的位置，考察中序遍历下一个应该考察的节点

  - 如果B->right == A，则B->right = NULL（恢复树的形状）。print(A)。A = A->right

    > 第二次遍历到，说明是前驱节点刚刚遍历完，因此现在可以输出当前节点A，考察中序遍历下一个应该考察的节点

- 重复上述两步，直到==当前节点为空==

<img src="/Users/apple/Library/Application Support/typora-user-images/image-20200409203956622.png" alt="image-20200409203956622" style="zoom: 67%;" />

```c++
//Morris算法中序遍历版本。
void Morris(BinNode<char> *root){
    BinNode<char> *pre;
    while(root!=NULL){
        if(root->lchild==NULL){
            cout << root->data << endl;
            root = root->rchild;
        }else{
            pre = root->lchild;
            while(pre->rchild!=NULL && pre->rchild!=root)   pre = pre->rchild;
            if(pre->rchild == NULL){
                pre->rchild = root;
                root = root->lchild;
            }else{
                pre->rchild = NULL;
                cout << root->data << endl;
                root = root->rchild;
            }
        }
    }
}

```

对Morris算法稍作修改即为该题正解。注意一定是**最开始和最后**出现**右边比左边大**的情况时的两个节点为出错根因。

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
    void recoverTree(TreeNode* root) {
        TreeNode* tmp = root;
        TreeNode *pre;
        TreeNode *flag1,*flag2;
        vector<TreeNode*> vec;
        pre = NULL;
        flag1 = NULL;
        flag2 = NULL;
        while(tmp!=NULL){
            if(tmp->left==NULL){
                vec.push_back(tmp);
                if((vec.size()>1&&vec[vec.size()-1]->val<vec[vec.size()-2]->val)){
                    if(flag1==NULL) flag1 = vec[vec.size()-2];
                    flag2 = vec[vec.size()-1];
                }
                tmp = tmp->right;
            }else{
                pre = tmp->left;
                while(pre->right!=NULL && pre->right!=tmp)   pre = pre->right;
                if(pre->right == NULL){
                    pre->right = tmp;
                    tmp = tmp->left;
                }else{
                    pre->right = NULL;
                    vec.push_back(tmp);
                    if((vec.size()>1&&vec[vec.size()-1]->val<vec[vec.size()-2]->val)){
                        if(flag1==NULL) flag1 = vec[vec.size()-2];
                        flag2 = vec[vec.size()-1];
                    }
                    tmp = tmp->right;
                }
            }
        }
        swap(flag1->val,flag2->val);
    }
    
};
```

### 4.10

#### [\450. Delete Node in a BST](https://leetcode.com/problems/delete-node-in-a-bst/)

##### 1. 方法一：初学者写法

这个是最初数据结构ppt上最笨的写法，也是当年拿来做pj的方法。思路直观，但是代码量长。

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
  //root最多只有一个孩子不为NULL
    TreeNode* deleteone(TreeNode* root){
        if(root->left==NULL&&root->right==NULL) return NULL;
        if(root->left!=NULL)    return root->left;
        return root->right;
    }
    
  //root两个孩子均非NULL
    TreeNode* deletetwo(TreeNode* root){
        TreeNode *pre,*parent;
        pre = root->left;
        parent = root;
      //找到中序的前驱节点，复制其值给root，转化为删除前驱节点（只有一个孩子）
        while(pre->right!=NULL){
            parent = pre;
            pre = pre->right;
        }
        root->val = pre->val;
        if(pre->val > parent->val) parent->right = deleteone(pre);
        else parent->left = deleteone(pre);
        return root;
    }
   
  //找到要删除的节点，并且标记出parent
    TreeNode* find(TreeNode* root,TreeNode* &parent,int key){
        if(root==NULL) return NULL;
        if(root->val == key)    return root;
        parent = root;
        if(root->val > key) return find(root->left,parent,key);
        else return find(root->right,parent,key);
    }
    
    TreeNode* deleteNode(TreeNode* root, int key) {
        TreeNode *tmp,*parent;
        parent = NULL;
        tmp = find(root,parent,key);
        if(tmp==NULL)   return root;
        if(tmp->right && tmp->left){
            tmp = deletetwo(tmp);
            return root;
        }else{
            if(parent==NULL){
                if(tmp->left)   return tmp->left;
                return tmp->right;
            }
            if(tmp->val >= parent->val) parent->right = deleteone(tmp);
            else parent->left = deleteone(tmp);
            return root;
        }
        
    }
};
```

##### 2. 方法二：递归法

用递归完成上面一系列操作，将前驱节点的值复制给该节点，然后转化为删除前驱节点。

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
    TreeNode* deleteNode(TreeNode* root, int key) {
        //找到节点
        if(root==NULL)  return NULL;
        if(root->val > key) root->left = deleteNode(root->left,key);
        else if(root->val < key) root->right = deleteNode(root->right,key);
        else{
            if(!root->left || !root->right){
                root = root->left?root->left:root->right;
            }else{
                //找到中序遍历前面的那个节点
                TreeNode *pre = root->left;
                while(pre->right)   pre = pre->right;
                root->val = pre->val;
                //之前的想法是细致到去找到pre的前驱节点，但是完全可以通过deleteNode直接再去删除一次
                root->left = deleteNode(root->left,pre->val);
            }
        }
        return root;
    }
};
```

如果并非BST而是任意二叉树，最后改成这样即可。

```c++
        root->left = deleteNode(root->left, key);
        root->right = deleteNode(root->right, key);
```



##### 3. 方法三：迭代法

本质还是删除该节点，所以把它的右孩子托付给前驱节点，无牵无挂，就可以返回左孩子

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
    TreeNode* deleteNode(TreeNode* root, int key) {
        if(root==NULL) return NULL;
        TreeNode *pre = NULL,*cur = root;
        cur = find(cur,pre,key);
        if(cur==NULL) return root;
        if(pre==NULL) return del(root);
        if(pre->val > cur->val) pre->left = del(cur);
        else pre->right = del(cur);
        return root;
    }
    
  //用递归找到要删除的节点，并且标记出parent
    TreeNode* find(TreeNode* root,TreeNode* &parent,int key){
        if(root==NULL) return NULL;
        if(root->val == key)    return root;
        if(root->val > key) return find(root->left,root,key);
        else return find(root->right,root,key);
    }
    
    TreeNode* del(TreeNode* root){
        if(!root->left||!root->right)   return root->left?root->left:root->right;
        //两个孩子都有，找到前驱节点（没有右孩子），所以把它的右孩子托付给前驱节点，然后返回左孩子的
        TreeNode *cur = root->left;
        while(cur->right)   cur = cur->right;
        cur->right = root->right;
        return root->left;
    }
    
};
```

### 4.11

#### [\701. Insert into a Binary Search Tree](https://leetcode.com/problems/insert-into-a-binary-search-tree/)

搜索树的插入。常规题，热个身

##### 1. 精简版

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
    TreeNode* insertIntoBST(TreeNode* root, int val) {
        if(root==NULL)  return new TreeNode(val);
        if(val > root->val)   root->right = insertIntoBST(root->right,val);
        else root->left = insertIntoBST(root->left,val);
        return root;
    }

};
```

##### 2. 初始的思路

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
    TreeNode* insertIntoBST(TreeNode* root, int val) {
        TreeNode *parent = NULL;
        if(root==NULL) return NULL;
        help(root,parent,val);
        return root;
    }
    //通过return TreeNode*可以解决父亲节点的问题
    void help(TreeNode* root,TreeNode* &parent,int val){
        if(root==NULL){
            if(val > parent->val) parent->right = new TreeNode(val);
            else parent->left = new TreeNode(val);
            return;
        }
        if(root->val > val) help(root->left,root,val);
        else help(root->right,root,val);
    }
};
```

#### [\110. Balanced Binary Tree](https://leetcode.com/problems/balanced-binary-tree/)

##### 1. 精简版

后序遍历的递归法比较常用。

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
    bool Balance = true;
    
    bool isBalanced(TreeNode* root) {
        help(root);
        return Balance;
    }
    
    int help(TreeNode* root){
        if(root==NULL) return 0;
        int left,right;
        left = help(root->left);
        right = help(root->right);
        if(abs(left-right)>1)   Balance = false;
        return max(left,right) + 1;
    }
    
};
```

##### 2. 初始的思路

前序遍历的递归法，要求最高层次的状态已经被保存，一般不常用。还是后序遍历的递归法比较常用。以后就不要这样想啦。

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
    unordered_map<TreeNode*,int> M;
    
    bool isBalanced(TreeNode* root) {
        M[NULL] = 0;
        height(root);
        return help(root);
    }
    
  //但其实help和height函数可以合起来，最后只用递归一次
    bool help(TreeNode* root){
        if(root==NULL) return true;
        if(abs(M[root->left]-M[root->right])>1) return false;
        bool left,right;
        left = help(root->left);
        right = help(root->right);
        if(left&&right) return true;
        return false;
    }
    
    int height(TreeNode* root){
        if(root==NULL) return 0;
        return M[root] = max(height(root->left),height(root->right)) + 1;
    }
    
};
```

#### 完全二叉树

##### [\222. Count Complete Tree Nodes](https://leetcode.com/problems/count-complete-tree-nodes/)

###### 1. 通用版

对于任意树都可以使用，但是没有利用完全二叉树的条件

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
    int count = 0;
    int countNodes(TreeNode* root) {
        if(root==NULL) return 0;
        count++;
        countNodes(root->left);
        countNodes(root->right);
        return count;
    }
};
```

当然还可以更简洁

```c++
class Solution {
public:
    int countNodes(TreeNode* root) {
        return root ? (1 + countNodes(root->left) + countNodes(root->right)) : 0;
    }
};
```

##### 4.22

##### [\919. Complete Binary Tree Inserter](https://leetcode.com/problems/complete-binary-tree-inserter/)

热身热身。注意申请全局变量时一定要对TreeNode*进行定义（NULL或者new创建），否则会报错。

完全二叉树一般都可以用数组来做。因为一个节点的角标为x，父节点的角标一定为(x-1)/2

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
class CBTInserter {
public:
    TreeNode *Root = NULL;
    vector<TreeNode*> ini;
    
    CBTInserter(TreeNode* root) {
        if(root==NULL) return;
        queue<TreeNode*> Q;
        TreeNode* temp = new TreeNode(-1);
        Q.push(root);
        Root = root;
        while(!Q.empty()){
            temp = Q.front();
            Q.pop();
            ini.push_back(temp);
            if(temp->left)  Q.push(temp->left);
            if(temp->right)  Q.push(temp->right);
        }
    }
    
    int insert(int v) {
        TreeNode *parent = ini[(ini.size()-1)/2];
        TreeNode *temp =  new TreeNode(v);
        if(parent->left)    parent->right = temp;
        else parent->left = temp;
        ini.push_back(temp);
        return parent->val;
    }
    
    TreeNode* get_root() {
        return Root;
    }
};

/**
 * Your CBTInserter object will be instantiated and called as such:
 * CBTInserter* obj = new CBTInserter(root);
 * int param_1 = obj->insert(v);
 * TreeNode* param_2 = obj->get_root();
 */
```

最开始读题读错了，以为CBTInserter函数的意思是要把原来的tree转化为完全二叉树，多做无伤大雅，但是以后读题希望脑子灵光一点。我的思路是先通过BFS存储所有的节点，再用insert函数来构建完全二叉树。insert函数的思想为：用一个全局变量queue tQ存储所有子节点小于两个的节点。这样queue的最前端总是完全二叉树最后那一个节点。如果是题目的意思则更简单，解法如下：

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
class CBTInserter {
public:
    TreeNode *Root = NULL;
    vector<int> ini;
    queue<TreeNode*> tQ;
    
    CBTInserter(TreeNode* root) {
        if(root==NULL) return;
        queue<TreeNode*> Q;
        TreeNode* temp = new TreeNode(-1);
        Q.push(root);
        Root = root;
        while(!Q.empty()){
            temp = Q.front();
            Q.pop();
            tQ.push(temp);
            if(temp->left)  Q.push(temp->left);
            if(temp->right)  Q.push(temp->right);
        }
    }
    
    int insert(int v) {
        TreeNode* temp = new TreeNode(v);
        while(1){
            TreeNode* t = tQ.front();
            if(!t->left)   t->left = temp;
            else if(!t->right)   t->right = temp;
            else{
                tQ.pop();
                continue;
            }
            tQ.push(temp);
            return t->val;
        }
    }
    
    TreeNode* get_root() {
        return Root;
    }
};

/**
 * Your CBTInserter object will be instantiated and called as such:
 * CBTInserter* obj = new CBTInserter(root);
 * int param_1 = obj->insert(v);
 * TreeNode* param_2 = obj->get_root();
 */
```

##### [\958. Check Completeness of a Binary Tree](https://leetcode.com/problems/check-completeness-of-a-binary-tree/)

注意不要拘泥于queue中不能有NULL。完全二叉树最关键的判断条件：如果出现空节点，则queue之后不能继续有非空节点。

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
    bool isCompleteTree(TreeNode* root) {
        //不要拘泥于queue中不能有NULL
        if(!root) return true;
        queue<TreeNode*> qq;
        qq.push(root);
        while(!qq.empty()){
            TreeNode *temp = qq.front();
            qq.pop();
            //如果出现空节点，则queue之后不能继续有非空节点。
            if(temp==NULL) break;
            qq.push(temp->left);
            qq.push(temp->right);
        }
        while(!qq.empty()){
            TreeNode *temp = qq.front();
            qq.pop();
            if(temp) return false;
        }
        return true;
    }

};
```

第一反应的brutal force的方法。也在此记录。分为两种情况讨论

- 倒数第二层以上的必须两个孩子都有
- 倒数第二层的，从右到左数
  - 如果某个节点有右孩子没左孩子，false
  - 如果某个节点有左孩子或右孩子或都有&&flag==0，flag=1
  - 如果flag==1，某个节点没有左孩子或右孩子，false

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
    unordered_map<int,vector<TreeNode*>> M;
    int height = 0;
    bool isCompleteTree(TreeNode* root) {
        help(root);
        if(height<2) return true;
        for(auto& v:M){
            vector<TreeNode*> vec = v.second;
            int flag = 0;
            if(v.first == height-2){
                for(int i=vec.size()-1;i>=0;i--){
                    if(vec[i]->left==NULL&&vec[i]->right!=NULL) return false;
                    if((vec[i]->left||vec[i]->right)&&flag==0) flag=1;
                    else if((!vec[i]->left||!vec[i]->right)&&flag) return false;
                }                
            }else if(v.first < height-2){
                for(int i=vec.size()-1;i>=0;i--){
                    if(vec[i]->right==NULL||vec[i]->left==NULL) return false;
                }
            }
        }
        return true;
    }
    
    void help(TreeNode *root){
        queue<TreeNode*> Q;
        Q.push(root);
        while(!Q.empty()){
            int size = Q.size();
            for(int i=0;i<size;i++){
                TreeNode* temp = Q.front();
                Q.pop();
                M[height].push_back(temp);
                if(temp->left)  Q.push(temp->left);
                if(temp->right)  Q.push(temp->right);
            }
            height++;
        }
    }
};
```

