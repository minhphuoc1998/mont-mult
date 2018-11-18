#ifndef SEQMON_H
#define SEQMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hugeint.h"

void monProSOS(hugeInt r, hugeInt a, hugeInt b, hugeInt n, uInt n0Prime, uInt rLen);

void monProFIOS(hugeInt r, hugeInt a, hugeInt b, hugeInt n, uInt n0Prime, uInt rLen);

void monProCIOS(hugeInt r, hugeInt a, hugeInt b, hugeInt n, uInt n0Prime, uInt rLen);

void monProFIOSm(hugeInt r, hugeInt a, hugeInt b, hugeInt n, uInt n0Prime, uInt rLen);

#endif