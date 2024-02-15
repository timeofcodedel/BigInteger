#include "BigInteger.h"
#include<iostream>
#include<algorithm>
#include <string>
#include <regex>

using namespace std;

BigInteger::BigInteger() {
    this->bigNumStr = "";
}

BigInteger::BigInteger(const vector<int> &val) {
    this->vecNum = val;
}

BigInteger::BigInteger(const string &str) {
    if (!BigInteger::formatCorrectness(str) || (str[0] == '-' && str[1] == '0')) {
        cerr << "error:The format is entered incorrect" << endl;
        exit(-1);
    }
    this->bigNumStr = str;
    initVector();
}

BigInteger::BigInteger(const BigInteger &num1) {
    this->bigNumStr = num1.bigNumStr;
    this->vecNum = num1.vecNum;
    this->symbol = num1.symbol;
}

BigInteger::BigInteger(const long long &val) {
    if (val >= 0)
        this->symbol = 1;
    else
        this->symbol = 0;
    this->bigNumStr = to_string(val);
    initVector();
}


BigInteger::BigInteger(const long &val) {
    if (val >= 0)
        this->symbol = 1;
    else
        this->symbol = 0;
    this->bigNumStr = to_string(val);
    initVector();
}

BigInteger::BigInteger(const int &val) {
    if (val >= 0)
        this->symbol = 1;
    else
        this->symbol = 0;
    this->bigNumStr = to_string(val);
    initVector();
}

istream &operator>>(istream &is, BigInteger &integer) {
    is >> integer.bigNumStr;
    if (!BigInteger::formatCorrectness(integer) || (integer.bigNumStr[0] == '-' && integer.bigNumStr[1] == '0')) {
        cerr << "error:The format is entered incorrect" << endl;
        exit(-1);
    }
    integer.initVector();
    return is;
}

ostream &operator<<(ostream &os, const BigInteger &integer) {
    if (!integer.vecNum.empty()) {
        auto ritem = integer.vecNum.rbegin();
        while (ritem != integer.vecNum.rend() && integer.vecNum.size() != 1) {
            if (*ritem != 0) {
                break;
            }
            ritem++;
        }
        if (integer.symbol == 0)
            os << '-';
        for (; ritem != integer.vecNum.rend(); ritem++) {
            os << *ritem;
        }
        os << endl;
    }
    return os;
}

BigInteger operator+(const BigInteger &a, const BigInteger &b) {
    if (!BigInteger::isEmpty(a, b)) {
        return {};
    }
    if ((a.symbol == 0 && b.symbol == 1) || (a.symbol == 1 && b.symbol == 0)) {
        return BigInteger::subtraction(a, b);
    } else if (a.symbol == 0 && b.symbol == 0) {
        BigInteger t = BigInteger::addIngeter(a, b);
        t.symbol = 0;
        return t;
    } else {
        return BigInteger::addIngeter(a, b);
    }

}


BigInteger operator-(const BigInteger &a, const BigInteger &b) {
    if (!BigInteger::isEmpty(a, b)) {
        return {};
    }
    BigInteger t;
    if (a.symbol == 0 && b.symbol == 1) {
        t = BigInteger::addIngeter(a, b);
        t.symbol = 0;
        return t;
    } else if (a.symbol == 0 && b.symbol == 0) {
        t = BigInteger::subtraction(a, b);
        return t;
    } else if (a.symbol == 1 && b.symbol == 0) {
        return BigInteger::addIngeter(a, b);
    } else {
        return BigInteger::subtraction(a, b);
    }

}

BigInteger operator*(const BigInteger &a, const BigInteger &b) {
    if (!BigInteger::isEmpty(a, b)) {
        return {};
    }
    if ((a.vecNum[0] == 0 && a.vecNum.size() == 1) || (b.vecNum[0] == 0 && b.vecNum.size() == 1)) {
        return BigInteger(0);
    } else
        return BigInteger::multiplication(a, b);
}

BigInteger operator/(BigInteger &a, const BigInteger &b) {
    if (!BigInteger::isEmpty(a, b)) {
        return {};
    }
    if (b.vecNum[0] == 0 && b.vecNum.size() == 1) {
        cerr << "error:The dividend cannot be zero" << endl;
        exit(-1);
    } else if (a.vecNum[0] == 0 && a.vecNum.size() == 1)
        return BigInteger(0);
    else
        return BigInteger::division(a, b);
}

