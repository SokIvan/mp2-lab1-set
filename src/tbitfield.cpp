// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0) throw exception("Negative Length");
	BitLen = len;
	if (BitLen % sizeof(TELEM) != 0) MemLen = BitLen / sizeof(TELEM)+1;
	else MemLen = BitLen / sizeof(TELEM);
	pMem = new TELEM[MemLen];
	memset(pMem, 0, sizeof(TELEM) * MemLen);
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));

}

TBitField::~TBitField()
{
	delete[]pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n & (sizeof(TELEM) * 8 - 1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n >= 0 && n < BitLen)
		pMem[GetMemIndex(n)] |= GetMemMask(n);
	else
		throw exception("Out of range");
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n > BitLen) throw exception("Out of range");
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= 0 && n < BitLen) 
		return pMem[GetMemIndex(n)] & GetMemMask(n);
	else
		throw exception("Out of range set error ");
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	BitLen = bf.BitLen;
	if (MemLen != bf.MemLen)
	{
		MemLen = bf.MemLen;
		delete[]pMem;
		pMem = new TELEM[MemLen];
	}
	memcpy(pMem, bf.pMem, MemLen*sizeof(TELEM));
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 0;

	for (size_t i = 0; i < MemLen; i++)
	{
		if (pMem[i] != bf.pMem[i])
			return 0;
	}
	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{        //TBitField.or_operator_applied_to_bitfields_of_non_equal_size

	if (BitLen > bf.BitLen)
	{
		TBitField tmp(*this);
		for (size_t i = 0; i < bf.MemLen; i++)
		{
			tmp.pMem[i] |= bf.pMem[i];
		}
		return tmp;
	}
	else
	{
			TBitField tmp(bf);
			for (size_t i = 0; i < MemLen; i++)
			{
				tmp.pMem[i] |= pMem[i];
			}
			return tmp;
	}
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	if (BitLen >= bf.BitLen)
	{
		TBitField tmp(*this);
		for (size_t i = 0; i < bf.MemLen; i++)
		{
			tmp.pMem[i] &= bf.pMem[i];
		}
		return tmp;
	}
	else
	{
		TBitField tmp(bf);
		for (size_t i = 0; i < MemLen; i++)
		{
			tmp.pMem[i] &= pMem[i];
		}
		return tmp;
	}
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(*this);
	for (size_t i = 0; i < BitLen; i++) {
		if (GetBit(i))
		{
			tmp.ClrBit(i);
		}
		else
		{
			tmp.SetBit(i);
		}
	}
	return tmp;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	TELEM in;
	for (size_t i = 0; i < bf.GetLength(); i++)
	{
		istr >> in;
		bf.SetBit(i);
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	size_t len = bf.GetLength();
	for (size_t i = 0; i < len; i++)
	{
		if (bf.GetBit(i))
			ostr << '1';
		else
			ostr << '0';
	}
	return ostr;
}
