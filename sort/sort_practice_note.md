## 排序

### 2.24 [Sort Colors](https://leetcode.com/problems/sort-colors/)

用快排的思想，但是比快排还要简单

```c++
class Solution {
public:
    void sortColors(vector<int>& nums){
        int left = 0, i = 0;
        int right = nums.size() - 1;
        //和快排想法相似，让left左边和right右边已经是最小以及最大了
        while( i <= right){
            if(nums[i] == 0){
                swap(nums[i++],nums[left++]);
            }else if(nums[i] == 2){
                //但是注意此时i不能前进，因为还需要检查换进来的是否为0
                swap(nums[i],nums[right--]);
            }else if(nums[i] == 1){
                i++;
            }
        }
    }
    
};
```

### 2.25 [Sort List](https://leetcode.com/problems/sort-list/)

对链表进行排序，只要涉及到链表：

- 如果跟中点有关：快慢指针
- 归并时非常简单，像穿针引线一样

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
    ListNode* sortList(ListNode* head) {
        //首先，对于链表来说，不要用索引，如果是跟中点有关，用快慢指针
        if(!head || !head->next) return head;
        ListNode *slow, *fast, *pre;
        slow = head;
        fast = head;
        while(fast && fast->next){
            pre = slow;
            slow = slow->next;
            fast = fast->next->next;
        }
        //其次，既然没有索引，每次都传指针即可
        //第三，要保证递归，需要把链表劈成两半，所以采用pre指针，所以现在merge的是两条链表
        pre->next = NULL;
        return merge(sortList(head),sortList(slow));
        
    }
    ListNode* merge(ListNode* l1, ListNode* l2) {
        //第四，为什么要用链表，是因为归并时非常简单
        //我太傻了，我想着常数空间复杂度，就要在原链表上更改，但是通过新指针就可重新改变顺序，像穿针引线一样
        //对于链表来说，需要改变大量顺序的操作非常浪费时间且易错，一定不要选择
        ListNode *temp,*curr;
        //可以先指向一个其他地方
        temp = new ListNode(-1);
        curr = temp;
        while(l1 && l2){
            if(l1->val < l2->val){
                curr->next = l1;
                l1 = l1->next;
            }else{
                curr->next = l2;
                l2 = l2->next;
            }
            curr = curr->next;
        }
        if(l1) curr->next = l1;
        if(l2) curr->next = l2;
        return temp->next;
    }
};
```

### 3.2 [Insertion Sort List](https://leetcode.com/problems/insertion-sort-list/)

对链表进行插入排序

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
    ListNode* insertionSortList(ListNode* head) {
        
        if(!head || head->next == NULL) return head;
        
        ListNode *mark;
        int min = head->val;
        for(mark = head->next;mark != NULL;mark = mark->next){
            if(mark->val < min) min = mark->val;
        }
        
        //回忆起fake node的好处了
        ListNode *temp = new ListNode(min);
        //循环插入一般需要四个指针，记住不要把q和pr搞混了
        ListNode *p,*q,*pr,*r;
        int flag;
        
        temp->next = head;
        head = temp;
        
        pr = head->next;
        r = head->next->next;
        while(r){
            flag = 0;
            p = head;
            q = head->next;
            while(q!=r){
                if(r->val >= p->val && r->val <= q->val){
                    pr->next = r->next;
                    r->next = q;
                    p->next = r;
                    r = pr;
                    flag = 1;
                    break;
                }else{
                    p = p->next;
                    q = q->next;
                }
            }
            // for(test = head; test != NULL; test = test->next)
            //     cout << test->val;
            // cout << endl;
            if(!flag) pr = pr->next;
            r = r->next;
        }
        return head->next;
        
    }
};
```



### 2.26 [Wiggle Sort II](https://leetcode.com/problems/wiggle-sort-ii/)

通过排序使得每个元素都比旁边两个要大

先排序，然后重新组合

