#include <cstdlib>
#include <iostream>
#include <string>
#include <cmath>    // abs()
#include <algorithm> // copy(), sort()
#include <sstream> // stringstream

using namespace std;

class Polynomial; // forward declaration


/* Node Class */
class PolynomialNode {

    friend Polynomial;
    friend ostream& operator<<(ostream& os, const Polynomial& poly);
    friend istream& operator>>(istream& is, Polynomial& poly);
    friend bool operator<(const PolynomialNode& t1, const PolynomialNode& t2);
    friend bool operator>(const PolynomialNode& t1, const PolynomialNode& t2);

public:
    PolynomialNode(float coef = 0, int exp = 0) { // constructor
        this->coef = coef;
        this->exp = exp;
        this->link = nullptr;
    }

    PolynomialNode(const PolynomialNode& t) { // copy constructor
        coef = t.coef;
        exp = t.exp;
    }

    PolynomialNode(float coef = 0, int exp = 0, PolynomialNode* link = nullptr) { // constructor
        this->coef = coef;
        this->exp = exp;
        this->link = link;
    }

private:
    float coef;
    int exp;
    PolynomialNode* link;
};

bool operator<(const PolynomialNode& t1, const PolynomialNode& t2) { // compare the exponents
    return t1.exp < t2.exp;
}

bool operator>(const PolynomialNode& t1, const PolynomialNode& t2) { // compare the exponents
    return t1.exp > t2.exp;
}

/* Polynomial Class */
class Polynomial {

    friend ostream& operator<<(ostream& os, const Polynomial& poly);
    friend istream& operator>>(istream& is, Polynomial& poly);

private:
    PolynomialNode* head;
    PolynomialNode* availableList;

    int capacity;
    int terms;
    

public:
    Polynomial() { // constructor
        availableList = nullptr;
        head = nullptr;
    }
    ~Polynomial() { // destructor
		clearList(head);
        clearAvailableList();
    }
    Polynomial(const Polynomial& poly) { // copy constructor
        head = nullptr;
        availableList = nullptr;

        PolynomialNode* temp = poly.head;
        if(temp == nullptr) return;
        do {
            InsertNode(head, temp->coef, temp->exp);
            temp = temp->link;
        } while (temp != poly.head);
    }
    PolynomialNode* allocateNode() { // allocate a new node
        if (availableList != nullptr) {
            PolynomialNode* newNode = availableList;
            availableList = availableList->link;

            newNode->coef = 0;
            newNode->exp = 0;
            newNode->link = nullptr;

            return newNode;
        }

        return new PolynomialNode(0, 0, nullptr);
    }

    void freeNode(PolynomialNode* node) { // free a node
        if (node == nullptr) return;
        node->link = availableList;
        availableList = node;
    }

    Polynomial& operator=(const Polynomial& poly) {
        if (this != &poly) { // 避免自我賦值
            clearList(head); // 清空當前多項式節點
            if (poly.head == nullptr) { // 如果來源多項式為空
                head = nullptr;
            } else {
                PolynomialNode* temp = poly.head;
                do {
                    InsertNode(head, temp->coef, temp->exp); // 使用 InsertNode 插入節點
                    temp = temp->link;
                } while (temp != poly.head); // 確保遍歷整個循環鏈結串列
            }
        }
        return *this;
    }


    Polynomial operator+(const Polynomial& poly) { // add two polynomials
        return Add(poly);
    }

    Polynomial operator-(const Polynomial& poly) { // subtract two polynomials
        return Sub(poly);
    }

    Polynomial operator*(const Polynomial& poly) { // multiply two polynomials
        return Mult(poly);
    }

    float Evaluate(float x) { // evaluate the polynomial at a given value of x
        return Eval(x);
    }


