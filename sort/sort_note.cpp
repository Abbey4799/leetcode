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

void showarray(string name,int* array,int length){
    cout << name;
    for(int i=0;i<length;i++){
        cout << array[i] << ",";
    }
    cout << endl;
}

void swap(int& a,int& b){
    int temp;
    temp = a;
    a = b;
    b = temp;
}


int min(int a,int b){
    return (a<b)?a:b;
}

/*
插入排序：
将一个记录插入到已排好顺序的序列中，
形成一个新的、记录数增1的有序序列。
*/


//插入排序
void insertion_sort(int* array,int length){
    if(length==0){
        return;
    }
    int temp = array[0];
    for(int i=1;i<length;i++){
        temp = array[i];
        for(int j=i-1;j>=0&&temp < array[j];j--){
            array[j+1] = array[j];
            array[j] = temp;
        }
        //其实也可以把array[j+1] = temp放在这里，但是这个时候j就不能是个局部变量，它要像小旗帜一样标注出比temp小的位置
        //默认包含了最开始
    }
    string name = "insertion_sort:  ";
    showarray(name,array,length);
}

void insertion_sort_2(int* array,int length){
    if(length==0){
        return;
    }
    int temp = array[0];
    int j = 0;
    for(int i=1;i<length;i++){
        temp = array[i];
        for(j=i-1;j>=0&&temp < array[j];j--){
            array[j+1] = array[j];
        }
        array[j+1] = temp;
    }
    string name = "insertion_sort_2:  ";
    showarray(name,array,length);
}


/*
冒泡排序:
基本思想:依次比较*相邻*的两个元素的顺序，
如果顺序不对，则将两者交换，
最值元素“上浮”到序列一端，然后缩小排序范围
重复这样的操作直到整个序列被排好序。
*/
void bubble_sort(int* a,int length){
    for(int i = 0;i < length;i++){
        for(int j = length-1;j > i;j--){
            if(a[j] < a[j-1]) swap(a[j-1],a[j]);
        }
    }
    string name ="bubble_sort:  ";
    showarray(name, a, length);
}

/*
选择排序：
1. 首先选出最小的项，与第一项交换位置
2. 选第二小的项，与第二项交换位置
...
与冒泡排序区别在于选择待排序列*最小*项的操作只进行一次交换：

*/

void selection_sort(int* a,int length){
    for(int i = 0;i < length;i++){
        int min = a[i];
        int flag = i;
        for(int j = i+1;j < length;j++){
            if(a[j] < min){
                min = a[j];
                flag = j;
            }
        }
        swap(a[i],a[flag]);
    }
    string name ="selection_sort:  ";
    showarray(name, a, length);
}

/*
归并排序：
- 基于分治法
- 基本思想：将两个或两个以上的有序序列合并为一个新的有序序列
*/



//自顶向下
void merge_arrays(int* a,int begin,int end){
    int mid = (begin + end)/2;
    int b[end];
    int flag = 0;
    int i = begin, j = mid;
    while(i < mid && j < end){
        if(a[j] <= a[i]){
            *(b + (flag++)) = *(a + (j++));
        }else{
            *(b + (flag++)) = *(a + (i++));
        }
    }
    
    for(int k = i; k < mid; k++) *(b + (flag++)) = *(a + k);
    for(int k = j; k < end; k++) *(b + (flag++)) = *(a + k);
    for(int k = 0; k < end - begin; k++) *(a + begin + k) = *(b + k);
    
    cout << begin << " " << end << endl;
    string name = "merging_sort:  ";
    showarray(name, a, 8);
}



void merging_sort(int* a,int begin,int end,int length){
    int mid = (begin + end)/2;
    if(mid <= begin){
        return;
    }
    merging_sort(a, begin, mid, length);
    merging_sort(a, mid, end, length);
    merge_arrays(a, begin, end);
}



//自底向上
//步长为1，2，4...
void merging_sort_down2up(int* a,int length){
    for(int i = 1; i < length;i=i*2){
        for(int j = 0; j < length;j = j+2*i){
            merge_arrays(a, j, min((j+2*i),length));
        }
    }
//    string name = "merging_sort:  ";
//    showarray(name, a, 8);
}


//快速排序
//平均性能最好的排序算法
int partition(int* a,int begin, int end){
    int temp = a[begin];
    int i = end - 1,j = begin;
    //注意不能等于，并且每一次循环都要判断
    while(i > j){
        while(a[i] >= temp && i > j) i--;
        swap(a[i],a[j]);
        while(a[j] <= temp && i > j) j++;
        swap(a[i],a[j]);
    }
    return i;
}

/*
快速排序: 平均性能最好的排序算法
基本思想(分治、二分法):
- 选择*控制值*，对剩下的数依次判断，将小于控制值的值放在左边，将大于控制值的值放右边
- 确定控制值的位置后，对控制值左、右两面分别进行以上操作
*/


int partition(vector<int>& a,int begin,int end){
    int temp = a[begin];
    int i = end - 1,j = begin;
    while(i > j){
        while(a[i] >= temp && i > j) i--;
        swap(a[i],a[j]);
        while(a[j] <= temp && i > j) j++;
        swap(a[i],a[j]);
    }
    return i;
}
void QuickSort(vector<int>& a,int begin,int end){
    if(begin >= end) return;
    int place = partition(a,begin,end);
    QuickSort(a,begin,place);
    QuickSort(a,place+1,end);
}


