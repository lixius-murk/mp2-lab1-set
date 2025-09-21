
#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : MaxPower(mp), BitField(mp)
{
    for (int i = 0; i < BitField.GetLength(); i++) {
        BitField.ClrBit(i);
    }
}

// конструктор копирования
TSet::TSet(const TSet& s) :MaxPower(s.MaxPower), BitField(s.MaxPower)
{

}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf)
{
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw out_of_range("Элемент вне юниверса множества!");
    }
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw out_of_range("Элемент ставится за пределами множества");
    }
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw out_of_range("Элемент за пределами множества удаляется");
    }
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if (this != &s) {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    if (MaxPower != s.MaxPower) { throw std::runtime_error("Нельзя сравнить множества разных размеров!"); }
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    return ~(*this == s);
}

TSet TSet::operator+(const TSet& s) // объединение
{
    TSet res(max(MaxPower, s.MaxPower));
    for (int i = 0; i < MaxPower; i++) {
        if (BitField.GetBit(i)) {
            res.InsElem(i);
        }
    }
    for (int i = 0; i < s.MaxPower; i++) {
        if (s.BitField.GetBit(i)) {
            res.InsElem(i);
        }
    }

    return res;
    //TBitField res = BitField | s.BitField;
    //return TSet(res);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet res = *this;
    if (Elem >= MaxPower) {
        TSet newSet(Elem + 1);
        for (int i = 0; i < MaxPower; i++) {
            if (BitField.GetBit(i)) {
                newSet.InsElem(i);
            }
        }
        newSet.InsElem(Elem);
        return newSet;
    }

    res.InsElem(Elem);
    return res;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet res = *this;
    if (Elem >= 0 && Elem <= MaxPower) {
        res.DelElem(Elem);
    }
    return res;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    TSet res(min(MaxPower, s.MaxPower));

    for (int i = 0; i < res.MaxPower; i++) {
        if (BitField.GetBit(i) && s.BitField.GetBit(i)) {
            res.InsElem(i);
        }
    }

    return res;
    //TBitField resultBF = BitField & s.BitField;
    //return TSet(resultBF);
}

TSet TSet::operator~(void) // дополнение
{
    TBitField res = ~BitField;
    return TSet(res);
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    for (int i = 0; i < s.MaxPower; i++) {
        int value;
        istr >> value;
        if (value == 1) {
            s.InsElem(i);
        }
        else if (value == 0) {
            s.DelElem(i);
        }
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    ostr << s.BitField;
    return ostr;
}