```c++
class Solution {
public:
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
    
    void wiggleSort(vector<int>& nums) {
        QuickSort(nums, 0, nums.size());
        //最初想法是好的，但是末尾和begin可能会一样，因为相隔不够远
        //所以总挑最后一个
        int size = nums.size();
        int begin = (size + 1)/2;
        int a[size];
        // for(int i=0,count=0;count<begin;i=i+2,count=count+1){
        //     a[i] = nums[count];
        //     if(i!=size-1) a[i+1] = nums[begin+count];
        // }
        for(int p=begin-1,q=size-1,i=0;p >= 0;p--,q--,i=i+2){
            a[i] = nums[p];
            if(i!=size-1) a[i+1] = nums[q];
        }
        for(int i=0;i<size;i++){
            nums[i] = a[i];
        }
    }
};
```

### 2.27 [Largest Number](https://leetcode.com/problems/largest-number/)

用一个包含数字的array组合成最大的数

不用一位一位的判断，直接判断是把a放前面大还是把b放前面大，学会这种==对sort重定义的方式==。

```c++
class Solution {
public:
    //不用一位一位的判断，直接判断是把a放前面大还是把b放前面大
    //string可以直接对比大小（wow），而不用计算位数
    //掌握快排以后直接用sort吧
    string largestNumber(vector<int>& nums) {
        string temp = "";
        sort(nums.begin(),nums.end(), [](int a,int b){
            return to_string(a) + to_string(b) > to_string(b) + to_string(a);
        });
        //学会这种遍历方式！
        for(auto num : nums){
            temp += to_string(num);
        }
        //注意判断特殊情况 00000
        return temp[0] == '0'? "0":temp;
    }
};
```

### 2.28 [Sort Characters By Frequency](https://leetcode.com/problems/sort-characters-by-frequency/)

```c++
class Solution {
public:
    string frequencySort(string s) {
        //unordered_map适用于查找，map适用于排序
        unordered_map<char,int> hmap;
        //map不用判断是否在不在，初始化为0，不同于python中的dict
        //string也属于STL，可以直接用sort排序
        for(auto ch : s) hmap[ch]++;
        //关键！原来sort加个&就可以全局变量（小s脸）
        sort(s.begin(),s.end(),[&](char& a,char& b){
            return hmap[a] > hmap[b] || (hmap[a] == hmap[b] && a < b);
        });
        return s;
    }
};
```

#### 215. [Kth Largest Element in an Array](https://leetcode.com/problems/kth-largest-element-in-an-array/)

找到数组中第k大的元素

用快排的方法，主要还是二分法的想法

```c++
class Solution {
public:
    int partition(vector<int>& nums, int begin, int end){
        int i = end - 1,j = begin;
        int temp = nums[begin];
        while( i > j){
            while(nums[i] <= temp && i > j) i--;
            swap(nums[i],nums[j]);
            while(nums[j] >= temp && i > j) j++;
            swap(nums[i],nums[j]);
        }
        return i;
    }
    void QuickSort(vector<int>& nums, int begin, int end, int k, int& ans){
        if(begin >= end) return;
        int place = partition(nums,begin,end);
        if(place == k-1){
            ans = nums[place];
            return;
        }
        else if(place > k-1) end = place;
        else begin = place+1;
        QuickSort(nums,begin,end,k,ans);
    }
    int findKthLargest(vector<int>& nums, int k) {
        //快排很棒的一点就是，它会直接找出这个数字应该处的位子，涉及到寻找第k个都可以选它
        int ans = 0;
        QuickSort(nums,0,nums.size(),k,ans);
        return ans;
    }
};
```

#### 216. 41. [First Missing Positive](https://leetcode.com/problems/first-missing-positive/)

找缺失的最小正数

```c++
class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        //想法是好的，但是需要考虑大数
        // int max = 0;
        // for(auto num : nums)    if(num > max) max = num;
        // int a[max+1];
        // for(auto num : nums)    if(num > 0) a[num] = 1;
        // for(int i = 1;i < max+1;i++){
        //     if(a[i] != 1) return i;
        // }
        // return max+1;
        
        //首先，需要明确的是，缺失的数不可能大于nums.size()
        //所以，将每个数放在它应该呆的位子上（如果超过nums.size()就抛弃）
        if(nums.size() == 0) return 1;
        int a[nums.size()] = {0};
        for(auto num : nums){
            if(num > 0 && num < nums.size() + 1) a[num-1] = 1;
        }
        for(int i = 0;i < nums.size();i++){
            if(a[i] != 1) return (i+1);
        }
        return (nums.size() + 1);
    }
};
```

