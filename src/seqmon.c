#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "seqmon.h"

void monProSOS(hugeInt r, hugeInt a, hugeInt b, hugeInt n, uInt n0Prime, uInt rLen)
{
    uInt tLen = 2 * rLen + 1;
    hugeInt t;  newNum(&t, tLen);
    bigInt cs = 0;
    uInt c, m;
    unsigned int i, j;
    
    for (i = 0; i < rLen; i ++)
    {
        c = 0;
        for (j = 0; j < rLen; j ++)
        {
            cs = (bigInt) t[i + j] + (bigInt) a[j] * (bigInt) b[i] + (bigInt) c;
            t[i + j] = (uInt) cs;
            c = cs >> 32;
        }
        t[i + rLen] = c;
    }
    
    for (i = 0; i < rLen; i ++)
    {
        c = 0;
        m = t[i] * n0Prime;
        for (j = 0; j < rLen; j ++)
        {
            cs = (bigInt) t[i + j] + (bigInt) m * (bigInt) n[j] + (bigInt) c;
            t[i + j] = (uInt) cs;
            c = cs >> 32;
        }
        add(t, t, c, tLen, rLen + i);
    }
    
    hugeInt tm = t + rLen;
    if (tm[rLen] != 0)
    {
        c = 0;
        for (i = 0; i < rLen; i ++)
        {
            cs = (bigInt) tm[i] - (bigInt) n[i] - (bigInt) c;
            r[i] = (uInt) cs;
            c = (cs >> 32) & 1;
        }
    }
    else
    {
        ASSIGN(r, tm, rLen);
    }
    free(t);
}

void monProCIOS(hugeInt r, hugeInt a, hugeInt b, hugeInt n, uInt n0Prime, uInt rLen)
{
	uInt tLen = 2 * rLen + 1;
	hugeInt t;	newNum(&t, tLen);
	bigInt cs = 0;	uInt c, m;	unsigned int i, j;
	
	for (i = 0; i < rLen; i ++)
	{
		c = 0;
		for (j = 0; j < rLen; j ++)
		{
			cs = (bigInt) t[j] + (bigInt) a[j] * (bigInt) b[i] + (bigInt) c;
			t[j] = (uInt) cs;
			c = cs >> W;
		}
		cs = (bigInt) t[rLen] + (bigInt) c;
		t[rLen] = (uInt) cs;
		c = cs >> W;
		t[rLen + 1] = c;
		
		c = 0;
		m = t[0] * n0Prime;
		cs = (bigInt) t[0] + (bigInt) m * (bigInt) n[0];
		c = cs >> W;
		
		for (j = 1; j < rLen; j ++)
		{
			cs = (bigInt) t[j] + (bigInt) m * (bigInt) n[j] + (bigInt) c;
			t[j - 1] = (uInt) cs;
			c = cs >> W;
		}
		cs = (bigInt) t[rLen] + (bigInt) c;
		t[rLen - 1] = (uInt) cs;
		c = cs >> W;
		t[rLen] = t[rLen + 1] + c;
		t[rLen + 1] = 0;
	}
	
	if (t[rLen] != 0)
	{
		c = 0;
		for (i = 0; i < rLen; i ++)
		{
			cs = (bigInt) t[i] - (bigInt) n[i] - (bigInt) c;
			r[i] = (uInt) cs;
			c = (cs >> W) & 1;
		}
	}
	else
	{
		ASSIGN(r, t, rLen);
	}
	free(t);
}

void monProFIOS(hugeInt r, hugeInt a, hugeInt b, hugeInt n, uInt n0Prime, uInt rLen)
{
	hugeInt t = NULL;	newNum(&t, rLen + 2);
	int i, j;	bigInt cs;	uInt m, c;
	for (i = 0; i < rLen; i ++)
	{
		cs = (bigInt) t[0] + (bigInt) a[0] * (bigInt)b[i];
		c = cs >> W;
		add(t, t, c, 1, rLen + 2);
		m = (uInt) cs * n0Prime;
		cs = (uInt) cs + (bigInt) m * (bigInt) n[0];
		c = cs >> W;
		for (j = 1; j < rLen; j ++)
		{
			cs = (bigInt) a[j] * (bigInt) b[i] + (bigInt) t[j] + (bigInt) c;
			c = cs >> W;
			add (t, t, c, j + 1, rLen + 2);
			cs = (bigInt) m * (bigInt) n[j] + (uInt) cs;
			c = cs >> W;
			t[j - 1] = (uInt) cs;
		}
		cs = (bigInt) t[rLen] + (bigInt) c;
		c = cs >> W;
		t[rLen - 1] = (uInt) cs;
		t[rLen] = t[rLen + 1] + c;
		t[rLen + 1] = 0;
	}
	if (t[rLen] == 0)
	{
		ASSIGN(r, t, rLen);
	}
	else
	{
		c = 0;
		for (i = 0; i < rLen; i ++)
		{
			cs = (bigInt) t[i] - (bigInt) n[i] - (bigInt) c;
			r[i] = (uInt) cs;
			c = (cs >> W) & 1;
		}
	}
    free(t);
}

void monProFIOSm(hugeInt r, hugeInt a, hugeInt b, hugeInt n, uInt n0Prime, uInt rLen)
{
    hugeInt t = NULL;   newNum(&t, rLen + 1);
    int i, j;   bigInt cs, cc; uInt m;
    for (i = 0; i < rLen; i ++)
    {
        cs = (bigInt) a[0] * (bigInt) b[i] + (bigInt) t[0];
        m = (uInt) cs * n0Prime;
        cc = cs >> W;
        cs = (uInt) cs + (bigInt) m * (bigInt) n[0];
        cc += cs >> W;
        for (j = 1; j < rLen; j ++)
        {
            cs = (bigInt) a[j] * (bigInt) b[i] + (bigInt) t[j] + (uInt) cc;
            cc = (cc >> W) + (cs >> W);
            cs = (bigInt) m * (bigInt) n[j] + (uInt) cs;
            cc += cs >> W;
            t[j - 1] = (uInt) cs;
        }
        cc += (bigInt) t[rLen];
        t[rLen - 1] = (uInt) cc;
        t[rLen] = cc >> W;
    }
    if (t[rLen] == 0)
    {
        ASSIGN(r, t, rLen);
    }
    else
    {
        uInt c = 0;
        for (i = 0; i < rLen; i ++)
        {
            cs = (bigInt) t[i] - (bigInt) n[i] - (bigInt) c;
            r[i] = (uInt) cs;
            c = (cs >> W) & 1;
        }
    }
    free(t);
}