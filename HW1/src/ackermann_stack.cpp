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
