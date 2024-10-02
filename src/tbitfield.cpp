// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
	if (len <= 0) throw - 1;
	BitLen = len;
	MemLen = (BitLen + (sizeof(TELEM) * 8 - 1)) / (sizeof(TELEM) * 8);
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	MemLen = bf.MemLen;
	BitLen = bf.BitLen;
	pMem = new TELEM[MemLen];

	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	int res;
	if ((n < 0) || (n >= BitLen)) throw - 3;
	res = n / 32;
	return res;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	int len = BitLen;
	return len;
}

void TBitField::SetBit(const int n) // установить бит
{
	int i = GetMemIndex(n);
	TELEM t = GetMemMask(n);
	pMem[i] = pMem[i] | t;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	int i = GetMemIndex(n);
	TELEM t = ~GetMemMask(n);
	pMem[i] = pMem[i] & t;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (this != &bf) {
		delete[] pMem;

		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++) {
			pMem[i] = bf.pMem[i];
		}
	}

	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (this->BitLen != bf.BitLen) return 0;
	for (int i = 0; i < MemLen - 1; ++i)
		if (pMem[i] != bf.pMem[i]) return 0;

	for (int i = (MemLen - 1) * sizeof(TELEM) * 8; i < BitLen; i++)
		if (GetBit(i) != bf.GetBit(i)) return 0;

	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	if (this->BitLen != bf.BitLen) return 1;
	for (int i = 0; i < BitLen; i++) {
		if (this->GetBit(i) != bf.GetBit(i)) return 1;
	}
	return 0;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	const int maxLength = BitLen > bf.BitLen ? BitLen : bf.BitLen;

	TBitField res(maxLength);
	for (int i = 0; i < MemLen; i++) {
		res.pMem[i] = this->pMem[i] | bf.pMem[i];
	}

	return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	const int maxLength = BitLen > bf.BitLen ? BitLen : bf.BitLen;

	TBitField res(maxLength);
	for (int i = 0; i < MemLen; i++) {
		res.pMem[i] = this->pMem[i] & bf.pMem[i];
		return res;
	}
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);
	for (int i = 0; i < MemLen; i++) {
		res.pMem[i] = ~pMem[i];
	}
	int lastElementMask = (1 << BitLen % (sizeof(TELEM) * 8)) - 1;
	if (BitLen % (sizeof(TELEM) * 8) != 0)
		res.pMem[MemLen - 1] &= lastElementMask;
	return res;
}

// ввод/вывод

istream& operator>>(istream& in, TBitField& bf) // ввод
{
	for (int i = 0; i < bf.BitLen; ++i) {
		int b;
		in >> b;
		if (b) {
			bf.SetBit(i);
		}
		else {
			bf.ClrBit(i);
		}
	}
	return in;
}

ostream& operator<<(ostream& out, const TBitField& bf) // вывод
{
	for (int i = 0; i < bf.BitLen; ++i) {
		if (bf.GetBit(i) == 0) out << 0;
		else out << 1;
	}
	return out;
}
