#ifndef OMPMON_H
#define OMPMON_H

#include "hugeint.h"

void monProPFIOS(hugeInt r, hugeInt a, hugeInt b, hugeInt n, uInt n0Prime, uInt rLen, int thdsNo);
void monProPFIOSn(hugeInt r, hugeInt a, hugeInt b, hugeInt n, uInt n0Prime, uInt rLen, int thdsNo);

#endif