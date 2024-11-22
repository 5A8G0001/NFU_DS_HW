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
    // default constructor

    Polynomial Add(Polynomial poly){

        Polynomial sum;
        int aPos = 0, bPos = 0;
        while((aPos < terms) && (bPos < poly.terms)){
            if(termArray[aPos].exp == poly.termArray[bPos].exp){
                float t = termArray[aPos].coef + poly.termArray[bPos].coef;
                if(t) sum.NewTerm(t,termArray[aPos].exp);
                aPos++; bPos++;
            }
            else if(termArray[aPos].exp < poly.termArray[bPos].exp){
                sum.NewTerm(poly.termArray[bPos].coef,poly.termArray[bPos].exp);
                bPos++;
            }
            else{
                sum.NewTerm(termArray[aPos].coef,termArray[aPos].exp);
                aPos++;
            }
        }

        for(;aPos < terms;aPos++){
            sum.NewTerm(termArray[aPos].coef,termArray[aPos].exp);
        }
        for(;bPos < poly.terms;bPos++){
            sum.NewTerm(poly.termArray[bPos].coef,poly.termArray[bPos].exp);
        }

        return sum;

    }
    // add two polynomials

    Polynomial Mult();
    // multiply two polynomials

    float Eval(float x) { // evaluate the polynomial at a given value of x
        float sum = 0;
        for(int i = 0; i < terms; i++){
            sum += termArray[i].coef * pow(x,termArray[i].exp);
        }
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
    
    if(poly.termArray[0].coef < 0){
        os << "-"; // if the first term is negative
    }

    for(int i = 0; i < poly.terms; i++){
        os << abs(poly.termArray[i].coef) << "x^" << poly.termArray[i].exp; // abs() to get the absolute value
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

/*
istream& operator>>(istream& is, Polynomial& poly){ // first version -> read the polynomial from the input(字串) 5x+2x^2+3x^3 == 5x1+2x2+3x3 
    cout << "Enter the number of terms: aix0+aix1-aix2,,3x0+2x3+1x3" << endl;
    
    string str;
    getline(cin,str);
    //cout << str << endl;
    int i = 0;


    
    while(i < str.length()){
        int coef = 0;
        int exp = 0;

        // check if the first term is negative
        if(str[i] == '-'){
            i++;
            coef = -1;
        }
        else if(str[i] == '+'){
            i++;
            coef = 1;
        }
        else{ // the first term is positive
            coef = 1;
        }

        // get the coefficient
        coef *= str[i] - '0';
        i++;
        i++; // skip 'x'
        // get the exponent
        exp = str[i] - '0';

        poly.NewTerm(coef,exp);
        i++;
    }

    // sort the polynomial
    for(int i = 0; i < poly.terms; i++){
        for(int j = i+1; j < poly.terms; j++){
            if(poly.termArray[i].exp < poly.termArray[j].exp){
                Term temp = poly.termArray[i];
                poly.termArray[i] = poly.termArray[j];
                poly.termArray[j] = temp;
            }
        }
    }

    // remove the same exponent
    for(int i = 0; i < poly.terms; i++){
        for(int j = i+1; j < poly.terms; j++){ // compare the i-th term with the j-th term
            if(poly.termArray[i].exp == poly.termArray[j].exp){ // if the exponents are the same
                poly.termArray[i].coef += poly.termArray[j].coef;
                for(int k = j; k < poly.terms - 1; k++){ // remove the j-th term
                    poly.termArray[k] = poly.termArray[k+1];
                }
                poly.terms--;
            }
        }
    }

    return is;
}*/




istream& operator>>(istream& is, Polynomial& poly){ // second version -> 以空白分隔 5x+2x^2+3x^3 == 5 1 2 2 3 3
    
    
    int tSize = 8;
    Term *tArr = new Term[tSize];

    cout << "Enter the number of terms: ";
    
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
    sort(tArr, tArr+i);

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


// operator overloading =
/*
Polynomial& operator=(const Polynomial& poly){
    if(this == &poly) return *this;
    delete[] termArray;
    terms = poly.terms;
    capacity = poly.capacity;
    termArray = new Term[capacity];
    copy(poly.termArray,poly.termArray+terms,termArray);
    return *this;
}
*/


int main()
{
    // test code
    Polynomial p1,p2;
    //Term  t1(1,0),t2(2,1),t3(3,2);

    //p1.NewTerm(1,0);p1.NewTerm(2,1);p1.NewTerm(3,2);

    //cin >> p1 >> p2;
    //cout << p1 << endl << p2 << endl;

    cin >> p1 >> p2;
    //cout << p1 << endl;
    //cout<< "val:" << p1.Eval(2) << endl;
    
    cout << p1.Add(p2) << endl;

    return 0;
}
