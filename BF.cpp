#include "BF.h"

BF::BF(int count, int type){
    int bytes_in_BASE = sizeof(BASE)*8;
    BASE len_out = 1 << count;

    len = count;
    len_ = (int)ceil((float)len_out/bytes_in_BASE);
    BF_ = new BASE[len_];


    switch (type){
        case 0:
        {
            for(int i = 0; i < len_; i++){
                BF_[i] = 0;
            }
            break;
        }
        case 1:
        {
            if (len_out < bytes_in_BASE){

                int k = bytes_in_BASE - 1;
                BASE mask = 0;
                for (int i = 0; i < len_out; i++, k--){
                    mask = mask | 1 << k;
                }
                BF_[0] = mask;
        

            } else {
                BASE init = 0;
                init = ~init;
                for(int i = 0; i < len_; i++){
                    BF_[i] = init;
                }
    
            }
            break;
        }
        case 2:
        {   
            
            if (len_out < bytes_in_BASE){
                int k = bytes_in_BASE - 1;
                BASE mask = 0;
                for (int i = 0; i < len_out; i++, k--){
                    mask |= (rand() & 1) << k;
                }         
                BF_[0] = mask;

            } else {
                BASE mask;
                for(int i=0; i < len_; i++){
                    mask = 0;
                    for (int j=0; j < sizeof(BASE); j++){
                        mask |= (rand() & 255) << j*8;
                    }
                    BF_[i] = mask;
                }
            }

            break;
        }
    }
}

BF::BF(const BF &cop){
    len = cop.len;
    len_ = cop.len_;
    BF_ = new BASE[len_];

    for (int i = 0; i < len_; i++){
        BF_[i] = cop.BF_[i];
    }
}

BF & BF::operator=(const BF &v){
    if(this != &v){
        delete [] BF_;
        len = v.len;
        len_ = v.len_;
        BF_ = new BASE[len_];
        for(int i = 0; i < len_; i++){
            BF_[i] = v.BF_[i];
        }
    }

    return *this;
}

bool BF::operator ==(BF &v){
    if (len != v.len){
        return false;
    }
    
    for (int i = 0; i<len_; i++){
        if (BF_[i] == v.BF_[i]){
            continue;
        } else {
            return false;
        }
    }
    return true;
}

BF::BF(string s){
    int bytes_in_BASE = sizeof(BASE)*8;
    int j = bytes_in_BASE - 1;
    unsigned int len_str = s.length();
    BASE mask = 0;

    if (len_str!=0 && (len_str & (len_str-1)) == 0){

        len_ = (int)ceil((float)len_str/bytes_in_BASE);
        BF_ = new BASE[len_];

        len = 0;
        unsigned int len_copy = len_str;
        len_copy = len_copy >> 1;
        while (len_copy){
            len++;
            len_copy = len_copy >> 1;
        }

        for(int i=0,k=0; i < len_str; i++){
            mask = mask | ((int)s[i]-'0') << j;
            j--;

            if (j < 0 || i==len_str-1){
                BF_[k] = mask;
                
                mask = 0;
                j = bytes_in_BASE - 1;
                k++;
            }
        }

    } else {
        BF_ = new BASE[0];
        cout << "ошибка: кол-во элементов в строке не степень двойки\n";
    }

}


int BF::weight(){
    BF b = *this;
    int n = 0;
    int i = 0;

    while (b.BF_[i] != 0 && i < b.len_){
        b.BF_[i] = b.BF_[i] & (b.BF_[i] - 1);
        n++;
        if (b.BF_[i] == 0 && i < b.len_) i++;
    }
    return n;
}

std::ostream &operator <<(ostream &out, const BF &v){
    int bytes_in_BASE = sizeof(BASE)*8;
    int index;
    BASE mask = 0;
    BASE len_out = 1 << v.len;
    
    mask = mask | 1 << (bytes_in_BASE-1);

    for (int i = 0; i < len_out; i++){
        index = (int)floor((float)i/bytes_in_BASE);

        if (v.BF_[index] & mask){
            cout << '1';
        } else {
            cout << '0';
        }

        mask = mask >> 1;
        if (mask == 0){
            mask = mask | 1 << (bytes_in_BASE-1);
        }
    }
    cout << '\n';
    return out;
}

BF BF::Mebius(){
    BF b = *this;
    //если кол-во переменных меньше 5, то работаем с 1 ячейкой массива
    if (b.len <= 5){
        Mebius32bits(0, b.len, b.BF_);
        return b;
    }
    
    toMebius(0, b.len_, b.BF_);
    return b;
}
// toMebius - рекурсивная функция вычисления преобразования Мёбиуса
// Если рассматриваем массив из 1 элемента, то выход.
// Иначе, находим индекс середины массива и количество шагов на данном этапе рекурсии.
// Рекурсивно вызываем для первой и второй половины массива.
// На "дне" рекурсии - xor первой половины со второй.
void BF::toMebius(int start, int end, unsigned int arr[]){
    //если рассматриваем отдельную ячейку 32 бита, то применяем к ней преобразование Мёбиуса
    if ( start==end-1 ){
        Mebius32bits(start, 5, arr);
        return;
    }

    int mid = ((start + end) >> 1);
    int num_steps = ((end-start) >> 1);

    toMebius(start, mid, arr);
    toMebius(mid, end, arr);
    for (int i = 0; i < num_steps; i++){
        arr[mid+i] ^= arr[start+i]; 
    }


}

