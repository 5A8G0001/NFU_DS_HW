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