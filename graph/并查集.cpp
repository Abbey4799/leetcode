//并查集的实现

#include<iostream>
using namespace std;
const int MAX_N = 10000;

int par[MAX_N]; //父节点，存储该节点对应的根节点的位置
int rank[MAX_N]; //树的高度，存储该树的位置

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


int main(){
    int n = 10 ;
    init(n);
    unite(1, 2);
}
