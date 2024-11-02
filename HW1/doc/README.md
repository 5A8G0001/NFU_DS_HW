# HW 1-Problem 1:Ackermann's function

## 1. Problem 1:Ackermann's function(Recursive)

### 1-1.解題說明

以遞迴實作計算 Ackermann's function，已知其函數定義如下：

$$
A(m, n) =
\begin{cases}
n + 1 & \text{if } m = 0 \\
A(m - 1, 1) & \text{if } n = 0 \\
A(m - 1, A(m, n - 1)) & \text{otherwise}
\end{cases}
$$

已知:m=0 or m=1時，A(m,n)=n+1+m，m=2時A(m,n)=n*2+3，;

因此，m<=2的查詢均為常數時間

在m=3時,想要查A(m,n)就需要n長度的陣列，m>=4則需要[m-2][A(m-1,n)]大小的陣列，後續以此類推

也就是說，A(m,n)的計算，需要補全A(m-1,n)長度的n

實作參見檔案 `ackermann_recursive.cpp`，其遞迴函式，在函式設計中以遞迴+查表實作，減少無意義的函式呼叫：

```cpp
long long ackermann_non_recursive(int m, int n, long long** dp_v, int* sizes) {
    // 處理 m == 2 的特殊情況
    if (m == 2) {
        return n * 2 + 3;
    }

    // 檢查是否需要擴充陣列，提前確保大小足夠(上一層的n長度是否足夠)
    if (n + 1 > sizes[m - 3]) {
        dp_v[m - 3] = resize_array(dp_v[m - 3], sizes[m - 3], n + 1);
        sizes[m - 3] = n + 1;  // 更新col的大小
    }

    // 如果需要計算值，則進行遞迴計算(往回查表，無值則補全)
    if (n > 0) {
        for (int j = 1; j <= n; j++) {
            if (dp_v[m - 3][j] == -1) {
                //cout << "recursive!" << endl;
                dp_v[m - 3][j] = ackermann_non_recursive(m - 1, dp_v[m - 3][j - 1], dp_v, sizes);
            }
        }
    } else {// n=0的情況，A(m,n)值將直接等於A(m-1,1)
        if (dp_v[m - 3][n] == -1) {
            dp_v[m - 3][n] = ackermann_non_recursive(m - 1, 1, dp_v, sizes);
        }
    }

    return dp_v[m - 3][n];  // 返回已經計算好的結果
}
```

### 1-2. 演算法設計與實作

