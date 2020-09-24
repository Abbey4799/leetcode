

## 贪心

##### [P1803 凌乱的yyy / 线段覆盖](https://www.luogu.com.cn/problem/P1803)

选择不相等的区间问题。

###### 贪心做法

我最开始想的贪心的指标有：开始时间、持续时间、重复时间条数，唯独没有想到结束时间...总是选择**结束时间最早的记录**。

可以用反证法，当我选择结束时间最早的记录（记录A）时，记录A不适合的唯一情况为，它会跟两个比赛重叠。但是如果它和两个比赛重叠，则一定存在一个比赛的结束时间比它早，所以记录A不会被选择。因此，只要结束时间最早的记录，一定是最优的。

```c++
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <iomanip>
#include <queue>

using namespace std;

struct LUOGU {
	int bg, ed;
	LUOGU(int a, int b) :bg(a), ed(b) {};
	bool operator < (const LUOGU a) const{
		return ed > a.ed;
	}
};

int n, cnt, endnow;
priority_queue<LUOGU> qq;


int main()
{
	cin >> n;
	for (int i = 0; i < n; i++) {
		int u, v;
		cin >> u >> v;
		qq.push(LUOGU(u, v));
	}

	while (!qq.empty()) {
		LUOGU tmp = qq.top();
		qq.pop();
		if (endnow <= tmp.bg) {
			cnt++;
			endnow = tmp.ed;
		}
	}

	cout << cnt;

	return 0;

}

```

> 贪心还需要多练，可能需要从反证法的角度思考问题

###### 动规做法

用f[i]表示前i场比赛最多可以参与的比赛数

状态转移方程：f[i] = max( f[i-1], f[temp] + 1)

其中temp是一个递增的变量，代表加入第i场比赛后，上一个available的比赛的index

```c++
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <math.h>
#include <iomanip>

using namespace std;

struct LUOGU {
	int bg, ed;
	LUOGU(int a, int b) :bg(a), ed(b) {};
	bool operator < (const LUOGU a) const {
		if (ed != a.ed) return ed < a.ed;
		return bg < a.bg;
	}
};


const int MAXN = 1e6 + 5;
int n, mark;
vector<LUOGU> save;
int f[MAXN];

int main()
{
	cin >> n;
	save.push_back(LUOGU(0, 0));

	for (int i = 0; i < n; i++) {
		int u, v;
		cin >> u >> v;
		save.push_back(LUOGU(u, v));
	}

	sort(save.begin(),save.end());

	for (int i = 1; i < save.size(); i++) {
    // Attention：注意是while
    // Attention：注意和mark+1相比
		while (save[i].bg >= save[mark + 1].ed) {
			mark++;
		}
		f[i] = max(f[i - 1], f[mark] + 1);
	}

	cout << f[n];
	return 0;

}

```

##### [P2240 【深基12.例1】部分背包问题](https://www.luogu.com.cn/problem/P2240)

经典背包问题。N堆金币，每堆有总重量以及总价值，规定了背包总承重T，求最多可以拿走的金币？

金币可以任意分割，单位价格不变

```c++
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <math.h>
#include <queue>
#include <iomanip>

using namespace std;

double N, T;
double M;
double ans;
struct LUOGU {
	double m, v;
	double p;
	LUOGU(double a, double b, double c) :m(a), v(b), p(c) {};
	bool operator < (const LUOGU a) const {
		return p < a.p;
	}
};

priority_queue<LUOGU> qq;

int main()
{
	cin >> N >> T;
	for (int i = 0; i < N; i++) {
		double u, v;
		cin >> u >> v;
		double p = v / u;
		qq.push(LUOGU(u, v, p));
	}


	while (!qq.empty()) {
		LUOGU temp = qq.top();
		qq.pop();

		if (M + temp.m > T) {
			ans += temp.p * (T - M);
			break;
		}
		else {
			M += temp.m;
			ans += temp.v;
		}
	}


	cout << setiosflags(ios::fixed) << setprecision(2);
	cout << ans;

	return 0;

}

```

又是被double小小卡住的一天呢😅

## 动规

##### [P1048 采药](https://www.luogu.com.cn/problem/P1048)

记住了，==0-1背包问题==，很容易想到：对于物品i，能做的选择就是 放与不放。那如何体现放与不放呢：

- 维护**dp\[i]\[j]**代表：以j为容量放入前i个物品的最大价值。因为总是需要j来记录现在的时间容量是否足以加入一个新的物品。
- 状态转移方程：dp\[i][j] = max(  **放**  dp\[i-1][j - w[i]], **不放**  dp\[i-1][j])