bool operator>=(const BigInteger &a, const BigInteger &b) {
    //这种是左正右负
    if (a.symbol == 1 && b.symbol == 0) {
        return true;
    } else if (a.symbol == 0 && b.symbol == 1) {//这种是左负右正
        return false;
    } else if ((BigInteger::compareNum(a, b) == 1 || BigInteger::compareNum(a, b) == -1) && a.symbol == 1 &&
               b.symbol == 1)//这种是ab均为正数且a>=b时(ab比较不计算正负)
        return true;
    else if (BigInteger::compareNum(a, b) == 0 && a.symbol == 1 &&
             b.symbol == 1)//这种事ab均为正数且a<b时(ab比较不计算正负)
        return false;
    else if (BigInteger::compareNum(a, b) == 1 && a.symbol == 0 && b.symbol == 0)//这种事ab均为负数且a>b时(ab比较不计算正负)
        return false;
    else if ((BigInteger::compareNum(a, b) == 0 || BigInteger::compareNum(a, b) == -1) && a.symbol == 0 &&
             b.symbol == 0)//这种是ab均为负数且a<=b时(ab比较不计算正负)
        return true;
}

bool operator<=(const BigInteger &a, const BigInteger &b) {
    //这种是左正右负
    if (a.symbol == 1 && b.symbol == 0) {
        return false;
    } else if (a.symbol == 0 && b.symbol == 1) {//这种是左负右正
        return true;
    } else if (BigInteger::compareNum(a, b) == 1 && a.symbol == 1 &&
               b.symbol == 1)//这种是ab均为正数且a>b时(ab比较不计算正负)
        return false;
    else if ((BigInteger::compareNum(a, b) == 0 || BigInteger::compareNum(a, b) == -1) && a.symbol == 1 &&
             b.symbol == 1)//这种事ab均为正数且a<=b时(ab比较不计算正负)
        return true;
    else if ((BigInteger::compareNum(a, b) == 1 || BigInteger::compareNum(a, b) == -1) && a.symbol == 0 &&
             b.symbol == 0)//这种事ab均为负数且a>=b时(ab比较不计算正负)
        return true;
    else if (BigInteger::compareNum(a, b) == 0 && a.symbol == 0 && b.symbol == 0)//这种是ab均为负数且a<b时(ab比较不计算正负)
        return false;
}

bool operator>(const BigInteger &a, const BigInteger &b) {
    if (a.symbol == 1 && b.symbol == 0) {
        return true;
    } else if (a.symbol == 0 && b.symbol == 1) {//这种是左负右正
        return false;
    } else if (BigInteger::compareNum(a, b) == 1 && a.symbol == 1 &&
               b.symbol == 1)//这种是ab均为正数且a>=b时(ab比较不计算正负)
        return true;
    else if (BigInteger::compareNum(a, b) == 0 && a.symbol == 1 &&
             b.symbol == 1)//这种事ab均为正数且a<b时(ab比较不计算正负)
        return false;
    else if (BigInteger::compareNum(a, b) == 1 && a.symbol == 0 && b.symbol == 0)//这种事ab均为负数且a>b时(ab比较不计算正负)
        return false;
    else if (BigInteger::compareNum(a, b) == 0 && a.symbol == 0 && b.symbol == 0)//这种是ab均为负数且a<=b时(ab比较不计算正负)
        return true;
    else if (BigInteger::compareNum(a, b) == -1)
        return false;
}

bool operator<(const BigInteger &a, const BigInteger &b) {
    //这种是左正右负
    if (a.symbol == 1 && b.symbol == 0) {
        return false;
    } else if (a.symbol == 0 && b.symbol == 1) {//这种是左负右正
        return true;
    } else if (BigInteger::compareNum(a, b) == 1 && a.symbol == 1 &&
               b.symbol == 1)//这种是ab均为正数且a>b时(ab比较不计算正负)
        return false;
    else if (BigInteger::compareNum(a, b) == 0 && a.symbol == 1 &&
             b.symbol == 1)//这种事ab均为正数且a<b时(ab比较不计算正负)
        return true;
    else if (BigInteger::compareNum(a, b) == 1 && a.symbol == 0 && b.symbol == 0)//这种事ab均为负数且a>b时(ab比较不计算正负)
        return true;
    else if (BigInteger::compareNum(a, b) == 0 && a.symbol == 0 && b.symbol == 0)//这种是ab均为负数且a<b时(ab比较不计算正负)
        return false;
    else if (BigInteger::compareNum(a, b) == -1)
        return false;
}