### 2.29

#### \88. [Merge Sorted Array](https://leetcode.com/problems/merge-sorted-array/)

```c++
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        vector<int> ans;
        int i = 0,j = 0;
        while(i < m && j < n){
            if(nums1[i] <= nums2[j]){
                ans.push_back(nums1[i]);
                i++;
            }else{
                ans.push_back(nums2[j]);
                j++;
            }
        }
        while(i < m) ans.push_back(nums1[i++]);
        while(j < n) ans.push_back(nums2[j++]);
        for(int i = 0;i < nums1.size() && i < ans.size();i++){
            nums1[i] = ans[i];
        }
    }
};
```

```c++
        //第二种方法：不用多余的数组空间，从后往前
        int i = m-1, j = n-1, k = m+n-1;
        while(i >= 0 && j >= 0){
            if(nums1[i] >= nums2[j])    nums1[k--] = nums1[i--];
            else nums1[k--] = nums2[j--];
        }
        while(j >= 0) nums1[k--] = nums2[j--];
```

#### 89. 23 Merge k Sorted Lists

只要涉及到多个+合并，就用归并的方法

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
//     int find_max(vector<ListNode*>& nodes){
//         //被一个感叹号坑晕了，一定要注意细节
//         //但是这个的无疑是比较笨的方法，时间复杂度n^3
//         // for(int i=0; i<nodes.size(); i++){
//         //     if(nodes[i])    cout << nodes[i]->val;
//         // }
//         // cout << "..." << endl;
        
//         if(!nodes.size()) return NULL;
//         int min;
//         int minnode = -1;
//         for(int i=0; i<nodes.size(); i++){
//             if(minnode == -1 && nodes[i]){
//                 min = nodes[i]->val;
//                 minnode = i;
//             }
//             if(nodes[i] && nodes[i]->val < min){
//                 min = nodes[i]->val;
//                 minnode = i;
//             }
//         }
//         return minnode;
//     }
    
  
//    ListNode* mergeKLists(vector<ListNode*>& lists) {
//         ListNode *begin,*temp;
//         begin = new ListNode(-1);
//         temp = begin;
        
//         int minnode = 0;
//         int nullcount = 0;
//         for(int i=0 ;i<lists.size(); i++){
//             if(!lists[i])   nullcount++;
//         }
        