```cpp
#include <iostream>

using namespace std;

// 自定義函數來模擬 vector::resize() 行為
long long* resize_array(long long* old_array, int old_size, int new_size) {
    // 動態分配新陣列
    long long* new_array = new long long[new_size];

    // 將舊陣列內容複製到新陣列
    for (int i = 0; i < old_size; i++) {
        new_array[i] = old_array[i];
    }

    // 將剩餘部分初始化為 -1（表示尚未計算）
    for (int i = old_size; i < new_size; i++) {
        new_array[i] = -1;
    }

    // 釋放舊陣列記憶體
    delete[] old_array;

    return new_array;  // 返回新陣列的指針
}

long long ackermann_non_recursive(int m, int n, long long** dp_v, int* sizes) {
    // 處理 m == 2 的特殊情況
    if (m == 2) {
        return n * 2 + 3;
    }

    // 檢查是否需要擴充陣列，提前確保大小足夠
    if (n + 1 > sizes[m - 3]) {
        dp_v[m - 3] = resize_array(dp_v[m - 3], sizes[m - 3], n + 1);
        sizes[m - 3] = n + 1;  // 更新行的大小
    }

    // 如果需要計算值，則進行遞迴計算(往回查表)
    if (n > 0) {
        for (int j = 1; j <= n; j++) {
            if (dp_v[m - 3][j] == -1) {
                //cout << "recursive!" << endl;
                dp_v[m - 3][j] = ackermann_non_recursive(m - 1, dp_v[m - 3][j - 1], dp_v, sizes);
            }
        }
    } else { // n=0的情況，A(m,n)值將直接等於A(m-1,1)
        if (dp_v[m - 3][n] == -1) {
            dp_v[m - 3][n] = ackermann_non_recursive(m - 1, 1, dp_v, sizes);
        }
    }

    return dp_v[m - 3][n];  // 返回已經計算好的結果
}

int main()
{
    int m, n;
    cin >> m >> n;

    // 處理 m < 2 和 m == 2 的特殊情況，直接輸出結果
    if (m < 2) {
        cout << n + 1 + m << endl;
        return 0;
    } else if (m == 2) {
        cout << n * 2 + 3 << endl;
        return 0;
    }

    // 動態分配二維陣列 dp_v，初始大小 n+1
    long long** dp_v = new long long*[m - 2];  // 分配 m-2 行
    int* sizes = new int[m - 2];  // 記錄每一行的大小
    for (int i = 0; i < m - 2; i++) {
        dp_v[i] = new long long[n + 1];  // 每行初始分配 n+1 個元素
        sizes[i] = n + 1;  // 初始化大小為 n + 1
        for (int j = 0; j <= n; j++) {
            dp_v[i][j] = -1;  // 初始化為 -1，表示尚未計算
        }
    }


    if (!dp_v) {
        cerr << "Memory allocation failed!" << endl;
        return -1;
    }

    // 從 m = 3 開始進行計算，並填充 dp_v 表
    for (int i = 3; i <= m; i++) {
        // 對每一行，確保大小足夠，進行計算
        if (n + 1 > sizes[i - 3]) {
            dp_v[i - 3] = resize_array(dp_v[i - 3], sizes[i - 3], n + 1);
            sizes[i - 3] = n + 1;  // 更新col的大小
        }
        for (int j = 0; j <= n; j++) {
            if (j == 0) {
                // 當 j == 0 時，計算 A(i-1, 1) 並存入 dp_v
                dp_v[i - 3][j] = ackermann_non_recursive(i - 1, 1, dp_v, sizes);
            } else {
                // 否則計算 A(i-1, dp_v[i-3][j-1])，並存入 dp_v
                dp_v[i - 3][j] = ackermann_non_recursive(i - 1, dp_v[i - 3][j - 1], dp_v, sizes);
            }
        }
    }

    // 輸出最終結果
    cout << dp_v[m - 3][n] << endl;

    // 釋放動態分配的記憶體
    for (int i = 0; i < m - 2; i++) {
        delete[] dp_v[i];  // 釋放每一行
    }
    delete[] dp_v;  // 釋放整個動態陣列
    delete[] sizes; // 釋放 sizes 陣列

    system("pause");

    return 0;
}

```

### 1-2 效能分析
#### f(m,n)
- $if &nbsp; m <= 2 , f(m,n) = O(1) $
- $if m = 3 , f(m,n) = O(n) $
- $if m >= 4 , f(m,n) = O\left(2^{2^{\cdots n}}\right)$
#### S(P)
- $if &nbsp; m <=2 , S(P) = 1$
- $if &nbsp; m >=3 , S(P) = (m-2) \times (n+1)$
#### T(P)
- $if m = 3 , T(P) = C \times n $
- $if m >= 4 , T(P) = O(n^m) $



### 1-3. 測試與過程

#### Input

```plain
2 12
3 26
4 1

```

#### Output

```plain
27
536870909
65533

```

#### 驗證
m = 0 -> A(0,n) = n+1  

0 0 -> a = 1  
0 1 -> a = 2  
0 2 -> a = 3  

1 0 -> 0,1 -> a = 2  
1 1 -> 0,(1,0)=2 -> 0,2 -> a = 3  

2 0 -> 1,1 -> a = 3  
2 1 -> 1,(2,0)=3 -> 1,3 -> a = 5  
2 2 -> 1,(2,1)=5 -> 1,5 -> a = 7  
2 3 -> 1,(2,2)=7 -> 1,7 -> a = 9  

If m=0 or m=1,A(m,n) = n+1+m，  
   m=2 -> A(m,n) = n*2+3  

else -> A(m-1,a(m,n-1) )  


