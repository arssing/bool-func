#pragma once
#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include <list>
using namespace std;
using BASE = unsigned int;

class BF {
    private:
        //len - число переменных, len_ - длина массива BF_
        unsigned int len, len_;
        BASE *BF_;
    public:
        BF(){len = 0; len_ = 0; BF_ = new BASE[len_];};
        BF(int count, int type);
        BF(const BF &);
        BF(string s);

        friend std::ostream &operator <<(ostream&, const BF &);
        BF& operator =(const BF&);
        bool operator ==(BF&);

        int weight();

        ~BF(){delete [] BF_;};

        BF Mebius();
        void toMebius(int, int, unsigned int[]);
        void Mebius32bits(int, int, unsigned int[]);

        list<unsigned int> ANF();
        void printANF();

        list<unsigned int> dummyVariables();
        list<unsigned int> linearVariables();

        int deg();
        bool isLinear();
        bool isAffine();
};