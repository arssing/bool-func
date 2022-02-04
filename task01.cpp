#include "BF.h"

int main(){

    BF c("00011110");
    BF b1("1010010110100101101001011010010110100101101001011010010110100101");

    cout << "c_ANF = ";
    c.printANF();
    cout << "b1_ANF = ";
    b1.printANF();

    
    list<unsigned int> linVarc = c.linearVariables();
    list<unsigned int> dumVarb1 = b1.dummyVariables();
    list<unsigned int> linVarb1 = b1.linearVariables();

    cout << "\nЛинейные переменные функции c\n";
    string out;
    for (auto it = linVarc.begin(); it != linVarc.end(); it++){
        out += 'x' + to_string(*(it)) + ',';
    }
    out = out.substr(0, out.size()-1);
    cout << out;

    out = "";
    cout << "\nФиктивные переменные функции b1\n";
    for (auto it = dumVarb1.begin(); it != dumVarb1.end(); it++){
        out += 'x' + to_string(*(it)) + ',';
    }
    out = out.substr(0, out.size()-1);
    cout << out;

    out = "";
    cout << "\nЛинейные переменные функции b1\n";
    for (auto it = linVarb1.begin(); it != linVarb1.end(); it++){
        out += 'x' + to_string(*(it)) + ',';
    }
    out = out.substr(0, out.size()-1);
    cout << out;
    
    if (b1.isLinear()){
        cout << "\nфункция является линейной\n";
    } 
    else{
        cout << "\nфункция не является линейной\n";
    }
    if (b1.isAffine()){
        cout << "\nфункция является аффинной\n";
    } 
    else{
        cout << "\nфункция не является аффинной\n";
    }

    /*  проверка работы функции Мебиуса
    BF Meb;

    for (int i = 2; i <= 30; i++){
        BF test(i, 2);

        Meb = test.Mebius();
        Meb = Meb.Mebius();

        if (Meb == test){
            cout << i << " - удачно\n";
            continue;
        }
        cout << i << " - ошибка\n";
        break;
    }
    */

    return 0;
}