A(3,0) -> A(2,1) -> 5  
A(3,1) -> A(2,A(3,0)=5) -> A(2,5) -> 13  
.  
.  
.  
A(4,0) -> A(3,1) ->  13  
A(4,1) -> A(3,A(4,0)=13) -> A(3,13) -> 65533  
A(4,2) -> A(3,A(4,1)=65533) -> A(3,65533) -> ....  



## 2. Problem 1:Ackermann's function(Iterator)

### 2-1.解題說明

與遞迴版本的差異在於以堆疊代替了函式內對自己的呼叫。

以非遞迴實作計算 Ackermann's function，已知其函數定義如下：

$$
A(m, n) =
\begin{cases}
n + 1 & \text{if } m = 0 \\
A(m - 1, 1) & \text{if } n = 0 \\
A(m - 1, A(m, n - 1)) & \text{otherwise}
\end{cases}
$$

已知:m=0 or m=1時，a(m,n)=n+1+m，m=2時a(m,n)=n*2+3，;

因此，m<=2的查詢均為常數時間


在m=3時,想要查m,n就需要n長度的陣列，m=4則需要m-1,n長度陣列，後續以此類推

也就是說，m需要查m-1的n的補全

實作參見檔案 `ackermann_stack.cpp`，其迭代函式，在函式設計中以stack+查表實作，減少無意義的函式呼叫：

```cpp
long long ackermann_iterative(int m, int n, long long** dp_v, int* sizes) {
    // 模擬 stack 的動態陣列
    int stack_size = 1000;  // 初始大小，根據需要調整
    int top = -1;  // stack頂部指標
    StackElement* stk = new StackElement[stack_size];

    // 將初始狀態推入stack
    stk[++top] = {m, n};
    long long result = -1;

    while (top >= 0) {
        StackElement current_element = stk[top];  // 取得當前 m, n
        int current_m = current_element.m;
        int current_n = current_element.n;
        top--;  // 模擬 pop

        if (current_m == 2) {
            result = current_n * 2 + 3;  // A(2, n) = 2n + 3
        } else if (current_m == 0) {
            result = current_n + 1;  // A(0, n) = n + 1
        } else if (current_n == 0) {
            // 如果需要擴大stack大小
            if (top + 1 >= stack_size) {
                stack_size *= 2;
                stk = resize_array_stk(stk, stack_size / 2, stack_size);
            }
            stk[++top] = {current_m - 1, 1};  // A(m, 0) = A(m-1, 1)
        } else {
            //  A(m-1, A(m, n-1))，先計算 A(m, n-1)
            if (top + 2 >= stack_size) {
                stack_size *= 2;
                stk = resize_array_stk(stk, stack_size / 2, stack_size);
            }
            stk[++top] = {current_m - 1, -1};  // 暫存 -1，稍後更新
            stk[++top] = {current_m, current_n - 1};
            continue;
        }

        // 如果是暫存的 -1，將計算結果填回stack頂部
        if (top >= 0 && stk[top].n == -1) {
            stk[top].n = result;
        }
    }

    delete[] stk;  // 釋放動態分配的stack記憶體
    return result;
}
```

### 2-2. 演算法設計與實作

