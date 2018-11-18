#ifndef HUGEINT_H
#define HUGEINT_H

#include <stdlib.h>
#include <stdio.h>
#define min(X, Y) (((X) < (Y)) ? (X) : (Y))
#define max(X, Y) (((X) > (Y)) ? (X) : (Y))
#define W 32

typedef unsigned int uInt;
typedef unsigned long long bigInt;
typedef uInt *hugeInt;

int readi(FILE *file, hugeInt buf, int size);
int newNum(hugeInt *t, size_t size);
int assign(hugeInt t, uInt n, uInt rLen);
int print(hugeInt t, uInt rLen);
int ADD(hugeInt result, hugeInt opr1, hugeInt opr2, uInt rLen);
int Add(hugeInt result, hugeInt opr1, uInt opr2, uInt rLen);
int add(hugeInt result, hugeInt opr1, uInt opr2, uInt rLen, uInt index);
int ASSIGN(hugeInt dest, hugeInt src, uInt rLen);
int COMP(hugeInt opr1, hugeInt opr2, uInt rLen);

uInt modInv(uInt x, uInt w);
uInt nZPrime(uInt nz, uInt w);

#endif
