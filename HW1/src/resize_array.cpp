#include <iostream>

using namespace std;


/*

new_size:resize_array函數是為了在ackermann function在求A(m,n)時，由於A(m,n)需要A(m-1,A(m-1,n))

如要求A(3,1)=A(2,5)，因為A(3,1)=A(3-1,A(3-1,1)),A(2,A(2,1))=A(2,5),而A(2,5)代表需要長度為5的陣列(假設不使用公式來得到A(2,n)的結果)

new_size就是這個A(2,1)的結果，使用動態查表的情況下，A(2,1)已經存在，則透過resize將陣列擴增為5，以此得出A(2,5)的結果，這個概念亦可以用在m=3 or m=4

*/

long long* resize_array(long long* old_array, int old_size, int new_size) {

    // 配置新的陣列大小
    long long* new_array = new long long[new_size];

    // 將舊陣列內容複製到新陣列
    for (int i = 0; i < old_size; i++) {
        new_array[i] = old_array[i];
    }

    // 將剩餘部分初始化為 -1
    for (int i = old_size; i < new_size; i++) {
        new_array[i] = -1;
    }

    // 釋放舊陣列記憶體
    delete[] old_array;

    return new_array;  // 返回新陣列的指針
}

int main(){



}
