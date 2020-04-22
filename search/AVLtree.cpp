#include <iostream>
using namespace std;

template<class Type>
struct AvlTree{
    Type data;
    int height;
    AvlTree *left;
    AvlTree *right;
    AvlTree(Type x){
        data = x;
        left = NULL;
        right = NULL;
        height = 0;
    }
    
};



int max(int a,int b){
    return a>b?a:b;
}

template<class Type>
int getheight(AvlTree<Type>* T){
    if(T==NULL) return -1;
    return T->height;
}


template<class Type>
AvlTree<Type>* singleRight(AvlTree<Type>* T){
    AvlTree<Type>* L = T->left;
    T->left =  L->right;
    L->right = T;
    T->height = max(getheight(T->left),getheight(T->right)) +1;
    L->height = max(getheight(L->left),getheight(L->right))  +1;
    return L;
}

template<class Type>
AvlTree<Type>* singleLeft(AvlTree<Type>* T){
    AvlTree<Type>* R = T->right;
    T->right =  R->left;
    R->left = T;
    T->height = max(getheight(T->left),getheight(T->right))  +1;
    R->height = max(getheight(R->left),getheight(R->right))  +1;
    return R;
}

template<class Type>
AvlTree<Type>* doubleLeft(AvlTree<Type>* T){
    T->left = singleLeft(T->left);
    return singleRight(T);
    
}


template<class Type>
AvlTree<Type>* doubleRight(AvlTree<Type>* T){
    T->right = singleRight(T->right);
    return singleLeft(T);
    
}

template<class Type>
AvlTree<Type>* Insert(AvlTree<Type>* T,Type x){
    if(T==NULL) T = new AvlTree<Type>(x);
    else if(x < T->data){
        T->left = Insert(T->left,x);
        if(getheight(T->left)-getheight(T->right) ==2 ){
            if(x < T->left->data) T = singleRight(T);
            else T = doubleLeft(T);
        }
    }
    else if(x > T->data){
        T->right = Insert(T->right ,x);
        if(getheight(T->right)-getheight(T->left) ==2  ){
            if(x > T->right->data) T = singleLeft(T);
            else T = doubleRight(T);
        }
    }
    T->height = max(getheight(T->left),getheight(T->right))+1;
    return T;
}

template<class Type>
void inorder(AvlTree<Type>* t) {
    if(t != NULL)  {
        inorder(t->left);
        cout<<t->data<<endl;
        inorder(t->right);
    }
}

int main()
{
    AvlTree<int>* root = NULL;
    root = Insert(root,1);
    root = Insert(root,8);
    root = Insert(root,2);
    root = Insert(root,6);
    root = Insert(root,5);
    root = Insert(root,4);
    inorder(root);
    return 0;
}