```c++
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <math.h>
#include <queue>
#include <iomanip>

using namespace std;


struct LUOGU {
	int t, v;
	LUOGU(int a, int b) :t(a), v(b) {}
};

const int MAXNM = 105;
const int MAXNT = 1005;
vector<LUOGU> Dict(MAXNM, LUOGU(0, 0));
int T, M;
int dp[MAXNM][MAXNT];


int main()
{
	cin >> T >> M;

	for (int i = 1; i <= M; i++) {
		int u, v;
		cin >> u >> v;
		Dict[i] = LUOGU(u, v);
	}

	for (int i = 1; i <= M; i++) {
		for (int j = 1; j <= T; j++) {
			LUOGU temp = Dict[i];
			if (j - temp.t < 0) {
				dp[i][j] = dp[i - 1][j];
			}
			else {
				dp[i][j] = max(dp[i - 1][j - temp.t] + temp.v, dp[i - 1][j]);
			}
		}
	}

	cout << dp[M][T];
	return 0;

}

```

> dp题 注意数组维度，可能两个维度的MAXN值不相同😅

注意如果把dp转化为一维数组需要倒着更新，否则会覆盖

##### [P1616 疯狂的采药](https://www.luogu.com.cn/problem/P1616)

==完全背包==问题。现在每种草药可以无限制的采摘。

最初的版本90分，WA了一个数据点：

```c++
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <math.h>
#include <queue>
#include <iomanip>

using namespace std;


struct LUOGU {
	int t, v;
	LUOGU(int a, int b) :t(a), v(b) {}
};

const int MAXNM = 1e4 + 5;
const int MAXNT = 1e7 + 5;
vector<LUOGU> Dict(MAXNM, LUOGU(0, 0));
int T, M;
int dp[MAXNT];


int main()
{
	cin >> T >> M;

	for (int i = 1; i <= M; i++) {
		int u, v;
		cin >> u >> v;
		Dict[i] = LUOGU(u, v);
	}

	for (int i = 1; i <= M; i++) {
		for (int j = 1; j <= T ; j++) {
			LUOGU temp = Dict[i];
			if (j - temp.t >= 0){
				dp[j] = max(dp[j - temp.t] + temp.v, dp[j]);
			}
		}
	}

	cout << dp[T];
	return 0;

}


```

注意int最大为1e10，需要敏感，只要规模大于等于1e11的（⚠️尤其是ans），都需要开==long long==

```c++
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <math.h>
#include <queue>
#include <iomanip>

using namespace std;


struct LUOGU {
	long long t;
	long long v;
	LUOGU(long long a, long long b) :t(a), v(b) {}
};

const long long MAXNM = 1e4 + 5;
const long long MAXNT = 1e7 + 5;
vector<LUOGU> Dict(MAXNM, LUOGU(0, 0));
long long T, M;
long long dp[MAXNT];


int main()
{
	cin >> T >> M;

	for (long long i = 1; i <= M; i++) {
		long long u;
		long long v;
		cin >> u >> v;
		Dict[i] = LUOGU(u, v);
	}

	for (long long i = 1; i <= M; i++) {
		for (long long j = Dict[i].t; j <= T ; j++) {
			dp[j] = max(dp[j - Dict[i].t] + Dict[i].v, dp[j]);
		}
	}

	cout << dp[T];
	return 0;

}


```

##### [P1757 通天之分组背包](https://www.luogu.com.cn/problem/P1757)

分组背包

0-1背包的变种，每组中的物品互相冲突，求最大利用价值

在原来题目的基础上再加一个循环，枚举这个组内物品。注意总是要跟dp\[i][k]比较，否则会覆盖同组的数据。

```c++
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <math.h>
#include <queue>
#include <iomanip>

using namespace std;

int m, n, mx;

struct LUOGU {
	int t;
	int v;
	LUOGU(int a, int b) :t(a), v(b) {}
};

const int MAXN = 1e3 + 5;

vector<vector<LUOGU> > Group(MAXN, vector<LUOGU>());
int dp[MAXN][MAXN];

int main()
{
	cin >> m >> n;
	for (int i = 1; i <= n; i++) {
		int u, v, w;
		cin >> u >> v >> w;
		Group[w].push_back(LUOGU(u, v));
		if (w > mx)	mx = w;
	}

	for (int i = 1; i <= mx; i++) {
		for (int j = 0; j < Group[i].size(); j++) {
			for (int k = 1; k <= m; k++) {
				if (k < Group[i][j].t) {
					dp[i][k] = max(dp[i - 1][k], dp[i][k]);
				}
				else {
					dp[i][k] = max(max(dp[i - 1][k - Group[i][j].t] + Group[i][j].v, dp[i-1][k]), dp[i][k]);
				}
			}
		}
	}

	cout << dp[mx][m];

	return 0;
}


```