    Polynomial Add(const Polynomial& poly) { // add two polynomials
        //1 2 3 4 2 3
        //-2 1 2 5 -2 3
        Polynomial sum;
        if (head == nullptr && poly.head == nullptr) return sum;
        PolynomialNode* aPos = head;
        PolynomialNode* bPos = poly.head;
        int count = 0;//

        bool aComplete = false;
        bool bComplete = false;

        do {
            count++;//
            if(aComplete || bComplete) {
                if(aComplete) {
                    sum.InsertNode(sum.head, bPos->coef, bPos->exp);
                    bPos = bPos->link;
                    if (bPos == poly.head) bComplete = true; // 標記 bPos 已完成一圈
                } else {
                    sum.InsertNode(sum.head, aPos->coef, aPos->exp);
                    aPos = aPos->link;
                    if (aPos == head) aComplete = true; // 標記 aPos 已完成一圈
                }
                continue;
            }
            if (aPos->exp == bPos->exp) {
                float t = aPos->coef + bPos->coef;
                if (t != 0)
                {
                    sum.InsertNode(sum.head, t, aPos->exp);
                }
                aPos = aPos->link;
                bPos = bPos->link;
                if (aPos == head) aComplete = true; // 標記 aPos 已完成一圈
                if (bPos == poly.head) bComplete = true; // 標記 bPos 已完成一圈
            } else if (aPos->exp > bPos->exp) {
                sum.InsertNode(sum.head, aPos->coef, aPos->exp);
                aPos = aPos->link;
                if (aPos == head) aComplete = true; // 標記 aPos 已完成一圈
            } else {
                sum.InsertNode(sum.head, bPos->coef, bPos->exp);
                bPos = bPos->link;
                if (bPos == poly.head) bComplete = true; // 標記 bPos 已完成一圈
            }
        } while (!aComplete || !bComplete);

        cout << "Number of Add comparisons: " << count << endl;//

        return sum;
    }

    Polynomial Sub(const Polynomial& poly) { // subtract two polynomials
        Polynomial sum;
        if (head == nullptr && poly.head == nullptr) return sum;
        PolynomialNode* aPos = head;
        PolynomialNode* bPos = poly.head;
        int count = 0;//

        bool aComplete = false;
        bool bComplete = false;

        do {
            count++;//
            if(aComplete || bComplete) {
                if(aComplete) {
                    sum.InsertNode(sum.head, -bPos->coef, bPos->exp);
                    bPos = bPos->link;
                    if (bPos == poly.head) bComplete = true; // 標記 bPos 已完成一圈
                } else {
                    sum.InsertNode(sum.head, aPos->coef, aPos->exp);
                    aPos = aPos->link;
                    if (aPos == head) aComplete = true; // 標記 aPos 已完成一圈
                }
                continue;
            }
            if (aPos->exp == bPos->exp) {
                float t = aPos->coef - bPos->coef;
                if (t != 0)
                {
                    sum.InsertNode(sum.head, t, aPos->exp);
                }
                aPos = aPos->link;
                bPos = bPos->link;
                if (aPos == head) aComplete = true; // 標記 aPos 已完成一圈
                if (bPos == poly.head) bComplete = true; // 標記 bPos 已完成一圈
            } else if (aPos->exp < bPos->exp) {
                sum.InsertNode(sum.head, -bPos->coef, bPos->exp);
                bPos = bPos->link;
                if (bPos == poly.head) bComplete = true; // 標記 aPos 已完成一圈
            } else {
                sum.InsertNode(sum.head, aPos->coef, aPos->exp);
                aPos = aPos->link;
                if (aPos == head) aComplete = true; // 標記 bPos 已完成一圈
            }
        } while (!aComplete || !bComplete);

        cout << "Number of Sub comparisons: " << count << endl;//

        return sum;
    }

    Polynomial Mult(const Polynomial& poly) { // multiply two polynomials
        Polynomial product;
        if (head == nullptr || poly.head == nullptr) return product;
        PolynomialNode* aPos = head;
        
        int count = 0;// 

        do {
            count++;
            PolynomialNode* bPos = poly.head;
            do {
                count++;//
                float t = aPos->coef * bPos->coef;
                int e = aPos->exp + bPos->exp;
                product.InsertNode(product.head, t, e);
                bPos = bPos->link;
            } while (bPos != poly.head);
            aPos = aPos->link;
        } while (aPos != head);

        cout << "Number of Mult comparisons: " << count << endl;//

        return product;

    }

