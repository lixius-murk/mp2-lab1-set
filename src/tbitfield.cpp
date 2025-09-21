#include "tbitfield.h"
#include "vector"

//int  BitLen; // длина битового поля - макс. к-во битов
//TELEM* pMem; // память для представления битового поля
//int  MemLen; // к-во эл-тов Мем для представления бит.поля
//
//int BitForInt = sizeof(TELEM) * 8;

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);


TBitField::TBitField(int len)
{
    if (len < 0) {
        throw invalid_argument("Длина не может быть отрицательной");
    }
    n = len;
    MemLen = (n + BitPerInt - 1) / BitPerInt;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
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

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= this->n) {
        throw out_of_range("Индекс за пределами диапазона");
    }
    return n / BitPerInt;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= this->n) {
        throw out_of_range("Индекс за пределами диапазона");
    }
    return 1 << (n % BitPerInt);

}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return n;
}

void TBitField::SetBit(const int n) // установить бит
{
    pMem[GetMemIndex(n)] |= GetMemMask(n); //надо ли выделять новые байты?????
}

void TBitField::ClrBit(const int n) // очистить бит
{
    pMem[GetMemIndex(n)] &= ~(GetMemMask(n)); //зануляем то, что было 1
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= this->n) {
        return 0;
    }
    return (pMem[GetMemIndex(n)] & GetMemMask(n));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this != &bf) {
        if (MemLen != bf.MemLen) {
            delete[] pMem;
            MemLen = bf.MemLen;
            pMem = new TELEM[MemLen];
        }

        n = bf.n;
        for (int i = 0; i < n; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

bool TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (n != bf.n) { throw std::runtime_error("Нельзя сравнить множества разных размеров!"); }
    TBitField result(std::min(n, bf.n));
    for (int i = 0; i < std::min(MemLen, bf.MemLen); i++) {
        result.pMem[i] = pMem[i] & bf.pMem[i];
    }

    return (result == 1) ? 1 : 0;
}

bool TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    if (n != bf.n) { throw std::runtime_error("Нельзя объединить множества разных размеров!"); }
    TBitField result(std::max(n, bf.n));
    for (int i = 0; i < (std::max(MemLen, bf.MemLen) + std::min(MemLen, bf.MemLen)); i++) {
        result.pMem[i] = pMem[i] | bf.pMem[i];
    }

    return result;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"

{
    if (n != bf.n) { throw std::runtime_error("Нельзя пересечь множества разных размеров!"); }
    TBitField result(std::min(n, bf.n));
    for (int i = 0; i < std::min(MemLen, bf.MemLen); i++) {
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