##### [P1064 金明的预算方案](https://www.luogu.com.cn/problem/P1064)

**有依赖的动态规划**：把依赖写出来

几个条件：

- 购买附件，必须得先购买主件
- 希望每件物品的价格x重要度的乘积最大

一般来说，如果题目给的信息越详细，就说明代码越笨

之前的0-1背包问题有两种情况，在此规定了（附件不会有附件、主件最多只有2个附件），因此就是5种情况，分别罗列状态方程即可。

二维数组版本：

```c++
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <math.h>
#include <queue>
#include <iomanip>

using namespace std;

int n, m;

struct LUOGU {
	int v, w;
	LUOGU(int a, int b) :v(a), w(b) {};
};


const int MAXNN = 32005;
const int MAXNM = 65;
vector<vector<LUOGU> > Dict(MAXNM, vector<LUOGU>());
int dp[MAXNM][MAXNN];

int main()
{
	cin >> n >> m;
	for (int i = 1; i <= m; i++) {
		int u, v, w;
		cin >> u >> v >> w;
		if (!w) Dict[i].push_back(LUOGU(u, v));
		else Dict[w].push_back(LUOGU(u, v));
	}

	
	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++) {
			if (!Dict[i].size())	dp[i][j] = dp[i - 1][j];
			else {
				if (j < Dict[i][0].v)	dp[i][j] = dp[i-1][j];
				else dp[i][j] = max(dp[i - 1][j - Dict[i][0].v] + Dict[i][0].v * Dict[i][0].w, dp[i - 1][j]);


				if (Dict[i].size() > 1 && j >= Dict[i][0].v + Dict[i][1].v) {
					dp[i][j] = max(dp[i - 1][j - Dict[i][0].v - Dict[i][1].v] + Dict[i][0].v * Dict[i][0].w + Dict[i][1].v * Dict[i][1].w, dp[i][j]);
				}

				if (Dict[i].size() > 2 && j >= Dict[i][0].v + Dict[i][2].v) {
					dp[i][j] = max(dp[i - 1][j - Dict[i][0].v - Dict[i][2].v] + Dict[i][0].v * Dict[i][0].w + Dict[i][2].v * Dict[i][2].w, dp[i][j]);
				}

				if (Dict[i].size() > 2 && j >= Dict[i][0].v + Dict[i][1].v + Dict[i][2].v) {
					dp[i][j] = max(dp[i - 1][j - Dict[i][0].v - Dict[i][1].v - Dict[i][2].v] + Dict[i][0].v * Dict[i][0].w + Dict[i][1].v * Dict[i][1].w + Dict[i][2].v * Dict[i][2].w, dp[i][j]);
				}

			}
		}
	}

	cout << dp[m][n];

	return 0;
}


```

别再被dp数组的维数坑了...omgg

一维数组版本：

```c++
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <math.h>
#include <queue>
#include <iomanip>

using namespace std;

int n, m;

struct LUOGU {
	int v, w;
	LUOGU(int a, int b) :v(a), w(b) {};
};


const int MAXN = 32005;
vector<vector<LUOGU> > Dict(MAXN, vector<LUOGU>());
int dp[MAXN];

int main()
{
	cin >> n >> m;
	for (int i = 1; i <= m; i++) {
		int u, v, w;
		cin >> u >> v >> w;
		if (!w) Dict[i].push_back(LUOGU(u, v));
		else Dict[w].push_back(LUOGU(u, v));
	}

	
	for (int i = 1; i <= m; i++) {
		for (int j = n; j > 0; j--) {
			if (Dict[i].size()) {
				if (j < Dict[i][0].v)	dp[j] = dp[j];
				else dp[j] = max(dp[j - Dict[i][0].v] + Dict[i][0].v * Dict[i][0].w, dp[j]);


				if (Dict[i].size() > 1 && j >= Dict[i][0].v + Dict[i][1].v) {
					dp[j] = max(dp[j - Dict[i][0].v - Dict[i][1].v] + Dict[i][0].v * Dict[i][0].w + Dict[i][1].v * Dict[i][1].w, dp[j]);
				}

				if (Dict[i].size() > 2 && j >= Dict[i][0].v + Dict[i][2].v) {
					dp[j] = max(dp[j - Dict[i][0].v - Dict[i][2].v] + Dict[i][0].v * Dict[i][0].w + Dict[i][2].v * Dict[i][2].w, dp[j]);
				}

				if (Dict[i].size() > 2 && j >= Dict[i][0].v + Dict[i][1].v + Dict[i][2].v) {
					dp[j] = max(dp[j - Dict[i][0].v - Dict[i][1].v - Dict[i][2].v] + Dict[i][0].v * Dict[i][0].w + Dict[i][1].v * Dict[i][1].w + Dict[i][2].v * Dict[i][2].w, dp[j]);
				}

			}
		}
	}

	cout << dp[n];

	return 0;
}


```

