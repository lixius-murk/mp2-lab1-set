#include "TBitField.h"
#include "vector"

//int  BitLen; // длина битового пол€ - макс. к-во битов
//TELEM* pMem; // пам€ть дл€ представлени€ битового пол€
//int  MemLen; // к-во эл-тов ћем дл€ представлени€ бит.пол€
//
//int BitForInt = sizeof(TELEM) * 8;

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);


TBitField::TBitField(int len)
{
    if (len < 0) {
        throw invalid_argument("ƒлина не может быть отрицательной");
    }
    n = len;
    MemLen = (n + BitPerInt - 1) / BitPerInt;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копировани€
{

    MemLen = bf.MemLen;


    n = bf.n;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < n; i++) {
        pMem[i] = bf.pMem[i];

    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс ћем дл€ бита n
{
    if (n < 0 || n >= this->n) {
        throw out_of_range("»ндекс за пределами диапазона");
    }
    return n / BitPerInt;
}

TELEM TBitField::GetMemMask(const int n) const // битова€ маска дл€ бита n
{
    if (n < 0 || n >= this->n) {
        throw out_of_range("»ндекс за пределами диапазона");
    }
    return 1 << (n % BitPerInt);

}

// доступ к битам битового пол€

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return n;
}

void TBitField::SetBit(const int n) // установить бит
{
    pMem[GetMemIndex(n)] |= GetMemMask(n); //надо ли выдел€ть новые байты?????
}

void TBitField::ClrBit(const int n) // очистить бит
{
    pMem[GetMemIndex(n)] &= ~(GetMemMask(n)); //занул€ем то, что было 1
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= this->n) {
        throw out_of_range("»ндекс за пределами диапазона");
    }
    return (pMem[GetMemIndex(n)] & GetMemMask(n));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this != &bf) {
        delete[] pMem;

        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        n = bf.n;
        for (int i = 0; i < n; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

bool TBitField::operator==(const TBitField& bf) const // сравнение
{
    int minLen = min(n, bf.n);
    for (int i = 0; i < minLen; i++) {
        if (GetBit(i) != bf.GetBit(i)) return 0;
    }

    for (int i = minLen; i < n; i++) {
        if (GetBit(i)) return 0;
    }
    for (int i = minLen; i < bf.n; i++) {
        if (bf.GetBit(i)) return 0;
    }

    return true;

    //if (n != bf.n) { throw std::runtime_error("Ќельз€ сравнить множества разных размеров!"); }
    //TBitField result(std::min(n, bf.n));
    //for (int i = 0; i < std::min(MemLen, bf.MemLen); i++) {
    //    result.pMem[i] = pMem[i] & bf.pMem[i];
    //}

    //return (result == 1) ? 1 : 0;
}

bool TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операци€ "или"
{
    //if (n != bf.n) { throw std::runtime_error("Ќельз€ объединить множества разных размеров!"); }
    int maxLen = max(n, bf.n);
    TBitField result(maxLen);
    int minMemLen = min(MemLen, bf.MemLen);

    for (int i = 0; i < minMemLen; i++) {
        result.pMem[i] = pMem[i] | bf.pMem[i];
    }

    if (MemLen > minMemLen) {
        for (int i = minMemLen; i < MemLen; i++) {
            result.pMem[i] = pMem[i];
        }
    }
    else if (bf.MemLen > minMemLen) {
        for (int i = minMemLen; i < bf.MemLen; i++) {
            result.pMem[i] = bf.pMem[i];
        }
    }

    return result;
}

TBitField TBitField::operator&(const TBitField& bf) // операци€ "и"

{
    //if (n != bf.n) { throw std::runtime_error("Ќельз€ пересечь множества разных размеров!"); }
    TBitField result(min(n, bf.n));
    for (int i = 0; i < min(MemLen, bf.MemLen); i++) {
        result.pMem[i] = pMem[i] & bf.pMem[i];
    }

    return result;

}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(n);

    for (int i = 0; i < MemLen; i++) {
        res.pMem[i] = ~pMem[i];
    }

    int extraBits = n % BitPerInt;
    if (extraBits != 0) {
        TELEM mask = (1 << extraBits) - 1;
        res.pMem[MemLen - 1] &= mask;
    }
    return res;

}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    for (int i = 0; i < bf.n; i++) {
        int value;
        istr >> value;
        if (value == 1) {
            bf.SetBit(i);
        }
        else if (value == 0) {
            bf.ClrBit(i);
        }
        bf.n++;
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    vector<TELEM> el;
    for (int i = 0; i < bf.n; i++) {
        if (bf.GetBit(i) != 0) {
            el.push_back(i);
        }
    }
    ostr << "{";
    for (TELEM i = 0; i < el.size(); i++) {
        if (i > 0) ostr << ", ";
        ostr << el[i];
    }
    ostr << "}";
    return ostr;
}
