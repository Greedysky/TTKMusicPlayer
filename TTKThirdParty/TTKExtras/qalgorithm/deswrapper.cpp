#include "deswrapper.h"
#include "ttkcryptographichash.h"

namespace QAlgorithm
{
static constexpr quint64 ARRAYMASK[] = {
    0x0000000000000001l, 0x0000000000000002l, 0x0000000000000004l, 0x0000000000000008l,
    0x0000000000000010l, 0x0000000000000020l, 0x0000000000000040l, 0x0000000000000080l,
    0x0000000000000100l, 0x0000000000000200l, 0x0000000000000400l, 0x0000000000000800l,
    0x0000000000001000l, 0x0000000000002000l, 0x0000000000004000l, 0x0000000000008000l,
    0x0000000000010000l, 0x0000000000020000l, 0x0000000000040000l, 0x0000000000080000l,
    0x0000000000100000l, 0x0000000000200000l, 0x0000000000400000l, 0x0000000000800000l,
    0x0000000001000000l, 0x0000000002000000l, 0x0000000004000000l, 0x0000000008000000l,
    0x0000000010000000l, 0x0000000020000000l, 0x0000000040000000l, 0x0000000080000000l,
    0x0000000100000000l, 0x0000000200000000l, 0x0000000400000000l, 0x0000000800000000l,
    0x0000001000000000l, 0x0000002000000000l, 0x0000004000000000l, 0x0000008000000000l,
    0x0000010000000000l, 0x0000020000000000l, 0x0000040000000000l, 0x0000080000000000l,
    0x0000100000000000l, 0x0000200000000000l, 0x0000400000000000l, 0x0000800000000000l,
    0x0001000000000000l, 0x0002000000000000l, 0x0004000000000000l, 0x0008000000000000l,
    0x0010000000000000l, 0x0020000000000000l, 0x0040000000000000l, 0x0080000000000000l,
    0x0100000000000000l, 0x0200000000000000l, 0x0400000000000000l, 0x0800000000000000l,
    0x1000000000000000l, 0x2000000000000000l, 0x4000000000000000l, 0x8000000000000000l
};

static constexpr int ARRAYIP[] = {
    57, 49, 41, 33, 25, 17,  9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7,
    56, 48, 40, 32, 24, 16,  8, 0,
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6
};

static constexpr int ARRAYE[] = {
    31, 0,   1,  2,  3,  4, -1, -1,
    3,  4,   5,  6,  7,  8, -1, -1,
    7,  8,   9, 10, 11, 12, -1, -1,
    11, 12, 13, 14, 15, 16, -1, -1,
    15, 16, 17, 18, 19, 20, -1, -1,
    19, 20, 21, 22, 23, 24, -1, -1,
    23, 24, 25, 26, 27, 28, -1, -1,
    27, 28, 29, 30, 31, 30, -1, -1
};

static constexpr char MATRIXNSBOX[8][64] = {
    { 14,  4,  3, 15,  2, 13, 5,  3,
      13, 14,  6,  9, 11,  2, 0,  5,
       4,  1, 10, 12, 15,  6, 9, 10,
       1,  8, 12,  7,  8, 11, 7,  0,
       0, 15, 10,  5, 14,  4, 9, 10,
       7,  8, 12,  3, 13,  1, 3,  6,
      15, 12,  6, 11,  2,  9, 5,  0,
       4,  2, 11, 14,  1,  7, 8, 13
    },
    { 15,  0,  9,  5,  6, 10, 12,  9,
       8,  7,  2, 12,  3, 13,  5,  2,
       1, 14,  7,  8, 11,  4,  0,  3,
      14, 11, 13,  6,  4,  1, 10, 15,
       3, 13, 12, 11, 15,  3,  6,  0,
       4, 10,  1,  7,  8,  4, 11, 14,
      13,  8,  0,  6,  2, 15,  9,  5,
       7,  1, 10, 12, 14,  2,  5,  9
    },
    { 10, 13, 1, 11, 6, 8, 11, 5,
      9, 4, 12, 2, 15, 3, 2, 14,
      0, 6, 13, 1, 3, 15, 4, 10,
      14, 9, 7, 12, 5, 0, 8, 7,
      13, 1, 2, 4, 3, 6, 12, 11,
      0, 13, 5, 14, 6, 8, 15, 2,
      7, 10, 8, 15, 4, 9, 11, 5,
      9, 0, 14, 3, 10, 7, 1, 12
    },
    { 7, 10, 1, 15, 0, 12, 11, 5,
      14, 9, 8, 3, 9, 7, 4, 8,
      13, 6, 2, 1, 6, 11, 12, 2,
      3, 0, 5, 14, 10, 13, 15, 4,
      13, 3, 4, 9, 6, 10, 1, 12,
      11, 0, 2, 5, 0, 13, 14, 2,
      8, 15, 7, 4, 15, 1, 10, 7,
      5, 6, 12, 11, 3, 8, 9, 14
    },
    { 2, 4, 8, 15, 7, 10, 13, 6,
      4, 1, 3, 12, 11, 7, 14, 0,
      12, 2, 5, 9, 10, 13, 0, 3,
      1, 11, 15, 5, 6, 8, 9, 14,
      14, 11, 5, 6, 4, 1, 3, 10,
      2, 12, 15, 0, 13, 2, 8, 5,
      11, 8, 0, 15, 7, 14, 9, 4,
      12, 7, 10, 9, 1, 13, 6, 3
    },
    { 12, 9, 0, 7, 9, 2, 14, 1,
      10, 15, 3, 4, 6, 12, 5, 11,
      1, 14, 13, 0, 2, 8, 7, 13,
      15, 5, 4, 10, 8, 3, 11, 6,
      10, 4, 6, 11, 7, 9, 0, 6,
      4, 2, 13, 1, 9, 15, 3, 8,
      15, 3, 1, 14, 12, 5, 11, 0,
      2, 12, 14, 7, 5, 10, 8, 13
    },
    { 4, 1, 3, 10, 15, 12, 5, 0,
      2, 11, 9, 6, 8, 7, 6, 9,
      11, 4, 12, 15, 0, 3, 10, 5,
      14, 13, 7, 8, 13, 14, 1, 2,
      13, 6, 14, 9, 4, 1, 2, 14,
      11, 13, 5, 0, 1, 10, 8, 3,
      0, 11, 3, 5, 9, 4, 15, 2,
      7, 8, 12, 15, 10, 7, 6, 12
    },
    { 13, 7, 10, 0, 6, 9, 5, 15,
      8, 4, 3, 10, 11, 14, 12, 5,
      2, 11, 9, 6, 15, 12, 0, 3,
      4, 1, 14, 13, 1, 2, 7, 8,
      1, 2, 12, 15, 10, 4, 0, 3,
      13, 14, 6, 9, 7, 8, 9, 6,
      15, 1, 5, 12, 3, 10, 14, 5,
      8, 7, 11, 0, 4, 13, 2, 11
    }
};

static constexpr int ARRAYP[] = {
    15,  6, 19, 20, 28, 11, 27, 16,
     0, 14, 22, 25,  4, 17, 30,  9,
     1,  7, 23, 13, 31, 26,  2,  8,
    18, 12, 29,  5, 21, 10,  3, 24
};

static constexpr int ARRAYIP_1[] = {
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41,  9, 49, 17, 57, 25,
    32, 0, 40,  8, 48, 16, 56, 24
};

static constexpr int ARRAYPC_1[] = {
    56, 48, 40, 32, 24, 16,  8,  0,
    57, 49, 41, 33, 25, 17,  9,  1,
    58, 50, 42, 34, 26, 18, 10,  2,
    59, 51, 43, 35, 62, 54, 46, 38,
    30, 22, 14,  6, 61, 53, 45, 37,
    29, 21, 13,  5, 60, 52, 44, 36,
    28, 20, 12,  4, 27, 19, 11,  3
};

static constexpr int ARRAYPC_2[] = {
    13, 16, 10, 23,  0,  4, -1, -1,
     2, 27, 14,  5, 20,  9, -1, -1,
    22, 18, 11,  3, 25,  7, -1, -1,
    15,  6, 26, 19, 12,  1, -1, -1,
    40, 51, 30, 36, 46, 54, -1, -1,
    29, 39, 50, 44, 32, 47, -1, -1,
    43, 48, 38, 55, 33, 52, -1, -1,
    45, 41, 49, 35, 28, 31, -1, -1
};

static constexpr int ARRAYLS[] = {
    1, 1, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 1
};

static constexpr quint64 ARRAYLSMASK[] = {
    0x0000000000000000l, 0x0000000000100001l, 0x0000000000300003l
};

enum class Mode
{
    Encrypt = 0,
    Decrypt = 1
};

/*! @brief The class of the des wrapper private.
 * @author Greedysky <greedysky@163.com>
 */
class DesPrivate : public TTKPrivate<Des>
{
public:
    quint64 bitTransform(const int *array, int len, quint64 source);
    void desSubKeys(quint64 key, quint64* K, Mode mode);
    quint64 des64(quint64 *subkeys, quint64 data);
    char* encrypt(char *src, int length, char *key);

