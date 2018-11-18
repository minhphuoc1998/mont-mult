#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <mpi.h>
#define min(X, Y) (((X) < (Y)) ? (X) : (Y))
#define max(X, Y) (((X) > (Y)) ? (X) : (Y))
#define W 32

typedef unsigned int uInt;
typedef unsigned long long bigInt;
typedef uInt* hugeInt;


uInt m[64] = {
    0x0C10E64F, 0x0AC4DFFE, 0x4E71B81C, 0xCF9DE538,                               
    0xFFA31F71, 0x7EF363E2, 0x6B8E75B9, 0xE3FB73C1, 0x4BA80A29, 0xC9B53DCF, 
    0x16E79763, 0x23F10B0E, 0x13042E9B, 0xC52172E4, 0xC928B2B9, 0xBE60E69C, 
    0xB9E587E8, 0x80CD86A1, 0x98C641A4, 0x315D75E1, 0x44328387, 0xCDF93ACC, 
    0xDC0A486D, 0x15987D9A, 0x1FD5A074, 0x7310F712, 0xDE31EFDC, 0x278273C7, 
    0x415D9330, 0x1602E714, 0xBC8985DB, 0x81286130, 0x70918836, 0xB3BF8A31, 
    0xB9C49708, 0x6A00E0A0, 0x8BBC27BE, 0xC6BA0B2C, 0xED34DBF6, 0xC9F98D11, 
    0xB6C12207, 0x7AD5B7D0, 0x55B7394B, 0xD91E8FEF, 0xEFDA4DF8, 0x9037C9ED, 
    0xAD6AC212, 0x6D3F8152, 0x1274A0A6, 0x1DE6B85A, 0x309C180E, 0xEB3D688A, 
    0x7BA1DF15, 0xAF9A3C40, 0xF95A56DB, 0xE6FA141D, 0xB61D0A75, 0xB54B1597, 
    0x683B9FD1, 0xA20D64E5, 0x9559C51F, 0xD660FAA7, 0x9123A9D0, 0xAD107E1E, 
};

uInt r2_modm[64] = {
    0x2ec2bd6b, 0x3bb48b69, 0x43836529, 0x44c734b8, 0xd8c671aa, 0x2b522529,
    0xfe7d0a78, 0x1b1c6bbc, 0xcf4ce640, 0xf35542d9, 0x7f8c859c, 0x3adb125b,
    0xe76dcc8f, 0x2fe74ebd, 0xf00edc9a, 0x782b7d91, 0xbf746299, 0x983d71a4,
    0xe0b8f780, 0xfcd65f0e, 0xa986611d, 0xe59df3e,  0x36bb3cfa, 0xeda913b2,
    0x25afae57, 0x96f6367,  0x4f1f69d4, 0x1f0dd87c, 0xf6c223c3, 0xf326176d,
    0x60c9e8a5, 0x342ea7d4, 0x703f2f11, 0x613c1887, 0x6054024f, 0x4d0b5ebc,
    0xd3b6694a, 0x4329764c, 0xd84e86c1, 0x8a9b8c76, 0xedc76b42, 0x36858fc9,
    0x647c0452, 0x3f462af2, 0x58ecb8d4, 0x35e47d0c, 0xeb59e616, 0x8c873d52,
    0x3aeab8d,  0x66b25557, 0xcd278a9d, 0x3bf43296, 0x6d86b7a2, 0x7d04e4c9,
    0x927ba630, 0x32a3d5f2, 0x16008064, 0xd552f36a, 0x59e84817, 0xf94dcf8d,
    0xc149dead, 0xcbaf2cd,  0xde5c8027, 0x9cd7f320
};

