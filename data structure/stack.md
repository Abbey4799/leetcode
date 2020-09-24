## 栈

##### [P1449 后缀表达式](https://www.luogu.com.cn/problem/P1449)

记住洛谷没有stoi()

```c++
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <math.h>
#include <queue>
#include <iomanip>
#include <stack>
#include <cctype>
#include <sstream>

using namespace std;

string a;
int temp;
stack<int> ss;

int main()
{
	cin >> a;
	for (int i = 0; i < a.size() - 1; i++) {
		if (a[i] == '.') {
			ss.push(temp);
			temp = 0;
		}else if (a[i] == '+' || a[i] == '-' || a[i] == '*' || a[i] == '/') {
			int s1 = ss.top();
			ss.pop();
			int s2 = ss.top();
			ss.pop();
			
			if (a[i] == '+') ss.push(s1 + s2);
			else if (a[i] == '-') ss.push(s2 - s1);
			else if (a[i] == '*') ss.push(s2 * s1);
			else if (a[i] == '/') ss.push(s2 / s1);
		}
		else {
			int tmp = a[i] - '0';
			temp = temp * 10 + tmp;
		}
	}

	cout << ss.top();

	return 0;
}


```