```cpp
#include <iostream>

using namespace std;

//  pair<int, int> 模擬stack
struct StackElement {
    int m;
    int n;
};

// 模擬 vector::resize() 的函數
long long* resize_array(long long* old_array, int old_size, int new_size) {
    long long* new_array = new long long[new_size];
    for (int i = 0; i < old_size; i++) {
        new_array[i] = old_array[i];
    }
    for (int i = old_size; i < new_size; i++) {
        new_array[i] = -1;
    }
    delete[] old_array;
    return new_array;
}

// 用於stack的resize
StackElement* resize_array_stk(StackElement* old_array, int old_size, int new_size) {
    StackElement* new_array = new StackElement[new_size];
    for (int i = 0; i < old_size; i++) {
        new_array[i] = old_array[i];
    }
    for (int i = old_size; i < new_size; i++) {
        new_array[i].n = -1;
    }
    delete[] old_array;
    return new_array;
}

// 迭代版本的 Ackermann 函數
long long ackermann_iterative(int m, int n, long long** dp_v, int* sizes) {
    // 模擬 stack 的動態陣列
    int stack_size = 1000;  // 初始大小，根據需要調整
    int top = -1;  // stack頂部指標
    StackElement* stk = new StackElement[stack_size];

    // 將初始狀態推入stack
    stk[++top] = {m, n};
    long long result = -1;

    while (top >= 0) {
        StackElement current_element = stk[top];  // 取得當前 m, n
        int current_m = current_element.m;
        int current_n = current_element.n;
        top--;  // 模擬 pop

        if (current_m == 2) {
            result = current_n * 2 + 3;  // A(2, n) = 2n + 3
        } else if (current_m == 0) {
            result = current_n + 1;  // A(0, n) = n + 1
        } else if (current_n == 0) {
            // 如果需要擴大stack大小
            if (top + 1 >= stack_size) {
                stack_size *= 2;
                stk = resize_array_stk(stk, stack_size / 2, stack_size);
            }
            stk[++top] = {current_m - 1, 1};  // A(m, 0) = A(m-1, 1)
        } else {
            //  A(m-1, A(m, n-1))，先計算 A(m, n-1)
            if (top + 2 >= stack_size) {
                stack_size *= 2;
                stk = resize_array_stk(stk, stack_size / 2, stack_size);
            }
            stk[++top] = {current_m - 1, -1};  // 暫存 -1，稍後更新
            stk[++top] = {current_m, current_n - 1};
            continue;
        }

        // 如果是暫存的 -1，將計算結果填回stack頂部
        if (top >= 0 && stk[top].n == -1) {
            stk[top].n = result;
        }
    }

    delete[] stk;  // 釋放動態分配的stack記憶體
    return result;
}

int main() {
    int m, n;
    cin >> m >> n;

    // 處理特殊情況 m < 2 和 m == 2
    if (m < 2) {
        cout << n + 1 + m << endl;
        system("pause");
        return 0;
    } else if (m == 2) {
        cout << n * 2 + 3 << endl;
        system("pause");
        return 0;
    }

    // 初始化 dp_v 二維陣列，大小為 m-2 行，每行 n+1 個
    long long** dp_v = new long long*[m - 2];
    int* sizes = new int[m - 2];

    for (int i = 0; i < m - 2; i++) {
        dp_v[i] = new long long[n + 1];
        sizes[i] = n + 1;
        for (int j = 0; j <= n; j++) {
            dp_v[i][j] = -1;  // 初始化為 -1，表示尚未計算
        }
    }

    // 用迭代方式填充 dp_v 表
    for (int i = 3; i <= m; i++) {
        if (n + 1 > sizes[i - 3]) {
            dp_v[i - 3] = resize_array(dp_v[i - 3], sizes[i - 3], n + 1);
            sizes[i - 3] = n + 1;
        }

        for (int j = 0; j <= n; j++) {
            if (j == 0) {
                dp_v[i - 3][j] = ackermann_iterative(i - 1, 1, dp_v, sizes);
            } else {
                dp_v[i - 3][j] = ackermann_iterative(i - 1, dp_v[i - 3][j - 1], dp_v, sizes);
            }
        }
    }

    // 輸出最終結果
    cout << dp_v[m - 3][n] << endl;

    // 釋放動態分配的記憶體
    for (int i = 0; i < m - 2; i++) {
        delete[] dp_v[i];
    }
    delete[] dp_v;
    delete[] sizes;
    system("pause");

    return 0;
}

```

### 2-2 效能分析
#### f(m,n)
- $if m <= 2 , f(m,n) = O(1) $
- $if m = 3 , f(m,n) = O(n) $
- $if m >= 4 , f(m,n) = O\left(2^{2^{\cdots n}}\right)$
#### S(P)
- $if &nbsp; m <=2 , S(P) = 1$
- $if &nbsp; m >=3 , S(P) = (m-2) \times (n+1)$
#### T(P)
- $if m = 3 , T(P) = C \times n $
- $if m >= 4 , T(P) = O(n^m) $



### 2-3. 測試與過程

#### Input

