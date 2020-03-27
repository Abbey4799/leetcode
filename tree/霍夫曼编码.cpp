#include   <iostream>
#include <vector>
using namespace std;
#define MAXN 100



typedef struct node  {
    char data;
    string code;
    struct node *lchild;
    struct node *rchild;
}NODE;




NODE *create_huffman_tree(vector<char> a, vector<int> w, int n)  {
    NODE *addr[2*n-1];
    int n1, n2, i, j;
    int u, v, min1, min2;
    for(i = 0; i<n; i++)  {
        addr[i] =new NODE();
        addr[i]->data = a[i];
        addr[i]->lchild = NULL;
        addr[i]->rchild = NULL;
        w[i] = -w[i];
    }
    for (i=n; i<2*n-1; i++){
        n1 = -1;
        min1 = 9999;
        n2 = -1;
        min2 = 9999;
        for(j=0; j<i; j++)
        {
            v = w[j];
            u = -v;
            if(u>0)
                if(u<min1){
                    min2 = min1;
                    n2 = n1;
                    min1 = u;
                    n1 = j;  }
                else if(u < min2){
                    min2 = u;
                    n2 = j;
                }  // min2是次小
        }
        addr[i] = new NODE();
        addr[i]->data = '*';
        addr[i]->lchild = addr[n1];
        addr[i]->rchild = addr[n2];
        w[i] = w[n1] + w[n2];
        w[n1] = -w[n1];
        w[n2] = -w[n2];
    }
    w[2*n-2] = -w[2*n-2];
    return addr[2*n-2] ;
}

void coding(NODE* t){
    if(t->lchild!=nullptr){
        t->lchild->code = t->code + "0";
        coding(t->lchild);
    }
    if(t->rchild!=nullptr){
        t->rchild->code = t->code + "1";
        coding(t->rchild);
    }
}


void preorder(NODE* t){
    if(t!=NULL){
        if(t->data!='*') cout << t->data <<":" << t->code << endl;
        preorder(t->lchild);
        preorder(t->rchild);
    }
}


int main(){
    string temp;
    vector<char> a;
    vector<int> w;
    cout << "please input your source text:";
    cin >> temp;
    vector<char>::iterator iter;
    for(int i=0;i<temp.size();i++){
        iter = find(a.begin(),a.end(),temp[i]);
        if(iter == a.end()){
            a.push_back(temp[i]);
            w.push_back(1);
        }
        else{
            long int i = iter-a.begin();
            w[i]++;
        }
    }
    for(int i=a.size();i<(2*a.size()-1);i++){
        w.push_back(0);
    }
    NODE* ans;
    ans = create_huffman_tree(a, w, a.size());
    coding(ans);
    preorder(ans);
}
