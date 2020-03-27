//
//  main.cpp
//  rank
//
//  Created by apple on 2020/2/22.
//  Copyright © 2020 apple. All rights reserved.
//

#include <iostream>
#include <string>
#include <stack>
#include <vector>
using namespace std;


//STL中的heap应用
void heap(){
    string name = "heap_sort:  ";
    int i,number[20]={29,23,20,22,17,15,26,51,19,12,35,40};
    make_heap(&number[0],&number[12]);
    showarray(name, number, 13);
    //结果是:51 35 40 23 29 20 26 22 19 12 17 15
    make_heap(&number[0],&number[12],cmp);
    showarray(name, number, 13);
    //结果：12 17 15 19 23 20 26 51 22 29 35 40 最小堆
    number[12] = 8;
    push_heap(&number[0],&number[13],cmp);
    showarray(name, number, 13);
    //结果：8 17 12 19 23 15 26 51 22 35 40 20
    pop_heap(&number[0],&number[13],cmp);
    //这里弹出的意思是放置到最后，然后把剩余的重新调整为堆
    showarray(name, number, 13);
    //结果：12 17 15 19 23 20 26 51 22 29 35 40
    sort_heap(&number[0],&number[12],cmp);
    //heap_sort:  51,40,35,29,26,23,22,20,19,17,15,12,8,
    showarray(name, number, 13);
    
}

int main(int argc, const char * argv[]) {
    heap();
}