uInt r_modm[64] = {
    0xf3ef19b1, 0xf53b2001, 0xb18e47e3, 0x30621ac7, 0x5ce08e,   0x810c9c1d,
    0x94718a46, 0x1c048c3e, 0xb457f5d6, 0x364ac230, 0xe918689c, 0xdc0ef4f1,
    0xecfbd164, 0x3ade8d1b, 0x36d74d46, 0x419f1963, 0x461a7817, 0x7f32795e,
    0x6739be5b, 0xcea28a1e, 0xbbcd7c78, 0x3206c533, 0x23f5b792, 0xea678265,
    0xe02a5f8b, 0x8cef08ed, 0x21ce1023, 0xd87d8c38, 0xbea26ccf, 0xe9fd18eb,
    0x43767a24, 0x7ed79ecf, 0x8f6e77c9, 0x4c4075ce, 0x463b68f7, 0x95ff1f5f,
    0x7443d841, 0x3945f4d3, 0x12cb2409, 0x360672ee, 0x493eddf8, 0x852a482f,
    0xaa48c6b4, 0x26e17010, 0x1025b207, 0x6fc83612, 0x52953ded, 0x92c07ead,
    0xed8b5f59, 0xe21947a5, 0xcf63e7f1, 0x14c29775, 0x845e20ea, 0x5065c3bf,
    0x6a5a924,  0x1905ebe2, 0x49e2f58a, 0x4ab4ea68, 0x97c4602e, 0x5df29b1a,
    0x6aa63ae0, 0x299f0558, 0x6edc562f, 0x52ef81e1
};

uInt m_prime[64] = {
    0x52af8f51, 0xcab19312, 0x4d72ab2c, 0xab0d8d3,  0x1c310b8d, 0xed456389,
    0xd15055c8, 0x6b94f163, 0xd2eda983, 0x816337bd, 0x63733eee, 0xea09e89b,
    0x834448a2, 0x11786fdf, 0x969fb701, 0x315c7a9,  0x3a05e816, 0xd2b3269a,
    0x815aa6ee, 0x84679f17, 0xd2e50962, 0xd85713af, 0x4a319cd3, 0x7a9b0f29,
    0x1eeadfdc, 0xc2dc3cdf, 0xff866ce2, 0x9101214b, 0xe9a60d60, 0xf64c2321,
    0x6c62ccaa, 0x79517d70, 0xe2c2197a, 0x2b77d945, 0x94671797, 0x6f012989,
    0x3a190ce7, 0x1b0adb10, 0xe756d200, 0xaa8c6626, 0x7f49466,  0xba73edfa,
    0xbd0f7ca5, 0xffba8d95, 0x1edd58fd, 0x3bd88b24, 0x7b8d283,  0x73ba075b,
    0xd0499cef, 0x91573f97, 0x37384faa, 0xa6d6b6d5, 0x781722cb, 0xd0b70cce,
    0xe4167c3b, 0x7c558936, 0x9e6baf8,  0xfadcea61, 0xbce8ed35, 0xb482166c,
    0x7e0b8f8e, 0xa78046de, 0x82086f,   0x6348474e
};

//A prime
uInt x[64] = {
    0x1E1A1597, 0xDB094AE9, 0xD7EF09CA, 0x693877FA,                                   
    0x6E11715F, 0x6116D227, 0xC198AF12, 0xA4B54330, 0xD7014103, 0x75F26375,     
    0x54E710C3, 0xC3A3960A, 0xBD0BE621, 0xDED4010A, 0x89962856, 0xC0B857F6,     
    0x71506026, 0xB3CA3F79, 0xE6B486F6, 0x1CCACB83, 0x14056425, 0x67E144E5,     
    0xA41825D9, 0xF6A167B5, 0x96524D8E, 0x3AD83477, 0x51BFA4AB, 0xF13C6D9A,     
    0x35488A0E, 0x2D525267, 0xCAA6B790, 0xB63ACAE1, 0x81B23F76, 0x4FDB70C5,     
    0x12307F5C, 0xBC39A0BF, 0xB1E59BB8, 0xB941F54E, 0xD45F9088, 0x6C5BFC11,     
    0x4275BF7B, 0x22E0B1EF, 0x5B4758C0, 0x91F9E672, 0x6BCF67ED, 0x5A8A9D30,     
    0x97517ABD, 0x209E0C64, 0x830E9A7C, 0x3BF4296D, 0x34096FAA, 0x16C3D911,     
    0x61B2AA30, 0xFAF7DF45, 0xD61957D4, 0xE00DF8F1, 0x435E3B00, 0x5D2CEED4,     
    0x660DD0F2, 0x8CEEF608, 0x65195999, 0xFFBBD19C, 0xB4B6663C, 0x87A8E61D
};