    Mode m_mode;
};

quint64 DesPrivate::bitTransform(const int *array, int len, quint64 source)
{
    quint64 dest = 0;
    const quint64 bts = source;

    for(int bti = 0; bti < len; ++bti)
    {
        if(array[bti] >= 0 && (bts & ARRAYMASK[array[bti]]) != 0)
        {
            dest |= ARRAYMASK[bti];
        }
    }
    return dest;
}

void DesPrivate::desSubKeys(quint64 key, quint64* K, Mode mode)
{
    quint64 temp = bitTransform(ARRAYPC_1, 56, key);
    for(int j = 0; j < 16; ++j)
    {
        const quint64 source = temp;
        temp = ((source & ARRAYLSMASK[ARRAYLS[j]]) << (28 - ARRAYLS[j])) | ((source & ~ARRAYLSMASK[ARRAYLS[j]]) >> ARRAYLS[j]);
        K[j] = bitTransform(ARRAYPC_2, 64, temp);
    }

    if(mode == Mode::Decrypt)
    {
        quint64 t;
        for(int j = 0; j < 8; ++j)
        {
            t = K[j];
            K[j] = K[15 - j];
            K[15 - j] = t;
        }
    }
}

quint64 DesPrivate::des64(quint64 *subkeys, quint64 data)
{
    quint64 out = bitTransform(ARRAYIP, 64, data);
    quint64 l = 0, r = 0;
    int sOut = 0;

    uchar *pR = new uchar[8];
    int *pSource = new int[2];
    pSource[0] = (int) (out & quint64(0x00000000ffffffffl));
    pSource[1] = (int) ((out & quint64(0xffffffff00000000l)) >> 32);

    for(int i = 0; i < 16; ++i)
    {
        r = pSource[1];
        r = bitTransform(ARRAYE, 64, r);
        r ^= subkeys[i];

        for(int k = 0; k < 8; ++k)
        {
            pR[k] = (uchar) (0xff & (r >> (k * 8)));
        }

        sOut = 0;
        for(int sbi = 7; sbi >= 0; --sbi)
        {
            sOut <<= 4;
            sOut |= MATRIXNSBOX[sbi][pR[sbi]];
        }
        r = sOut;
        r = bitTransform(ARRAYP, 32, r);

        l = pSource[0];
        pSource[0] = pSource[1];
        pSource[1] = (int) (l ^ r);
    }

    int t = pSource[0];
    pSource[0] = pSource[1];
    pSource[1] = t;

    out = ((quint64) pSource[1] << 32 & quint64(0xffffffff00000000l)) | (quint64) (quint64(0x00000000ffffffffl) & pSource[0]);
    out = bitTransform(ARRAYIP_1, 64, out);

    delete[] pR;
    delete[] pSource;
    return out;
}

char* DesPrivate::encrypt(char *src, int length, char *key)
{
    quint64 keyl = 0;
    for(int i = 0; i < 8; ++i)
    {
        const quint64 temp = (quint64) key[i] << (i * 8);
        keyl |= temp;
    }

    const int num = length / 8;

    quint64* subKey = new quint64[16];
    for(int i = 0; i < 16; ++i)
    {
        subKey[i] = 0;
    }
    desSubKeys(keyl, subKey, m_mode);

    quint64* pSrc = (quint64*) malloc(num * sizeof(quint64));
    for(int i = 0; i < num; ++i)
    {
        pSrc[i] = 0;
        for(int j = 0; j < 8; ++j)
        {
            pSrc[i] |= (quint64) src[i * 8 + j] << (j * 8);
        }
    }

    quint64* pEncyrpt = (quint64*) malloc((((num + 1) * 8 + 1) / 8) * sizeof(quint64));
    for(int i = 0; i < num; ++i)
    {
        pEncyrpt[i] = des64(subKey, pSrc[i]);
    }

    const int len = length;
    const int tailNum = len % 8;
    char* szTail = (char*) malloc(length - num * 8);

    memcpy(szTail, src + num * 8, length - num * 8);

    quint64 tail64 = 0;
    for(int i = 0; i < tailNum; ++i)
    {
        tail64 = tail64 | (quint64(szTail[i])) << (i * 8);
    }

    pEncyrpt[num] = des64(subKey, tail64);
    char* result = (char*) malloc((num + 1) * 8);

    int temp = 0;
    for(int i = 0; i < num + 1; ++i)
    {
        for(int j = 0; j < 8; ++j)
        {
            result[temp] = (char) (0xff & (pEncyrpt[i] >> (j * 8)));
            temp++;
        }
    }

    delete[] subKey;
    free(pSrc);
    free(pEncyrpt);
    free(szTail);

    return result;
}



Des::Des()
{
    TTK_INIT_PRIVATE(Des);
}

QByteArray Des::encrypt(const QByteArray &in, const QByteArray &key)
{
    TTK_D(Des);
    d->m_mode = Mode::Encrypt;
    char *encData = d->encrypt((char *)in.data(), in.length(), (char *)key.data());
    const TTKString &str = TTK::base64Encode((unsigned char *)encData, (in.length() / 8 + 1) * 8);

    delete encData;
    return QByteArray(str.data(), str.length());
}

QByteArray Des::decrypt(const QByteArray &in, const QByteArray &key)
{
    TTK_D(Des);
    d->m_mode = Mode::Decrypt;
    const TTKString &str = TTK::base64Decode(TTKString(in.data(), in.length()));
    char *encData = d->encrypt((char *)str.data(), str.length(), (char *)key.data());

    QByteArray value(encData);
    delete encData;
    return value;
}
}
