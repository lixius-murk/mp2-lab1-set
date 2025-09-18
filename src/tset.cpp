
#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(mp)
{
    for (int i = 0; i < BitField.GetLength(); i++) {
        BitField[i] = 0;
    }
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.MaxPower)
{

}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf.GetLength())
{
}

TSet::operator TBitField()
{
    BitField(0);
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
        throw out_of_range("Элемент за ставится пределами множества");
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

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (this != &s) {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if (MaxPower != s.MaxPower) { throw std::runtime_error("Нельзя сравнить множества разных размеров!"); }
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TBitField resultBF = BitField | s.BitField;
    return TSet(resultBF);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet result = *this;
    result.InsElem(Elem);
    return result;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet result = *this;
    result.DelElem(Elem);
    return result;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TBitField resultBF = BitField & s.BitField;
    return TSet(resultBF);
}

TSet TSet::operator~(void) // дополнение
{
    TBitField resultBF = ~BitField;
    return TSet(resultBF);
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
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

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    ostr << s.BitField;
    return ostr;
}