void BF::Mebius32bits(int index, int num_steps, unsigned int arr[]){
    unsigned int mask;
    unsigned int shift = 1;

    unsigned int masks[5] = {
        0b01010101010101010101010101010101,
        0b00110011001100110011001100110011,
        0b00001111000011110000111100001111,
        0b00000000111111110000000011111111,
        0b00000000000000001111111111111111
    };

    for (int i=0; i<num_steps;i++){
            mask = masks[i];
            arr[index] ^= (arr[index] >> shift) & mask; 
            shift <<= 1;
    }
}
// АНФ
// Используем преобразование Мёбиуса, чтобы найти АНФ:
// идем по массиву, если очередной бит равен 1, то заносим его номер в result
list<unsigned int> BF::ANF(){

    BF b = Mebius();

    list<unsigned int> result;
    unsigned int count = 0;

    for (int i = 0; i < len_; i++){
        for (int j = sizeof(BASE)*8-1; j>=0; j--){

            if ((b.BF_[i] >> j) & 1 == 1){
                result.push_back(count);
            }
            count++;
        }
    }

    return result;
}

void BF::printANF(){
    string out;
    list<unsigned int> ANF_1 = ANF(); 
    int index;
    for (auto it=ANF_1.begin(); it!=ANF_1.end(); it++){
        if (*(it) == 0) {
            out += "1⊕";
            continue;
        }
        index = len;
        for (int i = 0; i<len; i++){
            if ( (*(it) >> i) & 1 == 1 ){
                out = out + "x" + to_string(index);
            }
            index -= 1;
        }
        out += "⊕";

    }
    out = out.substr(0, out.size()-3);
    cout << out;
    cout << endl;
}
// Поиск фиктивных переменных.
// Если в АНФ нет xi => xi фиктивна. Находим дизъюнкцию всей АНФ.
// Находим дизъюнкцию всей АНФ, если в дизъюнкции - 0, то этой переменной нет в АНФ.
list<unsigned int> BF::dummyVariables(){
    list<unsigned int> dummy_var;

    list<unsigned int> ANF_1 = ANF();
    list<unsigned int>::iterator it;
    unsigned int disjunction = 0;
    unsigned int index;

    for (it=ANF_1.begin(); it!=ANF_1.end(); it++){
        disjunction |= *(it); 
    }

    for (int i =len-1; i>=0; i--){
        if ( ((disjunction >>i) & 1) == 0 ){
            index = len - i;
            dummy_var.push_back(index);
        }
    }
    return dummy_var;
}
// Поиск линейных переменных.
// Линейная переменная должна входить в АНФ 1 раз отдельным слагаемым.
// В dis хранится дизъюнкция возможных линейных переменных.
// В dis_ANF хранится дизъюнкция АНФ без возможных линейных переменных.
// теперь пробегаем по ячейке и если бит в dis == 1, а в dis_ANF == 0, то это линейная переменная.
list<unsigned int> BF::linearVariables(){
    list<unsigned int> linear_var;

    list<unsigned int> ANF_1 = ANF();
    list<unsigned int>::iterator it;
    unsigned int dis = 0;
    unsigned int dis_ANF = 0;
    unsigned int elem_index = 1;

    for (it=ANF_1.begin(); it!=ANF_1.end(); it++){

        if ( (*(it) & (*(it)-1)) == 0 ){
            dis |= *(it);
        }
        else {
            dis_ANF |= *(it);
        } 
    }

    for (int i = 31; i >= 0; i--){
        if ( ((dis >> i) & 1) == 1 && ((dis_ANF >> i) & 1) == 0 ){
            elem_index = len - i;
            linear_var.push_back(elem_index);
        }
    }
    return linear_var;
}
// Степень функции - максимальная степень монома:
// идем по АНФ и вычисляем количество единичных бит в очередном мономе.
// Если это число больше максимального - заменяем максимальное.
int BF::deg(){

    list<unsigned int> ANF_1 = ANF();
    list<unsigned int>::iterator it;
    int max_deg = 0;
    int count;

    for (it = ANF_1.begin(); it!=ANF_1.end(); it++){
        count = 0;
        for (int i = 0; i < sizeof(BASE)*8; i++){
            if ( ((*(it) >> i) & 1) == 1){
                count++;
            }
        }
        if (count > max_deg){
            max_deg = count;
        }
    }

    return max_deg;
}
// Линейность функции - функция линейна, если каждый моном в АНФ имеет степень не больше 1. Вид: {(a, x) : a ∈ Z2}
// Множество линейных функций - это подмножество аффинных. 
// Тогда, чтобы функция была линейной, нам нужно, чтобы не было a0 ∈ Z2 в АНФ (т.е. АНФ не имела вид: 1⊕...).
bool BF::isLinear(){
    
    if ( isAffine() ){

        list<unsigned int> ANF_1 = ANF();
        list<unsigned int>::iterator it;

        for (it = ANF_1.begin(); it!=ANF_1.end(); it++){
            if ( *(it) != 0 ){
                return true;
            }
            return false;
        }

    }
    return false;
}
// Аффинность функции - функция степени 1 или 0 называется аффинной. Вид: {a0 ⊕ (a, x) : a0 ∈ Z2, a ∈ Z2}
// Если функция неуравновешена, то она не может быть аффинной.
bool BF::isAffine(){
    unsigned int check = (1 << (len - 1));

    if ( weight() != check || deg() > 1){   
        return false;
    }

    return true;
}