void QuickSort(int* a, int begin, int end){
    if(begin >= end) return;
    int place = partition(a,begin,end);
    QuickSort(a, begin, place);
    //又错了！此时不可以包括place,它的位置已经固定了
    QuickSort(a, place+1, end);
}

void push_interval(stack<int>& s, int begin, int end){
    s.push(end);
    s.push(begin);
}

/*
改进1:如果程序递归空间不足
优先把长序列压栈，栈的深度最多为log2n
*/

void QuickSort_stack(int* a, int begin, int end,stack<int> s){
    push_interval(s, begin, end);
    int place = 0;
    //栈空，说明初始区间已经排好序了
    while(!s.empty()){
        begin = s.top();
        s.pop();
        end = s.top();
        s.pop();
        //结束循环
        if(end <= begin) continue;
        place = partition(a, begin, end);
        if(place - begin > end - place){
            push_interval(s, begin, place);
            push_interval(s, place+1, end);
        }else{
            push_interval(s, place+1, end);
            push_interval(s, begin, place);
        }
    }
}

/*
改进2:划分基准的选择
从序列中选择三项，然后选择三项之间的中间项

改进3:对于短序列，插入排序表现更好

改进4:重复值———三路划分
*/


//heapsort
/*
 最大堆：如果树T中的任一结点的值不小于它的任一子结点的值
 （但也并非排好序的）
 仍需要第二个循环进行排序
*/

//排序以a[i]为根的树，默认除了a[i]已经是最大堆，现在要调整a[i]
void siftdown(int a[],int i,int n){
    int j;
    while((j = 2*i + 1) < n){
        if(j < n-1 && a[j+1] > a[j]) j++;
        if(a[i] < a[j]){
            swap(a[i], a[j]);
            i = j;
        }else{
            break;
        }
    }
}

void heap_sort(int a[],int n){
    //先得到最大堆
    for(int i=(n-2)/2; i>=0; i--) siftdown(a, i, n);
    for(int i = n-1; i>=0; i--){
        swap(a[i],a[0]);
        siftdown(a, 0, i);
    }
    string name = "heap_sort:  ";
    showarray(name, a, 8);
}

//新元素插入到a[k]的位置，现在需要调整
//注意siftup和siftdown的目的不同
void siftup(int a[],int k){
    int j=k, i=(j-1)/2;
    int temp = a[k];
    while((i=(j-1)/2) >= 0){
        if(a[i] >= a[k]) break;
        if(a[i] < a[k]){
            swap(a[i],a[k]);
            j = i;
        }
    }
}

bool cmp(int a,int b){
    return a>b;
}


int main(int argc, const char * argv[]) {
    int array[8] = {11,23,1,2,8,3,20,4};
    insertion_sort(array,8);
    insertion_sort_2(array,8);
    bubble_sort(array, 8);
    selection_sort(array, 8);
    merging_sort(array, 0, 8, 8);
    merging_sort_down2up(array, 8);
    QuickSort(array, 0, 8);
    QuickSort_stack(array, 0, 8, s);
    heap_sort(array,8);
}

/*
希尔排序：主要与插入排序有关
1. 在最好情况下（本身有序），时间复杂度为O(n)
2. 对于短序列，插入排序很有效

1. 先将序列转化为若干小序列，在这些*小序列*内进行*插入排序*
2. 逐渐扩大小序列的规模，而减少小序列个数，使得待排序序列逐渐处于*更有序的状态*
3. 最后对整个序列进行扫尾直接插入排序，从而完成排序
*/


/*
Most significant digit first(MSD): 分、分、分...收
Least significant digit first(LSD): 分、收、分、收...
*/

/*
 桶排序
 基本思路是：确定最值、 划分区间、分配到桶、桶内快排、最后合并、桶越多时间越少、空间越大（接近O(n)）
 
  将待排序元素划分到不同的痛。先扫描一遍序列求出最大值 maxV 和最小值 minV ，设桶的个数为 k ，则把区间 [minV, maxV] 均匀划分成 k 个区间，每个区间就是一个桶。将序列中的元素分配到各自的桶。
 对每个桶内的元素进行排序。可以选择任意一种排序算法。
  将各个桶中的元素合并成一个大的有序序列。
 假设数据是均匀分布的，则每个桶的元素平均个数为 n/k 。假设选择用快速排序对每个桶内的元素进行排序，那么每次排序的时间复杂度为 O(n/klog(n/k)) 。总的时间复杂度为 O(n)+O(m)O(n/klog(n/k)) = O(n+nlog(n/k)) = O(n+nlogn-nlogk 。当 k 接近于 n 时，桶排序的时间复杂度就可以金斯认为是 O(n) 的。即桶越多，时间效率就越高，而桶越多，空间就越大。
 ————————————————
 版权声明：本文为CSDN博主「Rnan_wang」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
 原文链接：https://blog.csdn.net/qq_19446965/article/details/81517552
*/


//基数排序：https://blog.csdn.net/xgf415/article/details/76595887