//The generator
uInt y[64] = {
     0x6CC41659, 0x664B4C0F, 0xEF98C582, 0x5E2327CF, 
     0xD4795451, 0xD647D148, 0x90F00EF8, 0x2F630784, 0x1DB246C3, 0x184B523D,  
     0xCDC67EB6, 0xC7891428, 0x0DF92B52, 0x7FD02837, 0x64E0EC37, 0xB3353BBB,  
     0x57CD0915, 0xECD06E15, 0xDF016199, 0xB7D2BBD2, 0x052588B9, 0xC8484B1E,  
     0x13D3FE14, 0xDB2A3B73, 0xD182EA0A, 0xD052B985, 0xE83B9C80, 0xA4BD1BFF,  
     0xFB3F2E55, 0xDFC967C1, 0x767164E1, 0xB5045AF2, 0x6F2F9193, 0x1D14348F,  
     0x428EBC83, 0x64E67982, 0x82D6ED38, 0x8AC376D2, 0xAAB8A862, 0x777DE62A,  
     0xE9EC144B, 0xDDF463E5, 0xC77A57F2, 0x0196F931, 0x41000A65, 0xA55AE313,  
     0xC28CBB18, 0x901228F8, 0x7E8C6F62, 0xBC3773BF, 0x0C6B47B1, 0xBE3A6C1B,  
     0xAC0BB555, 0xFF4FED4A, 0x77BE463F, 0x10DBC150, 0x1A0BA125, 0x07F4793A,  
     0x21EF2054, 0x4CA7B18F, 0x60EDBD48, 0x2E775066, 0x73134D0B, 0x3FB32C9B
};

uInt r1[64] = {
    0x775dc713, 0x4c086acb, 0xc00d9adb, 0xee86f71d, 0xc29f99b6, 0x6212ac07,
    0xed85c6dd, 0x734ac6c9, 0xa2937ff9, 0xe242cfb6, 0x1f8065ea, 0x69a57a75,
    0x9237befc, 0x64431258, 0x79ed554d, 0x182da473, 0xa7d65c06, 0x1cbf2c36,
    0x0f32f00b, 0x64a3feae, 0xff16e94c, 0xceb78a0f, 0xad0228ac, 0x1ff9c335,
    0xba0f4a78, 0xe86a097e, 0x0ba5e9eb, 0x5e1f6a07, 0x341e1477, 0x9a314f12,
    0xd5daff22, 0x0d8b9c99, 0x83ed7723, 0xcc9c4944, 0x293c7171, 0x79c38dbf,
    0xd85afec7, 0xcc2cb39f, 0xcf3e0609, 0x5c89ed39, 0xbedbda30, 0xcde41c64,
    0xe44e9515, 0xd42398b3, 0xe5712598, 0x7b9b32bb, 0x194b9a7e, 0x9fce5b9d,
    0x36274209, 0xa0ff4f59, 0xd0bde9ae, 0x79b537a4, 0xf13cd019, 0x0ff419a4,
    0xa5cdfaa0, 0x030d284b, 0xa123b384, 0x93f516d0, 0x33f1d506, 0xfa3a9fca,
    0x9092fef0, 0x9c882165, 0xac070171, 0xb1cf880b
};

