#include <cstdlib>
#include <iostream>
#include <string>
#include <cmath>    // abs()
#include <algorithm> // copy(), sort()
#include <sstream> // stringstream

using namespace std;

class Polynomial; // forward declaration


/* Term Class */
class Term{

friend Polynomial;
friend ostream& operator<<(ostream& os, const Polynomial& poly);
friend istream& operator>>(istream& is, Polynomial& poly);
friend bool operator<(const Term& t1, const Term& t2);
friend bool operator>(const Term& t1, const Term& t2);

public:
    Term(float coef = 0,int exp = 0){ // constructor
        this->coef = coef;
        this->exp = exp;
    }

private:
    float coef;
    int exp;
};

bool operator<(const Term& t1, const Term& t2) { // compare the exponents
    return t1.exp < t2.exp;
}

bool operator>(const Term& t1, const Term& t2) { // compare the exponents
    return t1.exp > t2.exp;
}

/* Polynomial Class */
class Polynomial{

friend ostream& operator<<(ostream& os, const Polynomial& poly);
friend istream& operator>>(istream& is, Polynomial& poly);

public:
    Polynomial(){ // constructor
        capacity = 8;
        termArray = new Term[capacity];
        terms = 0;
    }
    ~Polynomial(){ // destructor
        delete[] termArray;
    }
    
    Polynomial(const Polynomial& poly){ // copy constructor
        capacity = poly.capacity;
        terms = poly.terms;
        termArray = new Term[capacity];
        copy(poly.termArray,poly.termArray+terms,termArray);
    }

    // sort and remove the same exponent
    void sortAndRemove(){ // 小到大排序並移除相同指數的項
        sort(termArray,termArray+terms,[](Term t1,Term t2){return t1.exp > t2.exp;}); // sort the terms by the exponent
        int count = 0;
        for(int i = 0; i < terms; i++){
            count++;
            for(int j = i+1; j < terms; j++){
                count++;
                if(termArray[i].exp == termArray[j].exp){
                    termArray[i].coef += termArray[j].coef;
                    for(int k = j; k < terms - 1; k++){
                        count++;
                        termArray[k] = termArray[k+1];
                    }
                    count++;
                    terms--;
                }
            }
            count++;
        }
        count++;
        cout << "Number of remove comparisons: " << count << endl;
    }

    Polynomial Add(Polynomial poly){ // add two polynomials

        Polynomial sum;
        int aPos = 0, bPos = 0; // position of the terms in the two polynomials
        int count = 0;
        while((aPos < terms) && (bPos < poly.terms)){
            count++; 
            if(termArray[aPos].exp == poly.termArray[bPos].exp){ // 如果兩個項的指數相同
                float t = termArray[aPos].coef + poly.termArray[bPos].coef; 
                if(t) sum.NewTerm(t,termArray[aPos].exp); // 如果t不為0,則加入到sum中
                aPos++; bPos++;
            }
            else if(termArray[aPos].exp < poly.termArray[bPos].exp){ // 如果第一個項的指數小於第二個項的指數
                sum.NewTerm(poly.termArray[bPos].coef,poly.termArray[bPos].exp); // 將第二個項加入到sum中
                bPos++;
            }
            else{
                sum.NewTerm(termArray[aPos].coef,termArray[aPos].exp); // 將第一個項加入到sum中
                aPos++;
            }
        }

        cout << "Number of Add comparisons: " << count << endl;
        for(;aPos < terms;aPos++){ // add the remaining terms
            sum.NewTerm(termArray[aPos].coef,termArray[aPos].exp);
        }
        for(;bPos < poly.terms;bPos++){ // add the remaining terms
            sum.NewTerm(poly.termArray[bPos].coef,poly.termArray[bPos].exp);
        }

        sum.sortAndRemove();

        return sum;

    }

    Polynomial Mult(Polynomial poly){ // multiply two polynomials
        Polynomial product;
        int count = 0;
        // multiply each term of the first polynomial with each term of the second polynomial
        for(int i = 0; i < terms; i++){ 
            count++;
            for(int j = 0; j < poly.terms; j++){
                count++;
                float t = termArray[i].coef * poly.termArray[j].coef;
                int e = termArray[i].exp + poly.termArray[j].exp;
                product.NewTerm(t,e);
            }
        }
        cout << "Number of Mult comparisons: " << count << endl;
        product.sortAndRemove();

        return product;

    }
    