//         while(nullcount != lists.size()){
//             minnode = find_max(lists);
//             // cout << nullcount;
//             // cout << minnode;
//             // cout << lists[minnode]->val << endl;
//             temp->next = lists[minnode];
//             temp = temp->next;
//             lists[minnode] = lists[minnode]->next;
//             if(lists[minnode] == NULL) nullcount++;
//         }
//         return begin->next;
//    }
  
  
    //合并两个有序列表
    ListNode* merge(ListNode* l1, ListNode* l2) {
        ListNode *temp,*curr;
        temp = new ListNode(-1);
        curr = temp;
        while(l1 && l2){
            if(l1->val < l2->val){
                curr->next = l1;
                l1 = l1->next;
            }else{
                curr->next = l2;
                l2 = l2->next;
            }
            curr = curr->next;
        }
        if(l1) curr->next = l1;
        if(l2) curr->next = l2;
        return temp->next;
    }
    
    //很容易就想到归并
    ListNode* merging_sort(vector<ListNode*>& lists, int begin, int end){
        if(begin >= end) return lists[begin];
        int mid = (begin+end)/2;
        ListNode *beginnode,*midnode;
        beginnode = merging_sort(lists,begin,mid);
        midnode = merging_sort(lists,mid+1,end);
        return merge(beginnode,midnode);
    }
    
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if(!lists.size()) return NULL;
        return merging_sort(lists,0,lists.size()-1);
    }
};
```

### 3.4 35. [Search Insert Position](https://leetcode.com/problems/search-insert-position/)

复习二分法

```c++
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        //复习一下二分法
        int size = nums.size();
        int begin = 0, end = size-1;
        int mid = 0;
        //这样写不对称，很不爽
        // while(begin < end){
        //     mid = (begin + end)/2;
        //     if(nums[mid] < target){
        //         begin = mid + 1;
        //     }else if(nums[mid] > target){
        //         end = mid - 1;
        //     }else{
        //         return mid;
        //     }
        // }
        // if(target > nums[begin]) return begin+1;
        // else return begin;
        
        while(begin <= end){
            mid = (begin + end)/2;
            if(nums[mid] < target){
                begin = mid + 1;
            }else if(nums[mid] > target){
                end = mid - 1;
            }else{
                return mid;
            }
        }
        return begin;
    }
};
```

### 3.5 \74. [Search a 2D Matrix](https://leetcode.com/problems/search-a-2d-matrix/)

对一个矩阵进行搜索

难道...不是把它转化为一个vector就好了吗

```c++
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        //复习一下二分法
        if(nums.size() == 0) return 0;
        int size = nums.size();
        int begin = 0, end = size-1;
        int mid = 0;
        
        while(begin <= end){
            mid = (begin + end)/2;
            if(nums[mid] < target){
                begin = mid + 1;
            }else if(nums[mid] > target){
                end = mid - 1;
            }else{
                return mid;
            }
        }
        return begin;
    }
    
    bool findtarget(vector<int>& nums, int target) {
        if(nums.size() == 0) return false;
        //复习一下二分法
        int size = nums.size();
        int begin = 0, end = size-1;
        int mid = 0;
        
        while(begin <= end){
            mid = (begin + end)/2;
            if(nums[mid] < target){
                begin = mid + 1;
            }else if(nums[mid] > target){
                end = mid - 1;
            }else{
                return true;
            }
        }
        return false;
    }
    
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        
        if(matrix.size() == 0) return false;
        
        int size = matrix.size();
        int p,test;
        vector<int> num;
        for(int i=0; i<size; i++){
            if(matrix[i].size()){
                num.push_back(matrix[i][0]);
            }
        }
        
        //这个地方易错，要考虑三种情况：
        //如果是0，那么有可能刚好是0位置的数(true)，或者比0位置小（false）
        //如果是(0,size)的数，那么有可能刚好是该位置的数(true)，或者比该位置小（p-1）
        //如果是size，那么一定是最后一行的数（p-1）
        //不要一直试，想清楚
        
        p = searchInsert(num,target);
        if(p!=size && matrix[p].size() && matrix[p][0] == target) return true;
        if(p && p <= size){
            test = findtarget(matrix[p-1],target);
            return test;
        }
        return false;
    }
};
```



## 其他

- 遍历时不采用局部变量，从而在break的时候，以小旗帜的形式标记需要改动的地方。有时候会默认循环遍历到最后一个的特殊情况，一劳永逸。

- c++查看数据类型

  ```c++
  #include <typeinfo>
  typeid(a).name()
  ```

- map vs unordered_map

  - map： map内部实现了一个==红黑树==，该结构具有自动排序的功能，因此map内部的所有元素都是有序的，红黑树的每一个节点都代表着map的一个元素，因此，对于map进行的查找，删除，添加等一系列的操作都相当于是对红黑树进行这样的操作，故红黑树的效率决定了map的效率。
    - 优点
      - **有序性**，这是map结构最大的优点，其元素的有序性在很多应用中都会简化很多的操作
      - **红黑树**，内部实现一个红黑书使得map的很多操作在*[Math Processing Error]lgn*的时间复杂度下就可以实现，因此效率非常的高
    - 缺点：
      - **空间占用率高**，因为map内部实现了红黑树，虽然提高了运行效率，但是因为每一个节点都需要额外保存父节点，孩子节点以及红/黑性质，使得每一个节点都占用大量的空间
    - 适用处，对于那些有顺序要求的问题，用map会更高效一些
  - unordered_map: unordered_map内部实现了一个==哈希表==，因此其元素的排列顺序是杂乱的，无序的
    - 优点：
      - 查找快，因为内部实现了哈希表，因此其查找速度非常的快
    - 缺点：
      - 建立费时间，哈希表的建立比较耗费时间
    - 适用处，对于**查找问题**，unordered_map会更加高效一些，因此遇到查找问题，常会考虑一下用unordered_map

