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

    return 0;
}