```plain
2 12
3 26
4 1

```

#### Output

```plain
27
536870909
65533

```

#### 驗證
m = 0 -> A(0,n) = n+1  

0 0 -> a = 1  
0 1 -> a = 2  
0 2 -> a = 3  

1 0 -> 0,1 -> a = 2  
1 1 -> 0,(1,0)=2 -> 0,2 -> a = 3  

2 0 -> 1,1 -> a = 3  
2 1 -> 1,(2,0)=3 -> 1,3 -> a = 5  
2 2 -> 1,(2,1)=5 -> 1,5 -> a = 7  
2 3 -> 1,(2,2)=7 -> 1,7 -> a = 9  

If m=0 or m=1,A(m,n) = n+1+m，  
   m=2 -> A(m,n) = n*2+3  

else -> A(m-1,a(m,n-1) )  


A(3,0) -> A(2,1) -> 5  
A(3,1) -> A(2,A(3,0)=5) -> A(2,5) -> 13  
.  
.  
.  
A(4,0) -> A(3,1) ->  13  
A(4,1) -> A(3,A(4,0)=13) -> A(3,13) -> 65533  
A(4,2) -> A(3,A(4,1)=65533) -> A(3,65533) -> ....  

# HW 1-Problem 2:Powerset

## 1. 解題說明

以遞迴實作計算幂集 Powerset(S) 的函式，幂集定義如下：

有一集合S，它的幂集P(S)由S的所有子集組成的集合，包括空集合與S本身。  
例如，集合S={a,b,c}的幂集為:  
P(S) = {{},{a},{b},{c},{a,b},{a,c},{b,c},{a,b,c}}

實作參見檔案 `powerset.cpp`，其遞迴函式：

```cpp
// 遞迴函數來生成冪集
void powerset_recursive(char set[], char subset[], int set_size, int index, int subset_size) {
    // 當到達集合末尾時，輸出當前的子集
    if (index == set_size) {
        cout << "{ ";
        for (int i = 0; i < subset_size; i++) {
            cout << subset[i] << " ";
        }
        cout << "}" << endl;
        return;
    }

    // 不包含當前元素的情況
    powerset_recursive(set, subset, set_size, index + 1, subset_size);

    // 包含當前元素的情況
    subset[subset_size] = set[index];  // 將當前元素加入子集
    powerset_recursive(set, subset, set_size, index + 1, subset_size + 1);  // 子集大小增加
}
```

## 2. 演算法設計與實作

```cpp
#include <iostream>
using namespace std;

// 遞迴函數來生成冪集
void powerset_recursive(char set[], char subset[], int set_size, int index, int subset_size) {
    // 當到達集合末尾時，輸出當前的子集
    if (index == set_size) {
        cout << "{ ";
        for (int i = 0; i < subset_size; i++) {
            cout << subset[i] << " ";
        }
        cout << "}" << endl;
        return;
    }

    // 不包含當前元素的情況
    powerset_recursive(set, subset, set_size, index + 1, subset_size);

    // 包含當前元素的情況
    subset[subset_size] = set[index];  // 將當前元素加入子集
    powerset_recursive(set, subset, set_size, index + 1, subset_size + 1);  // 子集大小增加
}

int main() {
    char set[] = {'a', 'b', 'c'};
    int set_size = sizeof(set) / sizeof(set[0]);


    // 使用一個陣列來存儲當前子集，最大大小與集合大小相同
    char subset[set_size];

    // 計算並輸出冪集
    powerset_recursive(set, subset, set_size, 0, 0);

    system("pause");

    return 0;
}

```

## 2. 效能分析

- $f(n) = O(n*2^n)$
- $S(P) = 1 \times n$, 1 個變數、n 次遞迴。
- $T(P) = C \times n*2^n$, 每層迴圈所需 C 時間、n*2^n 次遞迴。

## 3. 測試與過程

### Input

```plain
{a,b,c}
{x,y}

```

### Output

```plain
{{},{a},{b},{c},{a,b},{a,c},{b,c},{a,b,c}}
{{},{x},{y},{x,y}}

```

### 驗證