    PolynomialNode* InsertNode(PolynomialNode*& head, float coef, int exp) {
        PolynomialNode* newNode = allocateNode();
        newNode->coef = coef;
        newNode->exp = exp;
        int count = 1;

        if (head == nullptr) { // 如果頭為空
            newNode->link = newNode; // 指向自身形成環
            head = newNode;
        } else {
            PolynomialNode* prev = nullptr;
            PolynomialNode* curr = head;

            // 尋找合適的位置插入
            do {
                count++;
                if (curr->exp <= exp) break; // 找到插入點
                prev = curr;
                curr = curr->link;
            } while (curr != head);

            if (curr->exp == exp) { // 指數相同，合併係數
                curr->coef += coef;
                if (curr->coef == 0) { // 如果係數為 0，刪除節點
                    if (curr == head && curr->link == head) { // 只有一個節點
                        freeNode(curr);
                        head = nullptr;
                    } else {
                        prev->link = curr->link;
                        if (curr == head) head = curr->link; // 如果是頭節點，更新頭指針
                        freeNode(curr);
                    }
                }
            } else { // 插入新節點
                newNode->link = curr;
                if (prev != nullptr) {
                    prev->link = newNode;
                } else { // 插入到頭部
                    // 找到尾節點並更新環
                    PolynomialNode* tail = head;
                    while (tail->link != head) {
                        count++;
                        tail = tail->link;
                    }
                    tail->link = newNode;
                    head = newNode; // 更新頭指針
                }
            }
        }
        cout << "Number of Insert comparisons: " << count << endl;

        return head;
    }


    void clearList(PolynomialNode*& head) {
        if (head == nullptr) return;

        PolynomialNode* curr = head;
        do {
            PolynomialNode* temp = curr;
            curr = curr->link;
            freeNode(temp);
        } while (curr != head);

        head = nullptr; // 清空後設置頭指針為空
    }


    void clearAvailableList() {
        while (availableList != nullptr) {
            PolynomialNode* temp = availableList;
            availableList = availableList->link;
            delete temp; // 釋放節點
        }
    }

    float Eval(float x) { // evaluate the polynomial at a given value of x
        float sum = 0;
        int count = 0;//
        PolynomialNode* Thead = this->head;
        if(Thead == nullptr) return 0;
        do
        {
            count++;//
            sum += Thead->coef * pow(x, Thead->exp);
            Thead = Thead->link;
        } while (Thead != this->head);

        cout << "Number of Eval comparisons: " << count << endl;//
        return sum;
    }

};

ostream& operator<<(ostream& os, const Polynomial& poly) {
    if (poly.head == nullptr) {
        os << "0";
        return os;
    }

    PolynomialNode* current = poly.head;
    bool isFirstTerm = true;

    do {
        if (isFirstTerm) {
            if (current->coef < 0) os << "-";
            os << abs(current->coef);
        } else {
            if (current->coef < 0) os << " - ";
            else os << " + ";
            os << abs(current->coef);
        }

        if (current->exp != 0) {
            os << "x";
            if (current->exp != 1) os << "^" << current->exp;
        }

        current = current->link;
        isFirstTerm = false;
    } while (current != poly.head);

    return os;
}




istream& operator>>(istream& is, Polynomial& poly) { // second version -> 以空白分隔 5x+2x^2+3x^3 == 5 1 2 2 3 3

    PolynomialNode* head = nullptr;
    float coef;
    int exp;


    cout << "Enter terms (coef exp), one pair per term, and press Enter to finish:\n";

    string line;
    getline(cin, line); // 讀取整行輸入

    if (line.empty()) {
        poly.head = nullptr;
        return is;
    }
    stringstream ss(line); // 用stringstream解析這行輸入

    while (ss >> coef >> exp) { // 從解析流中提取係數和指數
        head = poly.InsertNode(head, coef, exp);
    }

    // 清除原本的項
    poly.clearList(poly.head);

    // 將合併後的項放入poly
    poly.head = head;

    return is;
}

int main()
{
    Polynomial p1, p2;

    cin >> p1 >> p2;

    cout << "Polynomial 1: " << p1 << endl;
    cout << "Polynomial 2: " << p2 << endl;

    Polynomial p3 = p1 + p2;
    cout << "Sum: " << p3 << endl;

    Polynomial p4 = p1 - p2;
    cout << "Difference: " << p4 << endl;

    Polynomial p5 = p1 * p2;
    cout << "Product: " << p5 << endl;
    p1 = Polynomial(p5);
    cout << "p1 = p5: " << p1 << endl;
    cout << "Evaluation of p1 at x=2: " << p1.Evaluate(2) << endl;


    system("pause");

    return 0;
}