## 字符串刷题

[TOC]

### 一些很简单但是我思维绕路的题

##### [P3741 honoka的键盘](https://www.luogu.com.cn/problem/P3741)

先把VK全部转化为XX，剩下的VV和KK就是可以增加的

```c++
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
int main()
{
	int num;
	string a;
	cin >> num >> a;
	int count = 0;
	int flag = 0;
	for (int i = 0; i < num - 1; i++) {
		if (a.substr(i, 2) == "VK") {
			count++;
			a[i] = 'X';
			a[i + 1] = 'X';
		}
	}
	for (int i = 0; i < num - 1; i++) {
		if (a.substr(i, 2) == "VV" || a.substr(i, 2) == "KK") {
			count++;
			break;
		}
	}
	cout << count;
}
```

##### [P1321 单词覆盖还原](https://www.luogu.com.cn/problem/P1321)

直接判断单词的是否可以出现即可

```c++
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
int boy,girl,n;
string a;
int main()
{
    cin>>a;
    n=a.length();
    for(int i=0;i<n-2;i++)
      boy+=(a[i]=='b'||a[i+1]=='o'||a[i+2]=='y');
    for(int i=0;i<n-3;i++)
      girl+=(a[i]=='g'||a[i+1]=='i'||a[i+2]=='r'||a[i+3]=='l');
    cout<<boy<<endl<<girl;               
    return 0;
}

```

### 8.6

##### [P5270 无论怎样神树大人都会删库跑路](https://www.luogu.com.cn/problem/P5270)

最开始的做法，维护一个长度为T的queue，而不是存储X，但是最终只有20分，超时严重。

```c++
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>

using namespace std;

const int N = 1e5;
int n, T, Q, m, mx = 0;
int S[N], R[N], M[N];
vector<int> a[N];
queue<char> qq;


int main()
{
	cin >> n >> T >> Q;
	for (int i = 0; i < T; i++) {
		int num;
		cin >> num;
		if (num > mx) {
			mx = num;
		}
		S[num]++;
	}

	
	for (int i = 1; i < n+1; i++) {
		vector<int> tmp;
		int num;
		cin >> num;
		for (int j = 0; j < num; j++) {
			int num1;
			cin >> num1;
			tmp.push_back(num1);
		}
		a[i] = tmp;
	}

	cin >> m;
	for (int i = 1; i < m+1; i++) {
		cin >> R[i];
	}


	int count = 0;
	int flag = 0;
	for (int i = 1; i < Q+1; i++) {
		int num = (i - 1) % (m + 1);
		if (num == 0) {
			Q++;
			continue;
		}
	
		vector<int> tmp = a[R[num]];
		if (tmp.size() > T) {
			while (!qq.empty()) {
				int pp = qq.front();
				M[pp]--;
				qq.pop();
			}
			for (int i = T-1; i >=0; i--) {
				qq.push(tmp[tmp.size() - 1 - i]);
				M[tmp[tmp.size() - 1 - i]]++;
			}
			for (int i = 0; i < mx; i++) {
				if (S[i] != M[i]) {
					flag = 1;
					break;
				}
			}
			if (flag == 0)	count++;
			flag = 0;
		}
		else {
			while (qq.size() > (T - tmp.size())) {
				int pp = qq.front();
				M[pp]--;
				qq.pop();
			}
			for (int i = 0; i < tmp.size();i++) {
				qq.push(tmp[i]);
				M[tmp[i]]++;
			}
			for (int i = 0; i < mx; i++) {
				if (S[i] != M[i]) {
					flag = 1;
					break;
				}
			}
			if (flag == 0)	count++;
			flag = 0;
		}
	}
	cout << count;

}

```

优化点：

1. 如何维护长度为T的后缀：双端队列
2. 如何判断长度为T的后缀与S是否匹配：对于每一组hash求和，$\sum_{i=1}^{len}hash(x)$

但是我看不懂它的答案...先欠着有缘再看一遍吧（不要做省选的题，做**普及/提高**的题！）

##### [P3375 【模板】KMP字符串匹配](https://www.luogu.com.cn/problem/P3375)

复习一下KMP算法判断大小的时候，注意-1比一般数都要大！

