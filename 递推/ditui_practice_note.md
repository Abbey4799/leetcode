## 递推

##### [P4994 终于结束的起点](https://www.luogu.com.cn/problem/P4994)

最开始的做法：30分；MLE了一部分，WA了一部分，应该是由于溢出。

```c++
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <iomanip>

using namespace std;

int M;
int tmp,n1,n2;
int cnt = 0;

void fibo() {
	while (1) {
		cnt++;
		tmp = n1 + n2;
		if (n2 % M == 0 && tmp % M == 1) {
			cout << cnt;
			return;
		}

		n1 = n2;
		n2 = tmp;
	}
}

int main()
{
	cin >> M;
	n1 = 0, n2 = 1;
	fibo();

	return 0;

}
```

所以在Attention部分把规模降下来，涉及到mod的可以先mod再相加。

```c++
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <iomanip>

using namespace std;

int M;
int tmp,n1,n2;
int cnt = 0;

void fibo() {
	while (1) {
		cnt++;
		// Attention
		tmp = (n1 + n2) % M;
		n2 = n2 % M;

		if (n2 % M == 0 && tmp % M == 1) {
			cout << cnt;
			return;
		}

		n1 = n2;
		n2 = tmp;
	}
}

int main()
{
	cin >> M;
	n1 = 0, n2 = 1;
	fibo();

	return 0;

}
```

##### [P1044 栈](https://www.luogu.com.cn/problem/P1044)

卡特兰数，经典递推

这个递推式找得我...但是成功想出来了！

```c++
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <iomanip>

using namespace std;

int n;
const int MAXN = 19;
int M[MAXN][MAXN];

int main()
{
	cin >> n;
	M[1][1] = 1;
	for (int i = 2; i <= n; i++) {
		for (int j = 1; j <= i; j++) {
			if (j == 1)	M[i][j] = M[i - 1][i - 1];
			else M[i][j] =  M[i][j - 1] + M[i - 1][i - 1] - M[i - 1][j - 2];
		}
	}

	cout << M[n][n];

	return 0;

}

```

