#include "mpimon.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


void monProPSHS(hugeInt r, hugeInt a, hugeInt b, hugeInt n, uInt n0Prime, uInt rLen, int p)
{
    int q, pid, nextPid, prevPid, k, jLimit, iLimit, cTag;
    q = rLen / p;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    hugeInt rt = NULL, m = NULL, c = NULL;  int rtLen = q + 2;
    newNum(&rt, rtLen); newNum(&m, rLen);   newNum(&c, 2);
    MPI_Status status;
    nextPid = (pid + 1) % p;    prevPid = (pid > 0) ? pid - 1 : p - 1;
    uInt ca;    bigInt cs;  int i, j, l, h;
    for (l = 0; l < (2 * rLen) / (p * q); l ++)
    {
        k = pid * q + l * p * q;
        assign(rt, 0, rtLen);   iLimit = min(k + q, rLen);
        for (i = max(0, k - rLen + 1); i < iLimit; i ++)
        {
            ca = 0;
            jLimit = min(k - i + q, rLen);
            for (j = max(0, k - i); j < jLimit; j ++)
            {
                cs = (bigInt) a[j] * (bigInt) b[i] + (bigInt) rt[j - k + i] + (bigInt) ca;
                rt[j - k + i] = (uInt) cs;
                ca = cs >> W;
            }
            h = min(q, rLen - k + i);
            for (j = h; j < rtLen; j ++)
            {
                if (ca == 0) break;
                cs = (bigInt) rt[j] + (bigInt) ca;
                rt[j] = (uInt) cs;
                ca = cs >> W;
            }
        }
        iLimit = min(k + q, rLen);
        for (i = max(0, k - (int) rLen + 1); i < iLimit; i ++)
        {
            if (i >= k)
            {
                m[i] = n0Prime * rt[i - k];
                MPI_Send(&m[i], 1, MPI_INT, nextPid, i, MPI_COMM_WORLD);
            }
            else
            {
                if (i >= k - (p - 1) * q)
                    MPI_Recv(&m[i], 1, MPI_INT, prevPid, i, MPI_COMM_WORLD, &status);
                if (i >= k - (p - 2) * q)
                    MPI_Send(&m[i], 1, MPI_INT, nextPid, i, MPI_COMM_WORLD);
            }
            ca = 0;
            jLimit = min(k - i + q, rLen);
            for (j = max(0, k - i); j < jLimit; j ++)
            {
                cs = (bigInt) n[j] * (bigInt) m[i] + (bigInt) rt[j - k + i] + (bigInt) ca;
                rt[j - k + i] = (uInt) cs;
                ca = cs >> W;
            }
            h = min(q, rLen - k + i);
            for (j = h; j < rtLen; j ++)
            {
                if (ca == 0) break;
                cs = (bigInt) rt[j] + (bigInt) ca;
                rt[j] = (uInt) cs;
                ca = cs >> W;
            }

            if ((i == min(k - 1, rLen - 1)) && (k != 0))
            {
                cTag = max(0, k - (int) rLen + 1);
                cTag += max(0, k - cTag) + rLen;

                MPI_Recv(c, 2, MPI_INT, prevPid, cTag, MPI_COMM_WORLD, &status);

                ca = 0;
                for (j = 0; j < 2; j ++)
                {
                    cs = (bigInt) c[j] + (bigInt) rt[j] + (bigInt) ca;
                    rt[j] = (uInt) cs;
                    ca = cs >> W;
                }

                for (j = 2; j < rtLen; j ++)
                {
                    if (ca == 0) break;
                    cs = (bigInt) rt[j] + (bigInt) ca;
                    rt[j] = (uInt) cs;
                    ca = cs >> W;
                }
            }
        }
        cTag = min(k + q, (int) rLen) - 1;
        cTag += min(k - cTag + q, (int) rLen) + (int) rLen;
        MPI_Send(&rt[q], 2, MPI_INT, nextPid, cTag, MPI_COMM_WORLD);
        if (k >= rLen)
        {
            for (i = 0; i < q; i ++)
            {
                r[k - rLen + i] = rt[i];
            }
        }
    }
    if (pid == 0)
    {
        MPI_Recv(c, 2, MPI_INT, prevPid, 3 * rLen - 1, MPI_COMM_WORLD, &status);
    }
    MPI_Bcast(c, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Send(&r[pid * q], q, MPI_INT, nextPid, 3 * rLen, MPI_COMM_WORLD);
    MPI_Recv(&rt[0], q, MPI_INT, prevPid, 3 * rLen, MPI_COMM_WORLD, &status);
    for (i = 0; i < q; i ++)
    {
        MPI_Send(&rt[0], q, MPI_INT, nextPid, 3 * rLen + i + 1, MPI_COMM_WORLD);
        MPI_Recv(&rt[0], q, MPI_INT, prevPid, 3 * rLen + i + 1, MPI_COMM_WORLD, &status);
        for (j = 0; j < q; j ++)
        {
            r[j + k * q] = rt[j];
        }
        k = (k > 0)? k - 1 : p - 1;
    }
    
    if (c[0] != 0)
    {
        ca = 0;
        for (i = 0; i < rLen; i ++)
        {
            cs = (bigInt) r[i] - (bigInt) n[i] - (bigInt) ca;
            r[i] = (uInt) cs;
            ca = (cs >> W) & 1;
        }
    }
    free(rt);   free(m);    free(c);
}