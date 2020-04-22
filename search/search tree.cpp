#include <iostream>
using namespace std;

template<class ElementType> class search_tree;
template <class ElementType> class Node  {
    friend class search_tree<ElementType>;
private:
    ElementType data; //权值
    Node <ElementType> *right; //指向右结点的指针
    Node <ElementType> *left; //指向左结点的指针
    int ltag=0,rtag=0;
public:
    Node( ) { }
    Node(ElementType d):data(d), left(NULL), right(NULL) { }
};

template < class ElementType> class search_tree  {
private:
    Node <ElementType> *root; //指向根结点
    int length;//二叉树中元素的个数
public:
    search_tree(){root = NULL;}//构造函数
    ~search_tree (){}; //析构函数
    Node<ElementType>* createTree();
    Node<ElementType>* search(ElementType e,Node<ElementType>*& p); //找值fe的结点
    int insert(ElementType e ) ;  //插入元素取值为e的数据项
    Node<ElementType>* createInThread(Node<ElementType>* t,Node<ElementType>* &pre);
    Node<ElementType>* InorderFindNext(Node<ElementType>*t);
    void inorder(Node<ElementType>*t);
    
    
};



template <class ElementType>
Node<ElementType> *search_tree < ElementType> ::search( ElementType a,Node<ElementType>*& p){
    Node<ElementType>* q;
    q = root;
    while((q!=NULL)&& (q->data!=a)){
        if(a<q->data){
            p = q;
            q = q->left;
        }
        else{
            p = q;
            q=q->right;
        }
    }
    return q;
}



template < class ElementType>
int search_tree <ElementType>::insert(ElementType a)    {
    Node<ElementType> *p=NULL,*q=NULL,*r;
    q=search(a,p);
    //从根*p_t找含元素a的结点，返回时q不为空则指向找到
    //的结点，q为空则指向应插入的位置。p指向q的父结点。
    if(q!=NULL)  return 1;      //找到就直接返回
    r=new Node<ElementType>(a);
    if(p==NULL) root = r; //查找树为空
    else if(p->data>a)
        p->left=r;
    else p->right=r;
    return 0;
}


template <class ElementType>
Node<ElementType>* search_tree<ElementType>::createTree(){
    ElementType data;
    cout << "please input data:";
    while(cin>>data) insert(data);
    return root;
}




template <class ElementType>
Node<ElementType>*  search_tree<ElementType>::createInThread(Node<ElementType>* t,Node<ElementType>*& pre){
    if(t==NULL) return NULL;
    createInThread(t->left,pre);
    if(t->left==NULL){
        t->left=pre;
        if(pre!=NULL) t->ltag=1;
    }
    if(pre!=NULL&&pre->right==NULL){
        pre->right = t;
        pre->rtag =1;
    }
    pre = t;
    createInThread(t->right,pre);
    return t;
}



template <class ElementType>
Node<ElementType>* search_tree<ElementType>::InorderFindNext(Node<ElementType>*t){
    if(t->rtag==1||t->right==NULL) return t->right;
    t=t->right;
    while(t->ltag==0) t=t->left;
    return t;
}


template <class ElementType>
void search_tree<ElementType>::inorder(Node<ElementType>*t){
    while(t->left!=NULL) t=t->left;
    while(t!=NULL){
        cout << t->data << endl;
        t = InorderFindNext(t);
    }
    return;
}



int main(){
    search_tree<int> root;
    Node<int>* a,*pre;
    a= root.createTree();
    pre = NULL;
    root.createInThread(a,pre);
    root.inorder(a);
    return 0;
}
