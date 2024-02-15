#ifndef HIGH_PRECISION_BIGINTEGER_H
#define HIGH_PRECISION_BIGINTEGER_H

#include <string>
#include <vector>
#include <ostream>
#include <istream>
using namespace std;

class BigInteger {
private:
    int symbol{};
    string bigNumStr;
    vector<int> vecNum;
    explicit BigInteger(const vector<int>&val);
    static bool formatCorrectness(const BigInteger& a);
    static bool formatCorrectness(const string& str);
    static BigInteger numcpy(BigInteger a,size_t len);
    static BigInteger addIngeter(const BigInteger &a, const BigInteger &b) ;
    static BigInteger subtraction(const BigInteger &a, const BigInteger &b);
    static BigInteger multiplication(const BigInteger &a, const BigInteger &b);
    static BigInteger division( BigInteger &a, const BigInteger &b);
public:
    BigInteger();
    BigInteger(BigInteger const &val);
    explicit BigInteger(const long long &val);
    explicit BigInteger(const long &val);
    explicit BigInteger(const int &val);
    explicit BigInteger(const string &str);
    void initVector(bool flag=true);
    static bool isEmpty(const BigInteger &a, const BigInteger &b);
    static int compareNum(const BigInteger &a, const BigInteger &b);
    friend ostream &operator<<(ostream &os, const BigInteger &integer);
    friend istream &operator>>(istream &is, BigInteger &integer);
    friend BigInteger operator+(const BigInteger &a,const BigInteger &b);
    friend BigInteger operator-(const BigInteger &a,const BigInteger &b);
    friend BigInteger operator*(const BigInteger &a,const BigInteger &b);
    friend BigInteger operator/( BigInteger &a,const BigInteger &b);
    friend bool operator>=(const BigInteger &a,const BigInteger &b);
    friend bool operator<=(const BigInteger &a,const BigInteger &b);
    friend bool operator>(const BigInteger &a,const BigInteger &b);
    friend bool operator<(const BigInteger &a,const BigInteger &b);
    friend bool operator==(const BigInteger &a,const BigInteger &b);
};
class InitException : public exception
{
public:
    [[nodiscard]] const char *what() const noexcept override
    {
        return "error:string is empty and cannot be evaluated";
    }
};



#endif
