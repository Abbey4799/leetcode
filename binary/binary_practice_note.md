## 二分查找

##### [P2249 【深基13.例1】查找](https://www.luogu.com.cn/problem/P2249)

值得注意的是：

- 找到Dict[mid] == u的时候，不能直接返回，需要继续往左找，因为需要确定第一次出现的位置；直到最后长度为1为止（start == end）
- 由于需要继续找，所以不能让end = mid-1，而是end = mid

```c++
#include <iostream>
#include <vector>
#include <string>

using namespace std;


int n, m;
const int MAXN = 1e6 + 5;
int Dict[MAXN];


int find(int start, int end, int u) {

  //最后的结束界限是只有1个位置，而不是start > end
	if (start == end) {
		if (Dict[start] == u) return start;
		else return -1;
	}


	if (start > end) return -1;

	int mid = (start + end) / 2;
  
  // Attention
	if (Dict[mid] >= u) end = mid;
	else if (Dict[mid] < u) start = mid + 1;

	return find(start, end, u);

}

int main()
{
	cin >> n >> m;
	for (int i = 1; i < n+1; i++) {
		cin >> Dict[i];
	}

	for (int i = 0; i < m; i++) {
		int tmp;
		cin >> tmp;
		cout << find(1, n, tmp) << " ";
	}
	
	return 0;

}
```

##### [P1024 一元三次方程求解](https://www.luogu.com.cn/problem/P1024)

已知一个一元三次方程，并且给出各项系数，已知该方程存在三个不同实根，则**从小到大**一次输出这三个实根，精确到小数点后两位

关键点：

<img src="/Users/apple/Library/Application Support/typora-user-images/image-20200912133712406.png" alt="image-20200912133712406" style="zoom:50%;" />

God...别再被double坑了

```c++
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <iomanip>

using namespace std;

double a, b, c, d;
double s1, e1, s2, e2;
vector<double> ans;


double cal(double x) {
	return a * pow(x, 3) + b * pow(x, 2) + c * pow(x, 1) + d;
}

void find(double start, double end) {
	if (ans.size() == 3) {
		return;
	}
	if (abs(start - end) < 0.001) {
		ans.push_back(start);
		return;
	}
	double mid = (start + end) / 2;
	double x, y, z;
	x = cal(start);
	y = cal(mid);
	z = cal(end);


	if (x == 0) {
		ans.push_back(start);
		return;
	}

	if (x * y < 0) {
		find(start, mid);
	}
	if (y * z < 0) {
		find(mid, end);
	}
}

int main()
{
	cin >> a >> b >> c >> d;
	for (int i = -100; i < 100; i++) {
		find(i, i + 1);
	}

	cout << setiosflags(ios::fixed) << setprecision(2);
	for (int i = 0; i < ans.size(); i++) {
		cout << ans[i] << " ";
	}
	return 0;

}
```

