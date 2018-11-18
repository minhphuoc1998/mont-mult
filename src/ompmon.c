#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <sys/wait.h>
#include "ompmon.h"

void monProPFIOS(hugeInt r, hugeInt a, hugeInt b, hugeInt n, uInt n0Prime, uInt rLen, int thdsNo)
{
	hugeInt t = NULL;	newNum(&t, 2 * rLen + 1);
	int i, j;
	bigInt cs, cc;
	omp_set_dynamic(0);
	omp_set_num_threads(thdsNo);
#pragma omp parallel default(none) private(i, j, cs, cc) firstprivate(rLen, n0Prime, a, b, n, t, thdsNo)
	{
		int tid = omp_get_thread_num();
		int chk = rLen / thdsNo;
		for (i = 0; i < rLen; i ++)
		{
			uInt m = (t[i] + a[0] * b[i]) * n0Prime;
#pragma omp barrier
			cc = 0;
#pragma imp for schedule (static, chk)
			for (j = 0; j < rLen; j ++)
			{
				cs = (bigInt) a[j] * (bigInt) b[i] + (bigInt) t[i + j] + (uInt) cc;
				cc = (cc >> W) + (cs >> W);
				cs = (bigInt) m * (bigInt) n[j] + (uInt) cs;
				cc += cs >> W;
				t[i + j] = (uInt) cs;
			}
			
			int ji = (tid + 1) * chk + i;
#pragma omp critical
			{
				cs = (bigInt) t[ji] + (bigInt) cc;
				t[ji] = (uInt) cs;
				//add(t, t, cs >> W, ji + 1, 2 * rLen + 1);
			}
		}
	}
	
	
	if (t[2 * rLen] == 0)
	{
		ASSIGN(r, t + rLen, rLen);
	}
	else
	{
		cc = 0;
		for (i = 0; i < rLen; i ++)
		{
			cs = (bigInt) t[rLen + i] - (bigInt) n[i] - (bigInt) cc;
			r[i] = (uInt) cs;
			cc = (cs >> W) & 1;
		}
	}
}

void monProPFIOSn(hugeInt r, hugeInt a, hugeInt b, hugeInt n, uInt n0Prime, uInt rLen, int thdsNo)
{
	hugeInt t = NULL;
	hugeInt t0s = NULL;
	bigInt* cc = NULL;
	newNum(&t, 2 * rLen);
	newNum(&t0s, rLen + 1);
	cc = calloc(rLen, sizeof(bigInt));
	int i, j, id;	bigInt cs;	uInt mi = 0, c = 0;
	omp_set_dynamic(0);
	omp_set_num_threads(thdsNo);
#pragma omp parallel default(none) private(i, j, id, cs) firstprivate(rLen, n0Prime, a, b, n, t, t0s, cc, thdsNo) shared(c, mi)
	{
		int tid = omp_get_thread_num();
		int chk = rLen / thdsNo;
		
		for (i = 0; i < rLen; i ++)
		{
#pragma omp flush(mi)
			while (i > mi)
			{
				wait(0);
#pragma omp flush(mi)
			}
			
			uInt m = (t0s[i] + a[0] * b[i]) * n0Prime;
			int ci = tid * chk + i % chk;
			cc[ci] = 0;
			int jst = 2 * chk * tid + i % chk;
			int ji = 0;
			
#pragma omp for schedule (static, chk) nowait
			for (j = 0; j < rLen; j ++)
			{
				cs = (bigInt) a[j] * (bigInt) b[i] + (bigInt) t[jst + ji] + (uInt) cc[ci];
				cc[ci] = (cc[ci] >> W) + (cs >> W);
				cs = (bigInt) m * (bigInt) n[j] + (uInt) cs;
				cc[ci] += cs >> W;
				t[jst + ji ++] = (uInt) cs;
			}
#pragma omp master
			{
				t0s[i + 1] = t[jst + 1];
				mi = i + 1;
			}
			
			if ((i + 1) % chk == 0)
			{
#pragma omp barrier
#pragma omp master
				{
					bigInt cb = 0;
					for (id = 0; id < thdsNo; id ++)
					{
						int js1 = chk * (2 * id + 1);
						int js2 = chk * (2 * (id + 1));
						int jd = chk * (2 * id);
						
						for (j = 0; j < chk; j ++)
						{
							cs = (js2 < 2 * rLen) ? t[js2 + j] : 0;
							cs += (bigInt) t[js1 + j] + (uInt) cb;
							cb = (bigInt) (cs >> W) + (bigInt) (cb >> W);
							cs = (bigInt) ((uInt) cs) + (uInt) cc[id * chk + j];
							cb += (bigInt) (cs >> W) + (bigInt) (cc[id * chk + j] >> W);
							t[jd + j] = (uInt) cs;
							t[js1 + j] = 0;
							if (js2 < 2 * rLen) t[js2 + j] = 0;
						}
					}
					c = (uInt) cb;
					t0s[i + 1] = t[0];	t[chk * (2 * thdsNo - 1)] = c;
				}
#pragma omp barrier
			}
		}
	}
	int chk = rLen / thdsNo;
	if (c == 0)
	{
		for (id = 0; id < thdsNo; id ++)
		{
			int js = chk * (2 * id);
			for (j = 0; j < chk; j ++)
			{
				r[id * chk + j] = t[js + j];
			}
		}
	}
	else
	{
		c = 0;
		for (id = 0; id < thdsNo; id ++)
		{
			int js = chk * (2 * id);
			for (j = 0; j < chk; j ++)
			{
				cs = (bigInt) t[js + j] - (bigInt) n[id * chk + j] - (bigInt) c;
				r[id * chk + j] = (uInt) cs;
				c = (cs >> W) & 1;
			}
		}
	}
	free(t);	free(cc);	free(t0s);
}
			