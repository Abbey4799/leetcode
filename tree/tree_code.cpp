//
//  main.cpp
//  tree
//
//  Created by apple on 2020/3/11.
//  Copyright © 2020 apple. All rights reserved.
//

#include <iostream>
#include <stack>
#include <queue>

#define MAXN 10
using namespace std;

////前序遍历树
//template<class Type> struct node{
//    Type data;
//    struct node *child[MAXN];
//    node <Type> *lchild;
//    node <Type> *rchild;
//};
//
//template<class Type> struct Tree{
//    private:
//        node<Type> *root;
//    public:
//        void preorder(node<Type> *t);
//};
//
//
////递归版
//template<class Type>
//void preorder(node<Type> *t){
//    int m = MAXN;
//    if(t){
//        cout << t->data << endl;
//        for(int i=0;i<m;i++) preorder(t->child[i]);
//    }
//}
//
////非递归版
//template<class Type>
//void preorder_2(node<Type> *t){
//    stack<node<Type>*> ss;
//    ss.push(t);
//    while(!ss.empty()){
//        //学到就是赚到
//        ss.pop(&t);
//        cout << t->data << endl;
//        for(int m=MAXN-1;m>=0;m--){
//            if(t->child[m]) ss.push(t->child[m]);
//        }
//    }
//}
//
////层次遍历——都用队列
//template<class Type>
//void layerorder(node<Type> *t){
//    queue<node<Type>*> qq;
//    qq.push(t);
//    while(!qq.empty()){
//          //错的，不能这么用
//        qq.pop(&t);
//        cout << t->data << endl;
//        for(int m=MAXN-1;m>=0;m--){
//            if(t->child[m]) qq.push(t->child[m]);
//        }
//    }
//}
//
////三次树森林转换为二叉树
//template <class Type> struct node3 {
//    Type data;       node3 <Type>  *child[3];
//};
//
//template <class Type> struct node2 {
//    Type data;  node2 <Type> *lchild;  node2 <Type> *rchild;
//};
//
//template <class Type>
//node2 <Type> *beta(node3 <Type> *p, node3 <Type> *q, node3 <Type> *r) {
//    node2 <Type> *t;
//    if(p==NULL) return NULL;
//    t = new node2 <Type> ();       t->data = p->data;
//    t->lchild = beta(p->child[0],  p->child[1],  p->child[2]);
//    t->rchild = beta(q,r,NULL);       return t;
//}

//遍历二叉树
//可运行
template <class Type> struct BinNode {
    Type data;
    BinNode(){lchild = rchild = NULL;}
    BinNode <Type> *lchild;
    BinNode <Type> *rchild;
};


template<class Type>struct stkNode{
    BinNode<Type> *ptr;
    int flag;
};


//根据以下输入建立二叉树
//6(2(1,3(,4(,5))),7(,9(8,)))#
void CreateBinTree2(istream& in, BinNode <char> *&BT){
    string a;
    int flag=0;
    stack<BinNode<char>*> s;
    BinNode <char>* p,*q;
    in >> a;
    BT = new BinNode<char>;
    BT->data = a[0];
    p=BT;
    for(int i=1;a[i]!='#';i++){
        switch (a[i]) {
            case '(':
                flag=0;
                s.push(p);
                break;
            case ',':
                flag=1;
                break;
            case ')':
                s.pop();
                break;
            default:
                if(flag==0){
                    p=s.top();
                    q=new BinNode<char>;
                    q->data = a[i];
                    p->lchild = q;
                    p=q;
                }
                else{
                    p=s.top();
                    q=new BinNode<char>;
                    q->data = a[i];
                    p->rchild = q;
                    p=q;
                }
                break;
        }
    }
}

void mid(BinNode<char> *t){
    stack<BinNode<char> *> s;
    while(t!=NULL||!s.empty()){
        while(t!=NULL){
            s.push(t);
            t=t->lchild;
        }
        if(!s.empty()){
            t=s.top();
            s.pop();
            cout << t->data;
            t=t->rchild;
        }
        cout << endl;
    }
}



void pre(BinNode<char> *t){
    stack<BinNode<char> *> s;
    s.push(t);
    while(!s.empty()){
        t = s.top();
        s.pop();
        cout << t->data << endl;
        //注意要判断是否为NULL
        if(t->rchild) s.push(t->rchild);
        if(t->lchild) s.push(t->lchild);
    }
}


