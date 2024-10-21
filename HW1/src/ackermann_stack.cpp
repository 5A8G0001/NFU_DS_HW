#include <iostream>
#include <stack>
#include <utility>  // 為了使用 std::pair

using namespace std;

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

// 迭代版本的 Ackermann 函數
long long ackermann_iterative(int m, int n, long long** dp_v, int* sizes) {
    stack<pair<int, int>> stk;  // 使用 stack 來模擬遞迴的計算過程
    stk.push({m, n});  // 將初始狀態推入堆疊

    long long result = -1;  // 儲存最終結果

    while (!stk.empty()) {
        auto current_pair = stk.top();  // 取得當前 m, n
        int current_m = current_pair.first;
        int current_n = current_pair.second;
        stk.pop();

        if (current_m == 2) {
            result = current_n * 2 + 3;  // A(2, n) = 2n + 3
        } else if (current_m == 0) {
            result = current_n + 1;  // A(0, n) = n + 1
        } else if (current_n == 0) {
            stk.push({current_m - 1, 1});  // A(m, 0) = A(m-1, 1)
        } else {
            // 模擬 A(m-1, A(m, n-1))，先計算 A(m, n-1)
            stk.push({current_m - 1, -1});  // 暫存 -1，稍後更新
            stk.push({current_m, current_n - 1});
            continue;  // 繼續進入下一次迭代
        }

        // 如果是暫存的 -1，將計算結果填回堆疊頂部
        if (!stk.empty() && stk.top().second == -1) {
            stk.top().second = result;
        }
    }

    return result;
}

int main() {
    int m, n;
    cin >> m >> n;

    // 處理特殊情況 m < 2 和 m == 2
    if (m < 2) {
        cout << n + 1 + m << endl;
        return 0;
    } else if (m == 2) {
        cout << n * 2 + 3 << endl;
        return 0;
    }

    // 初始化 dp_v 二維陣列，大小為 m-2 行，每行 n+1 個元素
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
