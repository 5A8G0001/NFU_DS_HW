#include <iostream>

using namespace std;

class Polynomial; // forward declaration


// Term class
class Term{
    
friend Polynomial;
friend ostream& operator<<(ostream& os, const Polynomial& poly);
friend ostream& operator>>(ostream& os, const Polynomial& poly);

private:
    float coef;
    int exp;
};

// Polynomial class
class Polynomial{

friend ostream& operator<<(ostream& os, const Polynomial& poly);
friend ostream& operator>>(ostream& os, const Polynomial& poly);

public:
    Polynomial(){
        
    }
    // default constructor

    Polynomial Add();
    // add two polynomials

    Polynomial Mult();
    // multiply two polynomials

    float Eval();
    // evaluate the polynomial at a given value


private:
    Term* termArray;
    int capacity;
    int terms;
};

ostream& operator<<(ostream& os, const Polynomial& poly){
    for(int i = 0; i < poly.terms; i++){
        os << poly.termArray[i].coef << "x^" << poly.termArray[i].exp;
        if(i != poly.terms - 1){
            os << " + ";
        }
    }
    return os;
}

ostream& operator>>(ostream& os, const Polynomial& poly){
    os << "Enter the number of terms: ";
    os >> poly.terms;
    for(int i = 0; i < poly.terms; i++){
        os >> poly.termArray[i].coef >> poly.termArray[i].exp;
    }
    return os;
}




int main()
{
    // test code
    Polynomial p1;
    cout << p1 << endl;

    return 0;
}