bool operator==(const BigInteger &a, const BigInteger &b) {
    if (BigInteger::compareNum(a, b) == -1)
        return true;
    return false;
}


//初始化vector容器并判断正负
void BigInteger::initVector(bool flag) {
    if (this->bigNumStr[0] == '-') {
        this->symbol = 0;
    } else {
        this->symbol = 1;
    }
    for (auto c: this->bigNumStr) {
        if (isdigit(c)) {
            this->vecNum.push_back(c - '0');
        }
    }
    if (flag)
        reverse(this->vecNum.begin(), this->vecNum.end());

}

bool BigInteger::isEmpty(const BigInteger &a, const BigInteger &b) {
    try {
        if (a.vecNum.empty() || b.vecNum.empty()) {
//            InitException e;
            throw InitException();
        }
    } catch (InitException &e) {
        cerr << e.what();
        return false;
    }
    return true;

}

int BigInteger::compareNum(const BigInteger &a, const BigInteger &b) {
    if (a.vecNum.size() > b.vecNum.size()) return 1;
    else if (a.vecNum.size() < b.vecNum.size()) return 0;
    else if (a.vecNum.size() == b.vecNum.size()) {
        for (auto item1 = a.vecNum.rbegin(), item2 = b.vecNum.rbegin();
             item1 != a.vecNum.rend() && item2 != b.vecNum.rend(); item1++, item2++) {
            if (*item1 > *item2) return 1;
            else if (*item1 < *item2)return 0;
            else continue;
        }
    }
    return -1;//表示两个数完全相等
}

BigInteger BigInteger::addIngeter(const BigInteger &a, const BigInteger &b) {
    int x = 0;
    BigInteger temp;
    unsigned long long minSize = min(a.vecNum.size(), b.vecNum.size());
    unsigned long long maxSize = max(a.vecNum.size(), b.vecNum.size());
    vector<int> mv = a.vecNum.size() > b.vecNum.size() ? a.vecNum : b.vecNum;
    for (int index = 0; index < minSize; index++) {
        temp.vecNum.push_back(a.vecNum[index] + b.vecNum[index] + x);
        x = temp.vecNum[index] / 10;
        temp.vecNum[index] %= 10;
    }
    for (auto i = minSize; i < maxSize; i++) {
        temp.vecNum.push_back(mv[i] + x);
        x = temp.vecNum[i] / 10;
        temp.vecNum[i] %= 10;
    }
    if (x != 0) {
        temp.vecNum.push_back(x);
    }
    temp.symbol = 1;
    return temp;
}

BigInteger BigInteger::subtraction(const BigInteger &a, const BigInteger &b) {
    int x = 0;
    //symbol：1为a>b，0为a<b，-1为两数相同
    int symbol = BigInteger::compareNum(a, b);
    unsigned long long minSize = min(a.vecNum.size(), b.vecNum.size());
    unsigned long long maxSize = max(a.vecNum.size(), b.vecNum.size());
    vector<int> max_long_v;
    vector<int> min_long_v;
    BigInteger temp;
    if (symbol == 1) max_long_v = a.vecNum, min_long_v = b.vecNum;
    else if (symbol == 0) max_long_v = b.vecNum, min_long_v = a.vecNum;
    else {
        temp.symbol = 1;
        temp.vecNum.push_back(0);
        return temp;
    }
    for (int i = 0; i < minSize; i++) {
        if (max_long_v[i] - x < min_long_v[i]) {
            temp.vecNum.push_back(max_long_v[i] - x + 10 - min_long_v[i]);
            x = 0;
            x++;
        } else if (max_long_v[i] - x >= min_long_v[i]) {
            temp.vecNum.push_back(max_long_v[i] - x - min_long_v[i]);
            x = 0;
        }
    }
    for (auto i = minSize; i < maxSize; i++) {
        if (max_long_v[i] - x < 0) {
            temp.vecNum.push_back(max_long_v[i] - x + 10);
            x = 0;
            x++;
        } else {
            temp.vecNum.push_back(max_long_v[i] - x);
            x = 0;
        }
    }
    //判断特例：当a为负数且a,b不算正负符号进行比较时,a>b,但实际的数为负数
    if (symbol == 1 && a.symbol == 0)
        temp.symbol = 0;
    else if ((symbol == 0 && b.symbol == 0 && a.symbol == 0) || (symbol == 0 && b.symbol == 1 && a.symbol == 0))
        temp.symbol = 1;
    else
        temp.symbol = symbol;
    for (auto rit = temp.vecNum.end() - 1; rit != temp.vecNum.begin(); rit--) {
        //cout<<*rit<<" "<<endl;
        if (*rit != 0) {
            break;
        }
        temp.vecNum.erase(rit);
    }
    return temp;
}

