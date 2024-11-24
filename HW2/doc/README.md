# HW 2-Polynomial

## 1. 解題說明

基於題目提供之ADT實作多項式類別(`Polynomial`)與`Term`類別，根據題目要求，多項式類別提供以下功能：

### Method

建構函數:

多項式相加(`Add`):

多項式相乘(`Mult`):

計算多項式結果(`Eval`):

新增非0項次(`NewTerm`):

### Operator overloading

輸入運算子(`<<`)

輸出運算子(`>>`)

對於`Term`類別，為了排序，也對其`>`、`<`實作Operator overloading

### 實作邏輯

1. 使用類別 `Term` 來表示多項式中的每一項，包括係數 (coef) 和指數 (exp)

2. 在 `Polynomial` 類別中維護一個動態陣列 `termArray` 來儲存所有的 `Term`，並設計動態調整容量
3. 針對加法與乘法運算，遍歷兩個多項式的項目進行計算，結果儲存於新的 `Polynomial` 物件中，最後排序並合併同指數項
4. 在輸入時，將依照指數由小到大排序。加法與乘法運算後的結果則是由大到小排序
5. 在輸出時，對於指數為0或是為1的項次特別處理顯示

## 2. Algorithm Design &  Programming

### sort

```cpp

```

## 3. 效能分析/Analysis

### 3-1. Time complexity

#### `sortAndRemove` 函數

1. 使用 `std::sort` 排序陣列，時間複雜度為 $O(n \log n)$
2. 合併同指數的項時，最壞情況需要 $O(n^2)$（當所有項的指數相同且需要進行大量位移時）
3. 綜合為$O(n^2)$

#### 加法與乘法 (`Add` 和 `Mult`)

1. 加法需遍歷兩個多項式的所有項，時間複雜度為 $O(m+n)$ ( $m$ 與 $n$ 為兩個多項式的項數，也就是 `terms`)
2. 乘法需進行兩層巢狀迴圈以計算所有項的乘積，時間複雜度為 $O(m \times n)$

#### 計算函數(`Eval`)

1. 遍歷多項式中的所有項，時間複雜度為 $O(n)$

### 3-2. Space complexity

1. 動態陣列`termArray`佔用空間為 $O(n)$ ，其中 $n$ 為多項式的項數，也就是 `terms`

2. 加法和乘法的結果需要新的`Polynomial`物件，最壞情況下需額外的 $O(m+n)$ 或 $O(m \times n)$ 空間

## 4. 測試與驗證(Testing and Proving)

### 測試案例

#### 多項式加法

Input: `p1 = 3x^2 + 2x + 1`，`p2 = x^2 + x + 4`

cmd:`3 2 2 1 1 0` `1 2 1 1 4 0`

Output:`4x^2 + 3x + 5`

#### 多項式乘法

Input: `p1 = 2x + 1`，`p2 = x + 3`

cmd:`2 1 1 0` `1 1 3 0`

Output:`2x^2 + 7x + 3`

#### 評估多項式

Input: `p = 3x^2 + 2x + 1`，`p.Eval(x = 2)`

cmd:`3 2 2 1 1 0`

Output:`17`

### TestImg

![test_img](./test_img.png "TestImg")


## 5. 效能量測 (Measuring)

## 6. 心得討論