    float Eval(float x) { // evaluate the polynomial at a given value of x
        float sum = 0;
        int count = 0;
        for(int i = 0; i < terms; i++){
            count++;
            sum += termArray[i].coef * pow(x,termArray[i].exp);
        }
        cout << "Number of Eval comparisons: " << count << endl;
        return sum;
    }
    
    
    void NewTerm(float theCoeff,int theExp) { // add a new term to the polynomial
        if(terms == capacity) // if the array is full
        {
            capacity *= 2;
            Term *temp = new Term[capacity];
            copy(termArray,termArray+terms,temp);
            delete[] termArray;
            termArray = temp;
        }
        termArray[terms].coef = theCoeff;
        termArray[terms++].exp = theExp;
    }


private:
    Term* termArray;
    int capacity;
    int terms;
};

ostream& operator<<(ostream& os, const Polynomial& poly){
    if(poly.terms == 0){
        os << "0";
        return os;
    }

    if(poly.termArray[0].coef < 0){
        os << "-"; // if the first term is negative
    }
    for(int i = 0; i < poly.terms; i++){
        os << abs(poly.termArray[i].coef); // abs() to get the absolute value

        if(poly.termArray[i].exp != 0){
            os << "x";
            if(poly.termArray[i].exp != 1){
                os << "^" << poly.termArray[i].exp;
            }
        }

        if(i != poly.terms - 1){
            if(poly.termArray[i+1].coef < 0){
                os << " - ";
            }
            else{
                os << " + ";
            }
        }
    }
    return os;
}



istream& operator>>(istream& is, Polynomial& poly){ // second version -> 以空白分隔 5x+2x^2+3x^3 == 5 1 2 2 3 3
    /*
        使用陣列實作，先對輸入的項數進行排序，再合併同樣指數的項，最後將合併後的項放入poly，可以減少排序和合併的時間複雜度
    */
    
    int tSize = 8;
    Term *tArr = new Term[tSize];

    float coef;
    int exp;

   
    cout << "Enter terms (coef exp), one pair per term, and press Enter to finish:\n";

    string line;
    getline(cin, line); // 讀取整行輸入
    stringstream ss(line); // 用stringstream解析這行輸入

    int i = 0;
    while (ss >> coef >> exp) { // 從解析流中提取係數和指數
        if (i == tSize) { // 如果陣列不夠大,擴大陣列
            tSize *= 2;
            Term *newArr = new Term[tSize];
            for (int j = 0; j < i; j++) {
                newArr[j] = tArr[j];
            }
            delete[] tArr;
            tArr = newArr;
        }
        tArr[i] = Term(coef, exp);
        i++;
    }
    // 將指數由小到大排序,coef也要跟著移動
    sort(tArr, tArr+i, [](Term t1, Term t2) { return t1 < t2; });

    // 合併同樣指數的項
    int j = 0;
    for (int k = 1; k < i; k++) { // k是目前的位置
        if (tArr[k].exp == tArr[j].exp) { // 如果指數相同,合併
            tArr[j].coef += tArr[k].coef;
        } else { // 否則,將k的項移到j+1的位置
            j++;
            tArr[j] = tArr[k];
        }
    }

    // 將合併後的項放入poly
    poly.terms = j+1; // 有j+1個項
    poly.capacity = tSize; 
    delete[] poly.termArray; // 刪除原本的項
    poly.termArray = new Term[tSize];
    for (int k = 0; k <= j; k++) { 
        poly.termArray[k] = tArr[k];
    }

    return is;
}

int main()
{
    /*
    // input output test
    Polynomial p;
    cin >> p;
    cout << p << endl;
    */

    
    // test code
    Polynomial p1,p2;

    cin >> p1 >> p2;
    cout << p1.Add(p2) << endl;

    cin >> p1 >> p2;
    cout << p1.Mult(p2) << endl;


    Polynomial p;
    cin >> p;
    cout << p.Eval(2) << endl;
    
    system("pause");

    return 0;
}