BigInteger BigInteger::multiplication(const BigInteger &a, const BigInteger &b) {
    int symbol;
    symbol = compareNum(a, b);
    unsigned long long minSize = min(a.vecNum.size(), b.vecNum.size());
    unsigned long long maxSize = max(a.vecNum.size(), b.vecNum.size());
    vector<int> t1(a.vecNum.size() + b.vecNum.size(), 0);
    vector<int> max_long_v;
    vector<int> min_long_v;
    if (symbol == 1) max_long_v = a.vecNum, min_long_v = b.vecNum;
    else if (symbol == 0) max_long_v = b.vecNum, min_long_v = a.vecNum;
    else {
        max_long_v = b.vecNum, min_long_v = a.vecNum;
    }
    for (int i = 0; i < minSize; i++) {
        int x = 0;
        for (int j = 0; j < maxSize; j++) {
            t1[i + j] = t1[i + j] + x + max_long_v[j] * min_long_v[i];
            x = t1[i + j] / 10;
            t1[i + j] %= 10;
        }
        t1[i + maxSize] = x;
    }
    BigInteger temp;
    temp.vecNum = t1;
    if ((a.symbol == 0 && b.symbol == 0) || (a.symbol == 1 && b.symbol == 1)) {
        temp.symbol = 1;
        return temp;
    } else {
        temp.symbol = 0;
        return temp;
    }
}

BigInteger BigInteger::division(BigInteger &a, const BigInteger &b) {

    int len = (int) (a.vecNum.size() - b.vecNum.size());
    if (len == 0) len++;
    else if (len < 0) return BigInteger(0);
    BigInteger t1;
    BigInteger a_temp = a;
    a_temp.symbol = 1;
    for (auto i = len; i > 0; i--) {
        int n = 0;
        BigInteger t = numcpy(b, i - 1);
        while (a_temp >= t) {
            n++;
            a_temp = a_temp - t;
        }
        t1.vecNum.push_back(n);
    }
    if (t1.vecNum[0] == 0 && t1.vecNum.size() == 1) {
        return BigInteger(0);
    } else if ((a.symbol == 0 && b.symbol == 1) || (a.symbol == 1 && b.symbol == 0))
        t1.symbol = 0;
    else
        t1.symbol = 1;

    reverse(t1.vecNum.begin(), t1.vecNum.end());
    return t1;
}

//len为除数的位数，a为除数
BigInteger BigInteger::numcpy(BigInteger a, size_t len) {
    //reverse(a.vecNum.begin(), a.vecNum.end());
    for (int i = 0; i < len; i++) {
        a.vecNum.insert(a.vecNum.begin(), 0);
    }
    a.symbol = 1;
    return a;
}

bool BigInteger::formatCorrectness(const BigInteger &a) {
    string regex_str = "^[-+]?[0-9]+$";
    if (!regex_match(a.bigNumStr, regex(regex_str)))
        return false;
    return true;
}

bool BigInteger::formatCorrectness(const string &str) {
    string regex_str = "^[-+]?[0-9]+$";
    if (!regex_match(str, regex(regex_str)))
        return false;
    return true;
}