能想到一维dp的话尽量用一维dp，不容易错

##### [数字三角形 Number Triangles](https://www.luogu.com.cn/problem/P1216)

一维动态规划

```c++
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <math.h>
#include <queue>
#include <iomanip>

using namespace std;

int r;
const int MAXN = 1e3 + 5;
int Dict[MAXN][MAXN];
int dp[MAXN];


int main()
{
	cin >> r;
	for (int i = 1; i <= r; i++) {
		for (int j = 1; j <= i; j++) {
			cin >> Dict[i][j];
		}
	}

	for (int j = 1; j <= r; j++) {
		dp[j] = Dict[r][j];
	}

	for (int i = r - 1; i >= 1; i--) {
		for (int j = 1; j <= i; j++) {
			dp[j] = max(dp[j], dp[j + 1]) + Dict[i][j];
		}
	}
	
	cout << dp[1];

	return 0;
}

```

##### [P1434 [SHOI2002]滑雪](https://www.luogu.com.cn/problem/P1434)

多维dp，求一个矩阵中递减数列最长的长度。

有几个注意点：

- dp不一定是每一行每一列一个个遍历（由前迭代至后），最开始思维局限在这样的遍历法，会发现没办法从下至上的计算

- dp有时候需要和dfs结合（例如曾经leetcode管道那个题），由后递归然后更新前面的状态，dp数组的意义在于记忆化搜索。

- 涉及到四个方向的题目：

  ```
  int dx[4] = { 0,0,1,-1 };
  int dy[4] = { 1,-1,0,0 };
  ```

这道题我也觉得应该要用dfs + dp来做，但最开始的思路是“更新——>递归”，思维局限，事实上应该是“递归——>更新”。太久没做递归题，手生了。

AC代码：

```c++
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <math.h>
#include <queue>
#include <iomanip>

using namespace std;

const int MAXN = 105;

int dict[MAXN][MAXN];
int dp[MAXN][MAXN];
int R,C;
int mx;

int dx[4] = { 0,0,1,-1 };
int dy[4] = { 1,-1,0,0 };


int dfs(int x, int y) {
	if (dp[x][y]) return dp[x][y];
	dp[x][y] = 1;

	for (int i = 0; i < 4; i++) {
		int nx = x + dx[i];
		int ny = y + dy[i];
		if (nx > 0 && ny > 0 && nx <= R && ny <= C) {
			if (dict[nx][ny] < dict[x][y]) {
				dp[x][y] = max(dfs(nx, ny) + 1, dp[x][y]);
			}
		}
	}

	return dp[x][y];
}

int main()
{
	cin >> R >> C;



	for (int i = 1; i <= R; i++) {
		for (int j = 1; j <= C; j++) {
			cin >> dict[i][j];
		}
	}

	for (int i = 1; i <= R; i++) {
		for (int j = 1; j <= C; j++) {
			mx = max(dfs(i, j), mx);
		}
	}

	cout << mx;


	return 0;
}

```

##### [P1854 花店橱窗布置](https://www.luogu.com.cn/problem/P1854)

区间DP

区间dp最重要的是**枚举**，并且需要确定范围。

70分，WA了三个数据点，稍后再查看。

```c++
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <math.h>
#include <queue>
#include <iomanip>

using namespace std;

const int MAXN = 105;

int F, V;

int dict[MAXN][MAXN];
int dp[MAXN][MAXN];
int ans[MAXN][MAXN];
vector<int> tmp;

int main()
{
	cin >> F >> V;
	for (int i = 1; i <= F; i++) {
		for (int j = 1; j <= V; j++) {
			cin >> dict[i][j];
		}
	}

	for (int i = 1; i <= F; i++) {
		for (int j = i + 1; j <= V - F + i; j++) {
			for (int k = i; k < j; k++) {
				if (dp[i - 1][k] + dict[i][j] > dp[i][j]) {
					dp[i][j] = dp[i - 1][k] + dict[i][j];
					ans[i][j] = k;
				}
			}
		}
	}

	int mx = 0;
	for (int i = 0; i <= V; i++) {
		mx = max(mx, dp[F][i]);
	}
	
	cout << mx << endl;

	mx = 0;
	int flag = 0;

	for (int i = 1; i <= V; i++) {
		if (dp[F][i] > mx) {
			mx = dp[F][i];
			flag = i;
		}
	}

	for (int i = F; i >= 1; i--) {
		tmp.push_back(flag);
		flag = ans[i][flag];
	}

	for (int i = tmp.size() - 1; i >= 0; i--) {
		cout << tmp[i] << " ";
	}

	return 0;
}


```