```c++
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const int N = 1e6;
int Next[N];



void pre(string S) {
	int i = 0, j = -1;
	Next[0] = -1;
	while (i < S.size()) {
		if (j == -1 || S[i] == S[j]) {
			i++;
			j++;
			Next[i] = j;
		}
		else j = Next[j];
	}
}

int main()
{
	string s1,s2;
	cin >> s1 >> s2;
	pre(s2);
	vector<int> ans;

	int i = 0, j = 0;
	while (i < s1.size()) {
		if (j == -1 || s1[i] == s2[j]) {
			i++;
			j++;
		}
		else {
			j = Next[j];
		}
		
		if (j != -1 && j >= s2.size()) {
			ans.push_back(i - s2.size()+1);
			j = Next[j];
		}
	}

	for (int i = 0; i < ans.size(); i++) {
		cout << ans[i] << endl;
	}

	for (int i = 1; i < s2.size() + 1; i++) {
		cout << Next[i] << " ";
	}
	return 0;
}

```

##### [P4391 [BOI2009]Radio Transmission 无线传输](https://www.luogu.com.cn/problem/P4391)

给你一个字符串 s_1*s*1，它是由某个字符串 s_2*s*2 不断自我连接形成的。但是字符串 s_2*s*2 是不确定的，现在只想知道它的最短长度是多少。

88分，1个测试点没过，大致思路就是两个指针i和j，i进行探索，j进行匹配

```c++
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int MAXN = 1e6;
int mn = MAXN;

int main()
{
	int num = 0;
	string a;
	cin >> num >> a;
	int i = 0, j = -1;
	int start = MAXN;
	int flag = 0;

	while (i < num) {
		if (j == -1 || a[i] == a[j]) {
			if (j != -1 && flag == 0) {
				start = i;
				flag = 1;
			}
			i++;
			j++;
			if (j == start) mn = i - start;
		}
		else {
			flag = 0;
			mn = MAXN;
			start = MAXN;
			if (j != 0)	i--;
			j = -1;
		}
	}
	if (start == MAXN) cout << a.size();
	else if (mn == MAXN) cout << start;
	else cout << mn;
	return 0;
}

```

- 注意**特殊情况**！如果string的长度只有1呢，不要为这种低级错误扣分。

- 如果是aaaaaab，返回7

- 以下情况不合适

  13
  aaaabaaaaaab

  答案应该是7，而不是12

原来的思路好像不能保证这种情况，因为原来的思路总是从头匹配，如果最长的字符串在后面，则无法匹配成功。

答案其实是：==n-Next[n]==，很巧妙。

<img src="/Users/apple/Library/Application Support/typora-user-images/image-20200807155016607.png" alt="image-20200807155016607" style="zoom:50%;" />

```c++
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const int MAXN = 1e6+1;
int Next[MAXN];

void pre(string a) {
	int i = 0, j = -1;
	Next[0] = -1;
	while (i < a.size()) {
		if (j == -1 || a[i] == a[j]) {
			i++;
			j++;
			//每一步都要记录
			Next[i] = j;
		}
		else j = Next[j];
	}
}

int main()
{
	int num = 0;
	string a;
	cin >> num >> a;
	pre(a);

	cout << a.size() - Next[a.size()];

	return 0;
}

```

### 8.7

##### [判断两个字符串的最长公共子串]()

用动态规划来做，用二维的dp数组，如果a[i]==b[j]，则dp\[i]\[j] = dp[i-1]\[j-1]+1

##### [P3805 【模板】manacher算法](https://www.luogu.com.cn/problem/P3805)

首先尝试暴力解法，时间复杂度n^2，在leetcode上通过成功（数据规模最长1000）

