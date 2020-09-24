## 高精度

##### [P1601 A+B Problem（高精）](https://www.luogu.com.cn/problem/P1601)

高精度加法，先加，再进位

```c++
#include <iostream>
#include <string>
#include <vector>

using namespace std;


const int MAXN = 505;
int A[MAXN], B[MAXN], C[MAXN];


int main()
{
	string a, b;
	cin >> a;

	for (int i = 0; i < a.size(); i++) {
		A[i] = a[a.size() - 1 - i] - '0';
	}

	cin >> b;

	for (int i = 0; i < b.size(); i++) {
		B[i] = b[b.size() - 1 - i] - '0';
	}

	int len = 0;
	while (len < a.size() || len < b.size()) {
		C[len] = A[len] + B[len];
		len++;
	}
	
	
	for (int i = 0; i < len; i++) {
		C[i + 1] += C[i] / 10;
		C[i] = C[i] % 10;
	}

	if (C[len]) len++;

	for (int i = len - 1; i >= 0; i--) {
		cout << C[i];
	}

	return 0;
}
```

##### [P1303 A*B Problem](https://www.luogu.com.cn/problem/P1303)

两个数从标号为1开始存储，则C[i+j-1] += A[i] * B[j]，最后再进位

注意0，计算出来不能是000，必须返回0

```c++
#include <iostream>
#include <vector>
#include <string>

using namespace std;


const int MAXN = 2005;
const int MMAXN = 2000 * 2000 + 5;
int A[MAXN], B[MAXN], C[MMAXN];
string a, b;


int main()
{

	cin >> a >> b;
	if (a == "0" || b == "0") {
		cout << 0;
		return 0;
	}

	for (int i = 0; i < a.size(); i++) {
		A[i+1] = a[a.size() - 1 - i] - '0';
	}

	for (int i = 0; i < b.size(); i++) {
		B[i+1] = b[b.size() - 1 - i] - '0';
	}

	int Alen = a.size() + 1;
	int Blen = b.size() + 1;

	for (int i = 1; i < Alen; i++) {
		for (int j = 1; j < Blen; j++) {
			C[i + j - 1] += A[i] * B[j];
		}
	}

	for (int i = 1; i < Alen + Blen - 2; i++) {
		C[i + 1] += C[i] / 10;
		C[i] = C[i] % 10;
	}
	
	int len = 0;
	if (C[Alen + Blen - 2])	len = Alen + Blen - 1;
	else len = Alen + Blen - 2;

	for (int i = len-1; i > 0; i--) {
		cout << C[i];
	}
	return 0;
}
```

##### [P2142 高精度减法](https://www.luogu.com.cn/problem/P2142)

注意a、b是否相等，如若相等，结果为0

先减后进位。

```c++
#include <iostream>
#include <vector>
#include <string>

using namespace std;


const int MAXN = 10086 + 5;
int A[MAXN], B[MAXN], C[MAXN];
string a, b;


int main()
{
	
	cin >> a >> b;
	if (a == b) {
		cout << "0";
		return 0;
	}

	int flag = 0;
	if ((a.size() <= b.size()) || (a.size() == b.size() && a[0] < b[0])){
		flag = 1;
		swap(a, b);
	}

	for (int i = 0; i < a.size(); i++) {
		A[i] = a[a.size() - i - 1] - '0';
	}

	for (int i = 0; i < b.size(); i++) {
		B[i] = b[b.size() - i - 1] - '0';
	}

	int len = 0;
	while (len < a.size() || len < b.size()) {
		C[len] = A[len] - B[len];
		len++;
	}
	
	for (int i = 0; i < len; i++) {
		if (C[i] < 0) {
			C[i] += 10;
			C[i + 1] -= 1;
		}
	}
	
	if (!C[len - 1]) {
		len--;
	}
	
	if (flag) cout << '-';

	for (int i = len - 1; i >= 0; i--) {
		cout << C[i];
	}

	return 0;
}
```

##### [P1480 A/B Problem](https://www.luogu.com.cn/problem/P1480)

大整数的除法

注意57002 / 10此类情况

```c++
#include <iostream>
#include <vector>
#include <string>

using namespace std;


const int MAXN = 5005;

string a,B;
int b;
vector<int> A,ans;


void divide(int start, int left) {
	if (start >= A.size()) return;
	int idx = start;
	int tmp = left * 10 + A[start];
	while (tmp < b) {
		ans.push_back(0);
		idx++;
		if (idx >= A.size()) break;
		tmp = tmp * 10 + A[idx];
	}
	if(idx < A.size())	ans.push_back(tmp / b);
	divide(idx + 1, tmp % b);
}


int main()
{
	cin >> a >> b;
	
	B = to_string(b);
	

	for (int i = 0; i < a.size(); i++) {
		A.push_back(a[i] - '0');
	}
	
	if (A.size() < B.size() || (A.size() == B.size() && A[0] < B[0])) {
		cout << 0;
		return 0;
	}


	divide(0, 0);
	int flag = 0;
	for (int i = 0; i < ans.size(); i++) {
		if (!flag && ans[i] != 0) {
			flag = 1;
			cout << ans[i];
		}
		else if (flag)	cout << ans[i];
	}
	
	return 0;
}
```

##### [P2818 天使的起誓](https://www.luogu.com.cn/problem/P2818)

注意：如果正好为整数倍，返回的是n而不是0

```c++
#include <iostream>
#include <vector>
#include <string>

using namespace std;


const int MAXN = 1e3 + 5;

string a,B;
int b;
vector<int> A, ans;


void divide(int start, int left) {
	ans.clear();
	ans.push_back(left);

	if (start >= A.size()) return;
	int idx = start;
	int tmp = left * 10 + A[start];
	while (tmp < b) {
		idx++;
		if (idx >= A.size()) break;
		tmp = tmp * 10 + A[idx];
	}
	divide(idx + 1, tmp % b);
}


int main()
{
	cin >> b >> a;
	
	B = to_string(b);
	

	for (int i = 0; i < a.size(); i++) {
		A.push_back(a[i] - '0');
	}

	if (A.size() < B.size() || (A.size() == B.size() && A[0] < (B[0] - '0'))) {
		cout << 0;
		return 0;
	}


	divide(0, 0);
	int flag = 0;
	for (int i = 0; i < ans.size(); i++) {
		if (!flag && ans[i] != 0) {
			flag = 1;
			cout << ans[i];
		}
		else if (flag)	cout << ans[i];
	}

	if (!flag) cout << b;
	
	return 0;
}
```

