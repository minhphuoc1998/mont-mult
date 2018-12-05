#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hugeint.h"

int readi(FILE *file, hugeInt buf, int size)
{
	int i;
    for (i = 0; i < size; i ++)
    {
        fscanf(file, "%x", &buf[i]);
    }
    
    return 0;
}

int newNum(hugeInt *t, size_t size)
{
	if (size == 0)
	{
		t = NULL;
		return 0;
	}
	*t = (hugeInt) calloc(size, sizeof(uInt));
	if (*t == NULL)
	{
		puts("Cannot allocate memory!!");
		exit(-1);;
	}
	return 1;
}

int assign(hugeInt t, uInt n, uInt rLen)
{
	t[0] = n;
	return 1;
}

int print(hugeInt t, uInt rLen)
{
	unsigned int i;
	for (i = 0; i < rLen; i ++)
	{
		printf("%08x", t[rLen - 1 - i]);
	}
	return 1;
}

int Add(hugeInt result, hugeInt opr1, uInt opr2, uInt rLen)
{
	return add(result, opr1, opr2, rLen, (uInt) 0);
}

int add(hugeInt result, hugeInt opr1, uInt opr2, uInt rLen, uInt index)
{
    if (index >= rLen)
        return 1;
	ASSIGN(result, opr1, rLen);
	bigInt temp;
	temp = (bigInt) result[index] + (bigInt) opr2;
	uInt carry = (uInt) (temp >> 32);
	if (carry)
	{
		add(result, result, (uInt) 1, rLen, index + 1);
	}
	result[index] = (uInt) ((temp << 32) >> 32);
	
	return 0;
}

int ASSIGN(hugeInt dest, hugeInt src, uInt rLen)
{
	memcpy(dest, src, rLen * sizeof(uInt));
	return 0;
}

int COMP(hugeInt opr1, hugeInt opr2, uInt rLen)
{
	uInt i;
	for (i = 0; i < rLen; i ++)
	{
		if (opr1[i] > opr2[i])
			return 1;
		else if (opr1[i] < opr2[i])
			return -1;
	}
	return 0;
}

uInt modInv(uInt x, uInt w)
{
    uInt r = 1;
    bigInt c = 1;
    int i;
    for (i = 2; i <= w; i ++)
    {
        c <<= 1;
        if (((bigInt) x * (bigInt) r) % (c << 1) > c)
        {
            r = r + (uInt) c;
        }
    }
    return r;
}

uInt nZPrime(uInt nz, uInt w)
{
    uInt nzinv = modInv(nz, w);
    uInt nzp = ((uInt) -1) - nzinv + 1;
    return nzp;
}