[\5. Longest Palindromic Substring](https://leetcode.com/problems/longest-palindromic-substring/)

```c++
class Solution {
public:
    string longestPalindrome(string s) {
        int mx = 0;
        int pos = 0;
        int flag = -1;
        if (s.size() == 0 || s.size() == 1) {
            return s;
        }
        for (int i = 0; i < s.size(); i++) {
            //odd
            for (int j = 0; i - j >= 0 && i + j < s.size(); j++) {
                if (s[i - j] == s[i + j]) {
                    int tmp = j * 2 + 1;
                    if (tmp > mx) {
                        flag = 0;
                        pos = i;
                        mx = tmp;
                    }
                }else break;
            }

            //even
            for (int j = 1; i - j + 1 >= 0 && i + j < s.size(); j++) {
                if (s[i - j + 1] == s[i + j]) {
                    int tmp = j * 2;
                    if (tmp > mx) {
                        flag = 1;
                        pos = i;
                        mx = tmp;
                    }
                }else break;
            }
        }
        
        if(flag == 0){
            return s.substr(pos-(mx-1)/2,mx);
        }else{
            return s.substr(pos-mx/2+1,mx);
        }
      
        return 0;
    }
};
```

在洛谷上可以得64分。

```c++
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;



int main()
{
	string s = "";
	cin >> s;
	
	int mx = 0;
	int pos = 0;
	int flag = -1;
	if (s.size() == 0 || s.size() == 1) {
		cout << s.size();
		return 0;
	}
	for (int i = 0; i < s.size(); i++) {
		//odd
		for (int j = 0; i - j >= 0 && i + j < s.size(); j++) {
			if (s[i - j] == s[i + j]) {
				int tmp = j * 2 + 1;
				if (tmp > mx) {
					flag = 0;
					pos = i;
					mx = tmp;
				}
			}
			else break;
		}

		//even
		for (int j = 1; i - j + 1 >= 0 && i + j < s.size(); j++) {
			if (s[i - j + 1] == s[i + j]) {
				int tmp = j * 2;
				if (tmp > mx) {
					flag = 1;
					pos = i;
					mx = tmp;
				}
			}
			else break;
		}
	}

	cout << mx;
	return 0;
}

```

引入[manachar算法](./string_note)

```c++
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 1.1e7;
int pos, maxr, len, ans;
int p[2*MAXN+2];
char s_new[2*MAXN+2];

void init(string s) {
	s_new[len++] = '$';
	s_new[len++] = '#';
	for (int i = 0; i < s.size(); i++) {
		s_new[len++] = s[i];
		s_new[len++] = '#';
	}
}


int main()
{
	string s = "";
	cin >> s;
	init(s);
	for (int i = 1; i < len; i++) {
		if (i < maxr)	p[i] = min(p[2 * pos - i], maxr - i);
		else p[i] = 1;

		while (s_new[i - p[i]] == s_new[i + p[i]]) p[i]++;

		if (i + p[i] > maxr) {
			pos = i;
			maxr = i + p[i];
		}

		if (p[i] - 1 > ans)	ans = p[i] - 1;
	}


	cout << ans;
	
	return 0;
}

```

### 8.8

##### [P3879 [TJOI2010]阅读理解](https://www.luogu.com.cn/problem/P3879)

本来是为了学习Trie树的，但是用unordered_set就一遍AC了

```c++
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;

vector<vector<string> > V;
vector<string> words;
vector<unordered_set<string> > W;
vector<vector<int> > ans;

int main()
{

	int num;
	cin >> num;
	for (int i = 0; i < num; i++) {
		vector<string> S;
		int l;
		cin >> l;
		for (int j = 0; j < l; j++) {
			string tmp;
			cin >> tmp;
			S.push_back(tmp);
		}
		V.push_back(S);
	}

	cin >> num;
	for (int i = 0; i < num; i++) {
		string tmp;
		cin >> tmp;
		words.push_back(tmp);
	}

	for (int i = 0; i < V.size(); i++) {
		unordered_set<string> tmp(V[i].begin(), V[i].end());
		W.push_back(tmp);
	}

	for (int i = 0; i < words.size(); i++) {
		vector<int> tmp;
		for (int j = 0; j < W.size(); j++) {
			if (W[j].count(words[i])) {
				tmp.push_back(j+1);
			}
		}
		ans.push_back(tmp);
	}

	for (int i = 0; i < ans.size(); i++) {
		for (int j = 0; j < ans[i].size(); j++) {
			cout << ans[i][j] << " ";
		}
		cout << endl;
	}
	
	return 0;
}

```

但还是看看[Trie树](string_note)是什么吧。

如果**给定n篇文章和m个询问，问每个询问在多少篇文章中出现**，则用map或者画unordered_set都很快。

但是如果是**给定n个单词和m组询问，问每个询问是多少个单词的前缀**，用map就会TLE，因此需要Trie树。

但是以下下午AC的代码..晚上再提交却出现了不少MLE...太玄了。

```c++
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;

int N, M;
int nex[500010][26];
int b[500010][1010];
int cnt;

void insert(int x) {
	string s;
	cin >> s;
	int now = 0;
	for (int i = 0; i < s.size(); i++) {
		if (!nex[now][s[i] - 'a']) {
			nex[now][s[i] - 'a'] = ++cnt;
		}
		now = nex[now][s[i] - 'a'];
	}
	b[now][x] = 1;
}

void check() {
	string s;
	cin >> s;
	int now = 0;
	int flag = 1;
	for (int i = 0; i < s.size(); i++) {
		if (!nex[now][s[i] - 'a']) {
			flag = 0;
			break;
		}
		now = nex[now][s[i] - 'a'];
	}
	if (flag) {
		for (int i = 0; i < N; i++) {
			if (b[now][i])	cout << i + 1 << " ";
		}
	}
	cout << endl;
}

int main()
{
	cin >> N;
	for (int i = 0; i < N; i++) {
		int L;
		cin >> L;
		for (int j = 0; j < L; j++) {
			insert(i);
		}
	}

	cin >> M;
	for (int i = 0; i < M; i++) {
		check();
	}

	return 0;
}
```

用struct的方式重新组织一下，但是全部MLE，所以struct中不能开太大的数组..

```c++
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;

int N, M;

const int MAXN = 500000;
const int MAXN1 = 1e3;
int cnt;

struct luogu {
	int nex[26];
	int b[MAXN1];
}trie[MAXN];


void insert(int x) {
	string s;
	cin >> s;
	int now = 0;
	for (int i = 0; i < s.size(); i++) {
		if (!trie[now].nex[s[i] - 'a']) {
			trie[now].nex[s[i] - 'a'] = ++cnt;
		}
		now = trie[now].nex[s[i] - 'a'];
	}
	trie[now].b[x] = 1;
}

void check() {
	string s;
	cin >> s;
	int now = 0;
	int flag = 1;
	for (int i = 0; i < s.size(); i++) {
		if (!trie[now].nex[s[i] - 'a']) {
			flag = 0;
			break;
		}
		now = trie[now].nex[s[i] - 'a'];
	}
	if (flag) {
		for (int i = 0; i < N; i++) {
			if (trie[now].b[i])	cout << i + 1 << " ";
		}
	}
	cout << endl;
}

int main()
{
	cin >> N;
	
	for (int i = 0; i < N; i++) {
		int L;
		cin >> L;
		for (int j = 0; j < L; j++) {
			insert(i);
		}
	}

	cin >> M;
	for (int i = 0; i < M; i++) {
		check();
	}

	return 0;
}


```



##### [P3808 【模板】AC自动机（简单版）](https://www.luogu.com.cn/problem/P3808)

给定 n*n* 个模式串 s_i*s**i* 和一个文本串 t*t*，求有多少个不同的模式串在文本串里出现过。

总是想看看第一时间想到的方法能拿多少分，多次调用kmp，总共n^2复杂度，得了50分，有一个极大的数据集没有过

```c++
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;

int N;
string t;
const int MAXN = 1e6;
int Next[MAXN];
int cnt;

void pre(string s) {
	int i = 0, j = -1;
	Next[0] = -1;
	while (i < s.size() && j < s.size()) {
		if (j == -1 || s[i] == s[j]) {
			i++;
			j++;
			Next[i] = j;
		}
		else j = Next[j];
	}
}

void kmp(string s) {
	int i = 0, j = 0;
	while (i < t.size() && j < s.size()) {
		if (j == -1 || t[i] == s[j]) {
			i++;
			j++;
		}
		else j = Next[j];
	}
	if (j !=-1 && j >= s.size())	cnt++;
}


int main()
{
	vector<string> S;

	cin >> N;
	for (int i = 0; i < N; i++) {
		string tmp;
		cin >> tmp;
		S.push_back(tmp);
	}
	cin >> t;
	for (int i = 0; i < N; i++) {
		pre(S[i]);
		kmp(S[i]);
	}
	cout << cnt;

	return 0;
}

```

利用[AC自动机](string_note)，几个值得注意的点：

- 记得利用Queue实现BFS
- 记得在ac()中每次数完需要将cnt置为-1

```c++
#include <iostream>
#include <string>
#include <vector>
#include <queue>


using namespace std;

const int MAXN = 500010;
struct luogu {
	int nex[26];
	int cnt, fail;
}trie[MAXN];


int n, code, ans;
string t;

void insert() {
	string s;
	cin >> s;
	int now = 0;
	for (int i = 0; i < s.size(); i++) {
		int p = s[i] - 'a';
		if (!trie[now].nex[p])  trie[now].nex[p] = ++code;
		now = trie[now].nex[p];
	}
	
	trie[now].cnt++;
}

void makefail() {
	queue<int> Q;
	for (int i = 0; i < 26; i++) {
		int son = trie[0].nex[i];
		if (son) {
			trie[son].fail = 0;
			Q.push(son);
		}
	}

	while (!Q.empty()) {
		int tmp = Q.front();
		Q.pop();
		for (int i = 0; i < 26; i++) {
			int son = trie[tmp].nex[i];
			int ff = trie[tmp].fail;
			if (son) {
				trie[son].fail = trie[ff].nex[i];
				Q.push(son);
			}
			else {
				trie[tmp].nex[i] = trie[ff].nex[i];
			}
		}
	}
}


void ac() {
	int now = 0;
	for (int i = 0; i < t.size(); i++) {
		int son = trie[now].nex[t[i] - 'a'];
		while (son && trie[son].cnt != -1) {
			ans += trie[son].cnt;
			trie[son].cnt = -1;
			son = trie[son].fail;
		}
		now = trie[now].nex[t[i] - 'a'];
	}
	cout << ans;
}

int main()
{
	cin >> n;
	for (int i = 0; i < n; i++) {
		insert();
	}
	makefail();
	
	cin >> t;
	ac();
	
	return 0;
}
```

### 霍夫曼编码

```c++
#include   <iostream>
#include <vector>
using namespace std;

const int MAXN = 1000;

typedef struct node {
    char data;
    string code;
    struct node* lchild;
    struct node* rchild;
}NODE;

NODE* create_huffman_tree(vector<char> a, vector<int> w, int n) {
    NODE* addr[MAXN];
    int n1, n2, i, j;
    int u, v, min1, min2;
    for (i = 0; i < n; i++) {
        addr[i] = new NODE();
        addr[i]->data = a[i];
        addr[i]->lchild = NULL;
        addr[i]->rchild = NULL;
        w[i] = -w[i];
    }
    for (i = n; i < 2 * n - 1; i++) {
        n1 = -1;
        min1 = 9999;
        n2 = -1;
        min2 = 9999;
        for (j = 0; j < i; j++)
        {
            v = w[j];
            u = -v;
            if (u > 0)
                if (u < min1) {
                    min2 = min1;
                    n2 = n1;
                    min1 = u;
                    n1 = j;
                }
                else if (u < min2) {
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
    w[2 * n - 2] = -w[2 * n - 2];
    return addr[2 * n - 2];
}

void coding(NODE* t) {
    if (t->lchild != nullptr) {
        t->lchild->code = t->code + "0";
        coding(t->lchild);
    }
    if (t->rchild != nullptr) {
        t->rchild->code = t->code + "1";
        coding(t->rchild);
    }
}


void preorder(NODE* t) {
    if (t != NULL) {
        if (t->data != '*') cout << t->data << ":" << t->code << endl;
        preorder(t->lchild);
        preorder(t->rchild);
    }
}


int main() {
    string temp;
    vector<char> a;
    vector<int> w;
    cout << "please input your source text:";
    cin >> temp;
    vector<char>::iterator iter;
    for (int i = 0; i < temp.size(); i++) {
        iter = find(a.begin(), a.end(), temp[i]);
        if (iter == a.end()) {
            a.push_back(temp[i]);
            w.push_back(1);
        }
        else {
            long int i = iter - a.begin();
            w[i]++;
        }
    }
    for (int i = a.size(); i < (2 * a.size() - 1); i++) {
        w.push_back(0);
    }
    NODE* ans;
    ans = create_huffman_tree(a, w, a.size());
    coding(ans);
    preorder(ans);
}

```