uInt r2[64] = {
    0x1a34febe, 0xcae22663, 0x1cfabd65, 0xdfbbee7e, 0xce892108, 0x39369a4a,
    0x0cc6d84e, 0xb35c557b, 0x71f60411, 0x24ca9567, 0x874d48be, 0x51383311,
    0xedd108ce, 0x2e531f17, 0xf3f17585, 0x95e1be19, 0xbf6bc835, 0x78e3191a,
    0x5b340f81, 0xc42f40cd, 0x0adf020a, 0x1a15d7d3, 0x7169eeb8, 0xd743c98c,
    0xd5489e7a, 0x77820a14, 0x99b33aa8, 0x96d17c23, 0x4ff119b8, 0xd89fa470,
    0x2f501942, 0xbc4f6f12, 0x16316ebf, 0xfa0bc6dd, 0x168c6229, 0xe3ef37c3,
    0xbf920790, 0xbaa5f742, 0x5416ffb7, 0x46ed6850, 0x4fbfa58c, 0x35e5d756,
    0x4a32b0cb, 0x942bfd5d, 0x86765283, 0xbf6e53ac, 0xd3067181, 0x5f2c6c24,
    0x7cc08a73, 0xfed04330, 0x4f243115, 0x1502cc1d, 0xcc48d44f, 0x6d53aded,
    0x19f3834c, 0x4dcda5d0, 0xb4b2b457, 0xccc4421d, 0x3be3fda0, 0x355db612,
    0x18e063ab, 0xa2c7a26b, 0xaef9dccc, 0xcdee1fe4
};

int newNum(hugeInt *t, size_t size);
int assign(hugeInt t, uInt n, uInt rLen);
int print(hugeInt t, uInt rLen);
int ADD(hugeInt result, hugeInt opr1, hugeInt opr2, uInt rLen);
int Add(hugeInt result, hugeInt opr1, uInt opr2, uInt rLen);
int add(hugeInt result, hugeInt opr1, uInt opr2, uInt rLen, uInt index);
int ASSIGN(hugeInt dest, hugeInt src, uInt rLen);
int COMP(hugeInt opr1, hugeInt opr2, uInt rLen);
void monProSOS(hugeInt r, hugeInt a, hugeInt b, hugeInt n, uInt n0Prime, uInt rLen);
void monProFIOS(hugeInt r, hugeInt a, hugeInt b, hugeInt n, uInt n0Prime, uInt rLen);
void monProCIOS(hugeInt r, hugeInt a, hugeInt b, hugeInt n, uInt n0Prime, uInt rLen);
void monProFIOSm(hugeInt r, hugeInt a, hugeInt b, hugeInt n, uInt n0Prime, uInt rLen);
void monProPFIOS(hugeInt r, hugeInt a, hugeInt b, hugeInt n, uInt n0Prime, uInt rLen);
void monProPSHS(hugeInt r, hugeInt a, hugeInt b, hugeInt n, uInt n0Prime, uInt rLen, int p);
uInt modInv(uInt x, uInt w);
uInt nZPrime(uInt nz, uInt w);

int main(int argc, char **argv)
{
    
	hugeInt r = NULL, a = NULL, b = NULL, n = NULL, f = NULL;
	newNum(&n, 64);
	ASSIGN(n, m, 64);
	
	newNum(&a, 64);
	ASSIGN(a, x, 64);
	
	newNum(&b, 64);
	ASSIGN(b, y, 64);
	
	newNum(&r, 64);
	
	uInt n0 = n[0];
	uInt n0p = nZPrime(n0, 32);
	printf("%u %u\n", n0, n0p);
/*	
	monProPFIOS(r, a, b, n, n0p, 64);
    
    print(r, 64);
    printf("\n");
*/   
    MPI_Init(&argc, &argv);
    int pid;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    
    for (int i = 1; i < 10; i ++)
    {
        monProPSHS(r, a, b, n, n0p, 64, 2);
        printf("%d %d\n", i, pid);
    }
    

    if (pid == 0)
    {
        print(r, 64);
        printf("\n");
    }

    if (pid == 1)
    {
        printf("123\n");
    }
    MPI_Finalize();

    
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
    //free(t);
}

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

/*
void monProPFIOS(hugeInt r, hugeInt a, hugeInt b, hugeInt n, uInt n0Prime, uInt rLen)
{
	hugeInt t = NULL;	newNum(&t, 2 * rLen + 1);
	int i, j, thdsNo = 2;
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
				add(t, t, cs >> W, ji + 1, 2 * rLen + 1);
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
*/

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