//后序遍历
void post(BinNode<char>* root){
    stack<stkNode<char>> S;
    stkNode<char> w;
    do{
        while(root!=NULL){
            //分配flag
            w.flag = 0;
            w.ptr = root;
            S.push(w);
            root = root->lchild;
        }
        w = S.top();
        S.pop();
        switch (w.flag) {
            case 0:
                //第一次遇到
                w.flag = 1;
                S.push(w);
                root = w.ptr->rchild;
                break;
            case 1:
                //第二次遇到
                cout << w.ptr->data << endl;
                break;
            default:
                break;
        }
    }while(!S.empty());
}


//计算结点数
int size(BinNode<char>* root){
    if(root==NULL) return 0;
    else return 1+size(root->lchild)+size(root->rchild);
}

//计算高度
int height(BinNode<char>* root){
    if(root==NULL) return -1;
    int left=height(root->lchild)+1;
    int right=height(root->rchild)+1;
    return left>right?left:right;
}


int lr(BinNode<char>* root,int& ans){
    if(root==NULL) return -1;
    int left = lr(root->lchild,ans)+1;
    int right = lr(root->rchild,ans)+1;
    if((left+right+1)>ans) ans = left+right+1;
    return left>right?left:right;
}



//寻找共同祖先，总有一个路口分道扬镳，其他时刻总有一个子树返回NULL
BinNode<char>* getLCA(BinNode<char>* root,char X,char Y){
    BinNode<char> *left,*right;
    if(root == NULL) return NULL;
    if(root->data == X || root->data == Y) return root;
    left = getLCA(root->lchild, X, Y);
    right = getLCA(root->rchild, X, Y);
    if(left && right) return root;
    if(left) return left;
    return right;
}

int help(BinNode<char> * root,int& ans){
    int lmax,rmax;
    if(root==NULL) return -1;
    lmax = 1+help(root->lchild,ans);
    rmax = 1+help(root->rchild,ans);
    ans = ans>(lmax+rmax)?ans:(lmax+rmax);
    return lmax>rmax?lmax:rmax;
}

int maxDistance(BinNode<char> * root){
    int ans=0;
    help(root,ans);
    return ans;
//    return height(root->lchild) + height(root->rchild) + 2;
}


//线索树的一些知识点

template<class Type> struct node
{
    Type data;
    node<Type>*lchild,*rchild;
    int ltag,rtag;
};


template<class Type>
node<Type> *pred(node<Type>* t){
    if(t->ltag==1||t->lchild==NULL) return t->lchild;
    t = t->lchild;
    //注意不是while(t->rchild) 是寻找真实的节点
    while(t->rtag==0)    t = t->rchild;
    return t;
}

template<class Type>
node<Type> *succ(node<Type>* t){
    if(t->rtag==1||t->rchild==NULL) return t->rchild;
    t = t->rchild;
    while(t->ltag==0)   t = t->lchild;
    return t;
}

template<class Type>
node<Type> *first(node<Type>* t){
    if(t==NULL) return NULL;
    //根本不用判断什么，一直左孩子到尽头
    while(t->lchild)    t=t->lchild;
}

template<class Type>
void midorder(node<Type>* t){
    t = first(t);
    while(t){
        cout << t->data;
        t = succ(t);
    }
}

template<class Type>
void createInThread(node<Type> *t,node<Type>* &pre){
    if(t==NULL) return;
    createInThread(t->lchild, pre);
    //注意pre==NULL意味着此时t是第一个，不需要改变
    if(pre!=NULL&&t->lchild==NULL){
        t->lchild = pre;
        t->ltag = 1;
    }
    if(pre!=NULL&&pre->rchild==NULL){
        pre->rchild = t;
        pre->rtag = 1;
    }
    pre = t;
    createInThread(t->rchild, pre);
}

//本质是 放前面，对于没有实在左孩子的就加一个左孩子 不然就在实在左孩子旁边加一个右孩子
template<class Type>
void left_insert(node<Type> *p,node<Type> *q,node<Type>**p_head){
    node<Type>*r;
    if(p->ltag==1||p->lchild==NULL){//p没有左孩子（才会有后继）
        //左边完全复制
        q->lchild=p->lchild;
        q->ltag=p->ltag;//肯定是1，不一定哦
        q->rchild=p;
        q->rtag=1;//q是作为p的左孩子出现的，所以q的右孩子当然不能是p呀
        p->lchild=q;
        p->ltag=0;//真·孩子
    }
    else{
        r=pred(p);
        q->rchild=r->rchild;
        q->rtag=r->rtag;
        q->lchild=r;
        q->ltag=1;
        r->rchild=q;
        r->rtag=0;
    }
}





//6(2(1,3(,4(,5))),7(,9(8,)))#

int main(){
    BinNode<char> *a;
    int ans=0;
    CreateBinTree2(cin,a);
    cout << maxDistance(a);
//    lr(a,ans);
//    cout << ans <<endl;
}
