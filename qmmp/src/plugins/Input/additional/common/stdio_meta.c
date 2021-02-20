#include "stdio_meta.h"
#include "convert_utf.h"
#include <ctype.h>

///////////////////////////////////////////////////////////////////////
#define UTF8_COMPUTE(Char, Mask, Len)                                       \
  if(Char < 128)                                                            \
  {                                                                         \
    Len = 1;                                                                \
    Mask = 0x7f;                                                            \
  }                                                                         \
  else if((Char & 0xe0) == 0xc0)                                            \
  {                                                                         \
    Len = 2;                                                                \
    Mask = 0x1f;                                                            \
  }                                                                         \
  else if((Char & 0xf0) == 0xe0)                                            \
  {                                                                         \
    Len = 3;                                                                \
    Mask = 0x0f;                                                            \
  }                                                                         \
  else if((Char & 0xf8) == 0xf0)                                            \
  {                                                                         \
    Len = 4;                                                                \
    Mask = 0x07;                                                            \
  }                                                                         \
  else if((Char & 0xfc) == 0xf8)                                            \
  {                                                                         \
    Len = 5;                                                                \
    Mask = 0x03;                                                            \
  }                                                                         \
  else if((Char & 0xfe) == 0xfc)                                            \
  {                                                                         \
    Len = 6;                                                                \
    Mask = 0x01;                                                            \
  }                                                                         \
  else                                                                      \
    Len = -1;

#define UTF8_LENGTH(Char)              \
  ((Char) < 0x80 ? 1 :                 \
   ((Char) < 0x800 ? 2 :               \
    ((Char) < 0x10000 ? 3 :            \
     ((Char) < 0x200000 ? 4 :          \
      ((Char) < 0x4000000 ? 5 : 6)))))


#define UTF8_GET(Result, Chars, Count, Mask, Len)                             \
  (Result) = (Chars)[0] & (Mask);                                             \
  for((Count) = 1; (Count) < (Len); ++(Count))                                \
    {                                                                         \
      if(((Chars)[(Count)] & 0xc0) != 0x80)                                   \
      {                                                                       \
        (Result) = -1;                                                        \
        break;                                                                \
      }                                                                       \
      (Result) <<= 6;                                                         \
      (Result) |= ((Chars)[(Count)] & 0x3f);                                  \
    }

#define UNICODE_VALID(Char)                   \
    ((Char) < 0x110000 &&                     \
    (((Char) & 0xFFFFF800) != 0xD800) &&      \
    ((Char) < 0xFDD0 || (Char) > 0xFDEF) &&   \
    ((Char) & 0xFFFE) != 0xFFFE)

/* is c the start of a utf8 sequence? */
#define isutf(c) (((c) & 0xC0) != 0x80)

int u8_valid(const char *str, int max_len, const char **end)
{
    const char *p;
    if(!str)
    {
        return 0;
    }

    if(end)
    {
        *end = str;
    }

    p = str;

    while((max_len < 0 || (p - str) < max_len) && *p)
    {
        int i, mask = 0, len;
        int32_t result;
        unsigned char c = (unsigned char) *p;

        UTF8_COMPUTE(c, mask, len);

        if(len == -1)
        {
            break;
        }

        /* check that the expected number of bytes exists in str */
        if(max_len >= 0 && ((max_len - (p - str)) < len))
        {
            break;
        }

        UTF8_GET(result, p, i, mask, len);

        if(UTF8_LENGTH(result) != len) /* Check for overlong UTF-8 */
        {
            break;
        }

        if(result == (int32_t) - 1)
        {
            break;
        }

        if(!UNICODE_VALID(result))
        {
            break;
        }

        p += len;
    }

    if(end)
    {
        *end = p;
    }

    /* See that we covered the entire length if a length was
     * passed in, or that we ended on a nul if not
     */
    if(max_len >= 0 && p != (str + max_len) && *p != 0)
    {
        return 0;
    }
    else if(max_len < 0 && *p != '\0')
    {
        return 0;
    }
    return 1;
}

int can_be_chinese(const uint8_t *str, int sz)
{
    int len = strlen(str);
    int i;
    for(i = 0; i < sz; str++, i++)
    {
        if(i < len-3
                && (*str >= 0x81 && *str <= 0xFE )
                && (*(str+1) >= 0x30 && *(str+1) <= 0x39)
                && (*(str+2) >= 0x81 && *(str+2) <= 0xFE)
                && (*(str+3) >= 0x30 && *(str+3) <= 0x39))
        {
            return 1;
        }

        if(i < len - 1
                && (*str >= 0x81 && *str <= 0xFE )
                && ((*(str+1) >= 0x40 && *(str+1) <= 0x7E)
                    || (*(str+1) >= 0x80 && *(str+1) <= 0xFE)))
        {
            return 1;
        }
    }
    return 0;
}

int can_be_russian(const signed char *str, int size)
{
   int latin = 0;
   int rus = 0;
   int rus_in_row = 0;
   int max_rus_row = 0;
   int n = 0;
   for(; n < size; str++, n++)
   {
       if((*str >= 'A' && *str <= 'Z') || *str >= 'a' && *str <= 'z')
       {
           if(rus_in_row > max_rus_row)
           {
               max_rus_row = rus_in_row;
           }
           rus_in_row = 0;
           latin++;
       }
       else if(*str < 0)
       {
           rus_in_row++;
           rus++;
       }
   }

   if(rus > latin / 2 || (max_rus_row > 4))
   {
       return 1;
   }
   return 0;
}

void u8_inc(const char *s, int32_t *i)
{
    (void)(isutf(s[++(*i)]) || isutf(s[++(*i)]) || isutf(s[++(*i)]) || ++(*i));
}

uint16_t extract_i16(const uint8_t *buf)
{
    uint16_t x;
    // big endian extract
    x = buf[0];
    x <<= 8;
    x |= buf[1];
    return x;
}

///////////////////////////////////////////////////////////////////////
uint16_t sj_to_unicode[] = {
#include "stdio_unicode.h"
};

static int cp1251_to_utf8(const uint8_t *in, int inlen, uint8_t *out, int outlen)
{
    static const long utf[256] = {
        0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
        31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,
        59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,
        87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,
        111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,1026,1027,8218,
        1107,8222,8230,8224,8225,8364,8240,1033,8249,1034,1036,1035,1039,1106,8216,8217,
        8220,8221,8226,8211,8212,8250,8482,1113,8250,1114,1116,1115,1119,160,1038,1118,1032,
        164,1168,166,167,1025,169,1028,171,172,173,174,1031,176,177,1030,1110,1169,181,182,
        183,1105,8470,1108,187,1112,1029,1109,1111,1040,1041,1042,1043,1044,1045,1046,1047,
        1048,1049,1050,1051,1052,1053,1054,1055,1056,1057,1058,1059,1060,1061,1062,1063,
        1064,1065,1066,1067,1068,1069,1070,1071,1072,1073,1074,1075,1076,1077,1078,1079,
        1080,1081,1082,1083,1084,1085,1086,1087,1088,1089,1090,1091,1092,1093,1094,1095,
        1096,1097,1098,1099,1100,1101,1102,1103
    };

    uint8_t *out_start = out;
    uint8_t *end = out + outlen;
    int i;
    for(i = 0; i < inlen && out < end - 4; i++)
    {
        long c = utf[*in++];
        if(c < 0x80)
        {
            *out++ = c;
        }
        else if(c < 0x800)
        {
            *out++ = (c >> 6) | 0xc0;
            *out++ = (c & 0x3f) | 0x80;
        }
        else if( c < 0x10000)
        {
            *out++ = (c >> 12) | 0xe0;
            *out++ = ((c >> 6) & 0x3f) | 0x80;
            *out++ = (c & 0x3f) | 0x80;
        }
    }
    *out++ = 0;
    return (int)(out - out_start);
}

int cp1252_to_utf8(const uint8_t *in, int inlen, uint8_t *out, int outlen)
{
    int len = 0;
    while(inlen > 0 && outlen-len > 2)
    {
        uint8_t c=*in;
        switch(c)
        {
        case 192 ... 255:
            *out++ = 195;
            *out++ = c - 64;
            len += 2;
            break;
        case 160 ... 191:
            *out++ = 0xc2;
            *out++ = c;
            len += 2;
            break;
#define CONV2(x,y,z) case x:\
            *out++ = y;\
            *out++ = z;\
            len += 2;\
            break;
#define CONV3(x,y,z,w) case x:\
            *out++ = y;\
            *out++ = z;\
            *out++ = w;\
            len += 3;\
            break;
        CONV2(0x9f,0xc5,0xb8);
        CONV2(0x9e,0xc5,0xbe);
        CONV3(0x9d,0xef,0xbf,0xbd);
        CONV3(0x80,0xe2,0x82,0xac);
        CONV3(0x81,0xef,0xbf,0xbd);
        CONV3(0x82,0xe2,0x80,0x9a);
        CONV2(0x83,0xc6,0x92);
        CONV3(0x84,0xe2,0x80,0x9e);
        CONV3(0x85,0xe2,0x80,0xa6);
        CONV3(0x86,0xe2,0x80,0xa0);
        CONV3(0x87,0xe2,0x80,0xa1);
        CONV2(0x88,0xcb,0x86);
        CONV3(0x89,0xe2,0x80,0xb0);
        CONV2(0x8a,0xc5,0xa0);
        CONV3(0x8b,0xe2,0x80,0xb9);
        CONV2(0x8c,0xc5,0x92);
        CONV3(0x8d,0xef,0xbf,0xbd);
        CONV2(0x8e,0xc5,0xbd);
        CONV3(0x8f,0xef,0xbf,0xbd);
        CONV3(0x90,0xef,0xbf,0xbd);
        CONV3(0x91,0xe2,0x80,0x98);
        CONV3(0x92,0xe2,0x80,0x99);
        CONV3(0x93,0xe2,0x80,0x9c);
        CONV3(0x94,0xe2,0x80,0x9d);
        CONV3(0x95,0xe2,0x80,0xa2);
        CONV3(0x96,0xe2,0x80,0x93);
        CONV3(0x97,0xe2,0x80,0x94);
        CONV2(0x98,0xcb,0x9c);
        CONV3(0x99,0xe2,0x84,0xa2);
        CONV2(0x9a,0xc5,0xa1);
        CONV3(0x9b,0xe2,0x80,0xba);
        CONV2(0x9c,0xc5,0x93);
#undef CONV2
#undef CONV3
        default:
            *out++ = c;
            len++;
            break;
        }

        in++;
        inlen--;
    }
    *out = 0;
    return len;
}

int utf8_to_cp1252(const uint8_t *in, int inlen, uint8_t *out, int outlen)
{
    uint8_t *outptr = out;
    const char *cp1252_charset[] = {"€", "", "‚", "ƒ", "„", "…", "†", "‡", "ˆ", "‰", "Š", "‹", "Œ", "", "Ž", "", "", "‘", "’", "“", "”", "•", "–", "—", "˜", "™", "š", "›", "œ", "", "ž", "Ÿ", " ", "¡", "¢", "£", "¤", "¥", "¦", "§", "¨", "©", "ª", "«", "¬", "­", "®", "¯", "°", "±", "²", "³", "´", "µ", "¶", "·", "¸", "¹", "º", "»", "¼", "½", "¾", "¿", "À", "Á", "Â", "Ã", "Ä", "Å", "Æ", "Ç", "È", "É", "Ê", "Ë", "Ì", "Í", "Î", "Ï", "Ð", "Ñ", "Ò", "Ó", "Ô", "Õ", "Ö", "×", "Ø", "Ù", "Ú", "Û", "Ü", "Ý", "Þ", "ß", "à", "á", "â", "ã", "ä", "å", "æ", "ç", "è", "é", "ê", "ë", "ì", "í", "î", "ï", "ð", "ñ", "ò", "ó", "ô", "õ", "ö", "÷", "ø", "ù", "ú", "û", "ü", "ý", "þ", "ÿ", NULL};
    while(inlen && outlen > 0)
    {
        if(*in < 0x80)
        {
            *out++ = *in++;
            outlen--;
            inlen--;
        }
        else
        {
            int idx = 0;
            u8_inc((char *)in, &idx);
            int i;
            for(i = 0; cp1252_charset[i]; i++)
            {
                if(strlen(cp1252_charset[i]) == idx && !memcmp(in, cp1252_charset[i], idx))
                {
                    *out++ = i + 0x80;
                    outlen--;
                    break;
                }
            }

            if(!cp1252_charset[i])
            {
                return -1;
            }
            in += idx;
            inlen -= idx;
        }
    }
    *out = 0;
    return (int)(out-outptr);
}

int utf8_to_ascii(const uint8_t *in, int inlen, uint8_t *out, int outlen)
{
    uint8_t *outptr = out;
    while(inlen && outlen > 0)
    {
        if(*in < 0x80)
        {
            *out++ = *in++;
            outlen--;
            inlen--;
        }
        else
        {
            int idx = 0;
            u8_inc((char *)in, &idx);
            in += idx;
            inlen -= idx;
        }
    }
    *out = 0;
    return (int)(out-outptr);
}

ConversionResult ConvertUTF16BEtoUTF8(const UTF16** sourceStart, const UTF16* sourceEnd, UTF8** targetStart, UTF8* targetEnd, ConversionFlags flags)
{
    // swap to make it little endian
    size_t sourceLESize = (size_t)(sourceEnd - *sourceStart) * sizeof(UTF16);
    UTF16 sourceLE[sourceLESize];
    UTF16 *pLE = sourceLE;
    const UTF16 *p = *sourceStart;
    for(; p != sourceEnd; p++)
    {
        *pLE++ = extract_i16((const uint8_t *)p);
    }

    const UTF16 *leStart = sourceLE;
    ConversionResult res = ConvertUTF16toUTF8(&leStart, pLE, targetStart, targetEnd, flags);
    *sourceStart += pLE - sourceLE;
    return res;
}

ConversionResult ConvertUTF8toUTF16BE(const UTF8** sourceStart, const UTF8* sourceEnd, UTF16** targetStart, UTF16* targetEnd, ConversionFlags flags)
{
    UTF16 *output = *targetStart;
    ConversionResult res = ConvertUTF8toUTF16(sourceStart, sourceEnd, targetStart, targetEnd, flags);
    if(res == conversionOK)
    {
        UTF16 *p = output;
        for(; p != *targetStart; p++)
        {
            *p = extract_i16((const uint8_t *)p);
        }
    }
    return res;
}

///////////////////////////////////////////////////////////////////////
int stdio_iconv(const char *in, int inlen, char *out, int outlen, const char *cs_in, const char *cs_out)
{
    // NOTE: this function must support utf8->utf8 conversion, used for validation
    int len = -1;
    *out = 0;
    if(inlen==0)
    {
        return 0;
    }

    if(!strcmp(cs_in, "UTF-16"))
    {
        if(in[0] == 0xfe && in[1] == 0xff)
        {
            cs_in = "UTF-16BE";
        }
        else
        {
            cs_in = "UTF-16LE";
        }
    }

    // to utf8 branch
    if(!strcasecmp(cs_out, "utf-8"))
    {
        if(!strcasecmp(cs_in, "utf-8"))
        {
            memcpy(out, in, inlen);
            out[inlen] = 0;
            int valid = u8_valid (out, inlen, NULL);
            if(valid) {
                len = inlen;
            }
        }
        else if(!strcasecmp(cs_in, "cp1251"))
        {
            len = cp1251_to_utf8(in, inlen, out, outlen);
        }
        else if(!strcasecmp(cs_in, "iso8859-1") || !strcasecmp(cs_in, "cp1252"))
        {
            len = cp1252_to_utf8(in, inlen, out, outlen);
        }
        else if(!strcasecmp(cs_in, "UTF-16LE") || !strcasecmp(cs_in, "UCS-2LE"))
        {
            char *target = out;
            ConversionResult result = ConvertUTF16toUTF8((const UTF16**)&in, (const UTF16*)(in + inlen), (UTF8**)&target, (UTF8*)(out + outlen), strictConversion);
            if(result == conversionOK)
            {
                *target = 0;
                len = target - out;
            }
        }
        else if(!strcasecmp(cs_in, "UTF-16BE") || !strcasecmp(cs_in, "UCS-2BE"))
        {
            // convert to big endian
            char temp[inlen];
            int i;
            for(i = 0; i < inlen; i += 2)
            {
                temp[i] = in[i+1];
                temp[i+1] = in[i];
            }
            char *target = out;
            char *src = temp;
            ConversionResult result = ConvertUTF16BEtoUTF8((const UTF16**)&src, (const UTF16*)(temp + inlen), (UTF8**)&target, (UTF8*)(out + outlen), strictConversion);
            if(result == conversionOK)
            {
                *target = 0;
                len = target - out;
            }
        }
        else if(!strcasecmp(cs_in, "SHIFT-JIS"))
        {
            int len = 0;
            while(inlen > 0 && len < outlen)
            {
                if(*in > 0)
                {
                    *out++ = *in++;
                    inlen--;
                    len++;
                }
                else if(inlen < 2)
                {
                    return -1;
                }
                else
                {
                    // find character in table
                    uint16_t c = (((uint8_t*)in)[0] << 8) | ((uint8_t*)in)[1];
                    int i;
                    for(i = 0; sj_to_unicode[i]; i += 2)
                    {
                        if(c == sj_to_unicode[i])
                        {
                            break;
                        }
                    }
                    if(sj_to_unicode[i])
                    {
                        // slow conversion!
                        char unicode_val[2] = { (sj_to_unicode[i+1] & 0xff00) >> 8, sj_to_unicode[i+1] & 0xff };
                        char utf8_val[5];
                        char *src = unicode_val, *dst = utf8_val;

                        ConversionResult res = ConvertUTF16toUTF8 ((const UTF16**)&src, (const UTF16 *)(src+2), (UTF8**)&dst, dst+5, strictConversion);
                        if(res == conversionOK)
                        {
                            if(src - utf8_val < outlen-len)
                            {
                                memcpy(out, utf8_val, src - utf8_val);
                                out += src - utf8_val;
                                len += src - utf8_val;
                                inlen -= 2;
                                in += 2;
                            }
                            else
                            {
                                return -1;
                            }
                        }
                        else
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        return -1; // error
                    }
                }
            }
        }
    }
    else if(!strcasecmp(cs_in, "utf-8"))
    {
        if(!strcasecmp(cs_out, "UTF-16LE") || !strcasecmp(cs_out, "UCS-2LE"))
        {
            char *target = out;
            ConversionResult result = ConvertUTF8toUTF16((const UTF8**)&in, (const UTF8*)(in + inlen), (UTF16**)&target, (UTF16*)(out + outlen), strictConversion);
            if(result == conversionOK)
            {
                *target = 0;
                *(target+1) = 0;
                len = target - out;
            }
        }
        else if(!strcasecmp(cs_out, "UTF-16BE") || !strcasecmp(cs_out, "UCS-2BE"))
        {
            char *target = out;
            ConversionResult result = ConvertUTF8toUTF16BE((const UTF8**)&in, (const UTF8*)(in + inlen), (UTF16**)&target, (UTF16*)(out + outlen), strictConversion);
            if(result == conversionOK)
            {
                *target = 0;
                *(target+1) = 0;
                len = target - out;
            }
        }
        else if(!strcasecmp(cs_out, "cp1252") || !strcasecmp(cs_out, "iso8859-1"))
        {
            int res = utf8_to_cp1252((uint8_t *)in, inlen, (uint8_t *)out, outlen);
            if(res >= 0)
            {
                len = res;
            }
        }
        else if(!strcasecmp(cs_out, "ascii"))
        {
            int res = utf8_to_ascii((uint8_t *)in, inlen, (uint8_t *)out, outlen);
            if(res >= 0) {
                len = res;
            }
        }
    }
    return len;
}

int can_be_shift_jis(const unsigned char *str, int size)
{
    unsigned char out[size*4];
    if(size < 2)
    {
        return 0;
    }

    const unsigned char *p = str;
    int s = size;
    while(s >= 2)
    {
        if((((p[0] >= 0x81 && p[0] <= 0x84) || (p[0] >= 0x87 && p[0] <= 0x9f))
                    && ((p[1] >= 0x40 && p[1] <= 0x9e) || (p[1] >= 0x9f && p[1] <= 0xfc)))
                || ((p[0] >= 0xe0 && p[0] <= 0xef)
                    && ((p[1] >= 0x40 && p[1] <= 0x9e) || (p[1] >= 0x9f && p[1] <= 0xfc))))
        {
            break;
        }
        s--;
        p++;
    }

    if(s >= 2)
    {
        if(stdio_iconv(str, size, out, sizeof(out), "shift-jis", "utf-8") >= 0)
        {
            return 1;
        }
    }
    return 0;
}

char *convstr_id3v2(int version, uint8_t encoding, const uint8_t *str, int sz)
{
    const char *enc = NULL;
    // detect encoding
    if(version == 4 && encoding == 2)
    {
        enc = "UTF-16BE";
    }
    else if(version == 4 && encoding == 3)
    {
        enc = "utf-8";
    }
    else if(encoding == 0)
    {
        if(can_be_chinese(str, sz))
        {
            // hack to add cp936 support
            enc = "cp936";
        }
        else if(can_be_russian(str, sz))
        {
            // hack to add limited cp1251 recoding support
            enc = "cp1251";
        }
        else
        {
            enc = "cp1252";
        }
    }
    else if(encoding != 1 && !(version == 4 && encoding == 3))
    {
        return NULL; // invalid encoding
    }

    if(encoding == 1)
    {
        // detect kind of unicode used
        if(sz < 2)
        {
            return NULL;
        }

        if(version < 4)
        {
            if(str[0] == 0xff && str[1] == 0xfe)
            {
                enc = "UCS-2LE";
                str += 2;
                sz -= 2;
            }
            else if(str[1] == 0xff && str[0] == 0xfe)
            {
                enc = "UCS-2BE";
                str += 2;
                sz -= 2;
            }
            else
            {
                enc = "UCS-2LE";
                // NOTE: this is an assumption, might break in the future.
            }
        }
        else
        {
            enc = "UTF-16";
        }
    }

    int converted_sz = 0;

    int outlen = sz*4+1;
    char *out = malloc(outlen);
    if((converted_sz = stdio_iconv(str, sz, out, outlen, enc, "utf-8")) < 0)
    {
        free(out);
        return NULL;
    }

    int n;
    for(n = 0; n < converted_sz; n++)
    {
        if(out[n] == 0 && n != converted_sz-1)
        {
            out[n] = '\n';
        }
    }
    // trim trailing linebreaks
    for(n = converted_sz-1; n >= 0; n--)
    {
        if((uint8_t)out[n] <= 32)
        {
            out[n] = 0;
        }
        else
        {
            break;
        }
    }
    return out;
}

const char *convstr_id3v1(const char* str, int sz, const char *charset, char *out, int outsize)
{
    if(!charset)
    {
        return str;
    }
    int i;
    for(i = 0; i < sz; i++)
    {
        if(str[i] != ' ')
        {
            break;
        }
    }

    if(i == sz)
    {
        out[0] = 0;
        return out;
    }

    int len = stdio_iconv(str, sz, out, outsize, charset, "utf-8");
    if(len >= 0)
    {
        return out;
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////
typedef struct metacache_str_s
{
    struct metacache_str_s *next;
    uint32_t refcount;
    char cmpidx; // positive means "equals", negative means "notequals"
    char str[1];
} metacache_str_t;

typedef struct
{
    metacache_str_t *chain;
} metacache_hash_t;

#define HASH_SIZE 4096
static metacache_hash_t hash[HASH_SIZE];

uint32_t metacache_get_hash_sdbm(const char *str)
{
    uint32_t hash = 0;
    int c;
    while(c = *str++)
    {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

metacache_str_t *metacache_find_in_bucket(uint32_t h, const char *str)
{
    metacache_hash_t *bucket = &hash[h];
    metacache_str_t *chain = bucket->chain;
    while(chain)
    {
        if(!strcmp(chain->str, str))
        {
            return chain;
        }
        chain = chain->next;
    }
    return NULL;
}

static int n_strings = 0;
static int n_inserts = 0;
static int n_buckets = 0;

const char *metacache_add_string(const char *str)
{
    uint32_t h = metacache_get_hash_sdbm(str);
    metacache_str_t *data = metacache_find_in_bucket(h % HASH_SIZE, str);
    n_inserts++;
    if(data)
    {
        data->refcount++;
        return data->str;
    }

    metacache_hash_t *bucket = &hash[h % HASH_SIZE];
    if(!bucket->chain)
    {
        n_buckets++;
    }

    size_t len = strlen(str);
    data = malloc(sizeof(metacache_str_t) + len);
    memset(data, 0, sizeof(metacache_str_t) + len);
    data->refcount = 1;
    memcpy(data->str, str, len+1);
    data->next = bucket->chain;
    bucket->chain = data;
    n_strings++;
    return data->str;
}

void metacache_remove_string(const char *str)
{
    uint32_t h = metacache_get_hash_sdbm (str);
    metacache_hash_t *bucket = &hash[h % HASH_SIZE];
    metacache_str_t *chain = bucket->chain;
    metacache_str_t *prev = NULL;

    while(chain)
    {
        if(!strcmp(chain->str, str))
        {
            chain->refcount--;
            if(chain->refcount == 0)
            {
                if(prev)
                {
                    prev->next = chain->next;
                }
                else
                {
                    bucket->chain = chain->next;
                }
                free(chain);
            }
            break;
        }
        prev = chain;
        chain = chain->next;
    }
}

void metacache_ref(const char *str)
{
    uint32_t *refc = (uint32_t *)(str - 5);
    *refc++;
}

void metacache_unref(const char *str)
{
    uint32_t *refc = (uint32_t *)(str - 5);
    *refc--;
}

///////////////////////////////////////////////////////////////////////
#define MAX_TEXT_FRAME_SIZE 10000
#define MAX_CUESHEET_FRAME_SIZE 10000
#define MAX_APEV2_FRAME_SIZE 2000000
#define MAX_ID3V2_FRAME_SIZE 100000
#define MAX_ID3V2_APIC_FRAME_SIZE 2000000

#define FRAME_MAPPINGS 5

enum
{
    MAP_DDB = 0,
    MAP_ID3V23 = 1,
    MAP_ID3V24 = 2,
    MAP_ID3V22 = 3,
    MAP_APEV2 = 4
};

// map of known id3v2 and apev2 text tags
// order: ddb, id3-2.3, 2.4, 2.2, apev2
static const char *frame_mapping[] = {
// these tags will be displayed and edited uniformly for all tag types
    "artist", "TPE1", "TPE1", "TP1", "Artist",
    "disc", "TPOS", "TPOS", "TPA", "Disc", // NOTE: this is special case when writing id3v2
    "title", "TIT2", "TIT2", "TT2", "Title",
    "album", "TALB", "TALB", "TAL", "Album",
    "copyright", "TCOP", "TCOP", "TCO", "Copyright",
    "genre", "TCON", "TCON", "TCO", "Genre",
    "composer", "TCOM", "TCOM", "TCM", "Composer",
    "year", "TYER", "TDRC", "TYE", "Year", // NOTE: TDRC and TYER are slightly different, and are converted on read/write
    "track", "TRCK", "TRCK", "TRK", "Track", // NOTE: this is special case when writing id3v2
// misc id3v2 fields
// these might or might not have appropriate fields in every tag type
    "BAND", "TPE2", "TPE2", "TP2", NULL,
    "ENCODER", "TENC", "TENC", "TEN", NULL,
    "BEATS_PER_MINUTE", "TBPM", "TBPM", "TBP", NULL,
    "PLAYLIST_DELAY", "TDLY", "TDLY", "TDY", NULL,
    "TEXT_WRITERS", "TEXT", "TEXT", "TXT", NULL,
    "FILE_TYPE", "TFLT", "TFLT", "TFT", NULL,
    "CONTENT_GROUP_DESCRIPTION", "TIT1", "TIT1", "TT1", NULL,
    "SUBTITLE", "TIT3", "TIT3", "TT3", NULL,
    "INITIAL_KEY", "TKEY", "TKEY", "TKE", NULL,
    "LANGUAGES", "TLAN", "TLAN", "TLA", NULL,
    "LENGTH", "TLEN", "TLEN", "TLE", NULL,
    "MEDIA_TYPE", "TMED", "TMED", "TMT", NULL,
    "ORIGINAL_ALBUM_TITLE", "TOAL", "TOAL", "TOT", NULL,
    "ORIGINAL_FILENAME", "TOFN", "TOFN", "TOF", NULL,
    "ORIGINAL_TEXT_WRITERS", "TOLY", "TOLY", "TOL", NULL,
    "ORIGINAL_ARTISTS", "TOPE", "TOPE", "TOA", NULL,
    "FILE_OWNER", "TOWN", "TOWN", NULL, NULL,
    "PERFORMER_REFINEMENT", "TPE3", "TPE3", "TP3", NULL,
    "MODIFIED_BY", "TPE4", "TPE4", "TP4", NULL,
    "PUBLISHER", "TPUB", "TPUB", "TPB", NULL,
    "INTERNET_RADIO_STATION_NAME", "TRSN", "TRSN", NULL, NULL,
    "INTERNET_RADIO_STATION_OWNER", "TRSO", "TRSO", NULL, NULL,
    "ISRC", "TSRC", "TSRC", NULL, NULL,
    "ENCODING_SOFTWARE_HARDWARE", "TSSE", "TSSE", "TSS", NULL,
    "RECORDING_TIME", NULL, "TDRC", NULL, NULL,
    "RELEASE_TIME", NULL, "TDRL", NULL, NULL,
    "TAGGING_TIME", NULL, "TDTG", NULL, NULL,
    "ALBUM_SORT_ORDER", NULL, "TSOA", NULL, NULL,
    "PERFORMER_SORT_ORDER", NULL, "TSOP", NULL, NULL,
    "TITLE_SORT_ORDER", NULL, "TSOT", NULL, NULL,
    "SIZE", "TSIZ", NULL, "TSI", NULL,
    "RECORDING_DATES", "TRDA", NULL, "TRD", NULL,
    "INVOLVED_PEOPLE_LIST", NULL, "TIPL", NULL, NULL,
    "MUSICIAN_CREDITS_LIST", NULL, "TMCL", NULL, NULL,
    "ENCODING_TIME", NULL, "TDEN", NULL, NULL,
    "ORIGINAL_RELEASE_TIME", NULL, "TDOR", NULL, NULL,
    "ORIGINAL_RELEASE_YEAR", NULL, NULL, NULL, "ORIGINALYEAR",
    "MOOD", NULL, "TMOO", NULL, NULL,
    "PRODUCED_NOTICE", NULL, "TPRO", NULL, NULL,
    "musicbrainz_trackid", NULL, NULL, NULL, NULL,
    NULL
};

#define ID3V1_GENRE_COUNT (sizeof(stdio_genretbl) / sizeof(char *) - 1)

static const char *stdio_genretbl[] = {
    "Blues",
    "Classic Rock",
    "Country",
    "Dance",
    "Disco",
    "Funk",
    "Grunge",
    "Hip-Hop",
    "Jazz",
    "Metal",
    "New Age",
    "Oldies",
    "Other",
    "Pop",
    "R&B",
    "Rap",
    "Reggae",
    "Rock",
    "Techno",
    "Industrial",
    "Alternative",
    "Ska",
    "Death Metal",
    "Pranks",
    "Soundtrack",
    "Euro-Techno",
    "Ambient",
    "Trip-Hop",
    "Vocal",
    "Jazz+Funk",
    "Fusion",
    "Trance",
    "Classical",
    "Instrumental",
    "Acid",
    "House",
    "Game",
    "Sound Clip",
    "Gospel",
    "Noise",
    "AlternRock",
    "Bass",
    "Soul",
    "Punk",
    "Space",
    "Meditative",
    "Instrumental Pop",
    "Instrumental Rock",
    "Ethnic",
    "Gothic",
    "Darkwave",
    "Techno-Industrial",
    "Electronic",
    "Pop-Folk",
    "Eurodance",
    "Dream",
    "Southern Rock",
    "Comedy",
    "Cult",
    "Gangsta",
    "Top 40",
    "Christian Rap",
    "Pop/Funk",
    "Jungle",
    "Native American",
    "Cabaret",
    "New Wave",
    "Psychedelic",
    "Rave",
    "Showtunes",
    "Trailer",
    "Lo-Fi",
    "Tribal",
    "Acid Punk",
    "Acid Jazz",
    "Polka",
    "Retro",
    "Musical",
    "Rock & Roll",
    "Hard Rock",
    "Folk",
    "Folk-Rock",
    "National Folk",
    "Swing",
    "Fast Fusion",
    "Bebob",
    "Latin",
    "Revival",
    "Celtic",
    "Bluegrass",
    "Avantgarde",
    "Gothic Rock",
    "Progressive Rock",
    "Psychedelic Rock",
    "Symphonic Rock",
    "Slow Rock",
    "Big Band",
    "Chorus",
    "Easy Listening",
    "Acoustic",
    "Humour",
    "Speech",
    "Chanson",
    "Opera",
    "Chamber Music",
    "Sonata",
    "Symphony",
    "Booty Bass",
    "Primus",
    "Porn Groove",
    "Satire",
    "Slow Jam",
    "Club",
    "Tango",
    "Samba",
    "Folklore",
    "Ballad",
    "Power Ballad",
    "Rhythmic Soul",
    "Freestyle",
    "Duet",
    "Punk Rock",
    "Drum Solo",
    "Acapella",
    "Euro-House",
    "Dance Hall",
    "Goa",
    "Drum & Bass",
    "Club-House",
    "Hardcore",
    "Terror",
    "Indie",
    "BritPop",
    "Negerpunk",
    "Polsk Punk",
    "Beat",
    "Christian Gangsta",
    "Heavy Metal",
    "Black Metal",
    "Crossover",
    "Contemporary C",
    "Christian Rock",
    "Merengue",
    "Salsa",
    "Thrash Metal",
    "Anime",
    "JPop",
    "SynthPop",
    "Abstract",
    "Art Rock",
    "Baroque",
    "Bhangra",
    "Big Beat",
    "Breakbeat",
    "Chillout",
    "Downtempo",
    "Dub",
    "EBM",
    "Eclectic",
    "Electro",
    "Electroclash",
    "Emo",
    "Experimental",
    "Garage",
    "Global",
    "IDM",
    "Illbient",
    "Industro-Goth",
    "Jam Band",
    "Krautrock",
    "Leftfield",
    "Lounge",
    "Math Rock",
    "New Romantic",
    "Nu-Breakz",
    "Post-Punk",
    "Post-Rock",
    "Psytrance",
    "Shoegaze",
    "Space Rock",
    "Trop Rock",
    "World Music",
    "Neoclassical",
    "Audiobook",
    "Audio Theatre",
    "Neue Deutsche Welle",
    "Podcast",
    "Indie Rock",
    "G-Funk",
    "Dubstep",
    "Garage Rock",
    "Psybient",
    NULL
};


uint32_t extract_i32_le(unsigned char *buf)
{
    uint32_t x;
    // little endian extract
    x = buf[3];
    x <<= 8;
    x |= buf[2];
    x <<= 8;
    x |= buf[1];
    x <<= 8;
    x |= buf[0];
    return x;
}

void str_trim_right(uint8_t *str, int len)
{
    uint8_t *p = str + len - 1;
    while(p >= str && *p <= 0x20)
    {
        p--;
    }
    p++;
    *p = 0;
}

int stdio_apev2_add_frame(metaInfo_t *it, apev2_tag_t *tag_store, apev2_frame_t **tail, uint32_t itemsize, uint32_t itemflags, const char *key, const uint8_t *value)
{
    if(tag_store)
    {
        apev2_frame_t *frm = malloc(sizeof(apev2_frame_t) + itemsize);
        memset(frm, 0, sizeof(apev2_tag_t));
        frm->flags = itemflags;
        strcpy(frm->key, key);
        frm->size = itemsize;
        memcpy(frm->data, value, itemsize);

        if(*tail)
        {
            (*tail)->next = frm;
        }
        else
        {
            tag_store->frames = frm;
        }
        *tail = frm;
    }

    if(it)
    {
        int valuetype = ((itemflags >> 1) & 3);
        // add metainfo only if it's textual
        if(valuetype == 0 && (itemsize < MAX_TEXT_FRAME_SIZE || (!strcasecmp(key, "cuesheet") && itemsize < MAX_CUESHEET_FRAME_SIZE)))
        {
            if(!u8_valid(value, itemsize, NULL))
            {
                return -1;
            }

            int m;
            for(m = 0; frame_mapping[m]; m += FRAME_MAPPINGS)
            {
                if(frame_mapping[m + MAP_APEV2] && !strcasecmp(key, frame_mapping[m + MAP_APEV2]))
                {
                    if(!strcmp(frame_mapping[m + MAP_DDB], "track"))
                    {
                        stdio_add_track_meta(it, value);
                    }
                    else
                    {
                        stdio_append_meta(it, frame_mapping[m + MAP_DDB], value);
                    }
                    break;
                }
            }

            if(!frame_mapping[m])
            {
                if(strncasecmp(key, "replaygain_album_gain", 21) &&
                   strncasecmp(key, "replaygain_album_peak", 21) &&
                   strncasecmp(key, "replaygain_track_gain", 21) &&
                   strncasecmp(key, "replaygain_track_peak", 21))
                {
                    stdio_append_meta(it, key, value);
                }
            }
        }
    }
    return 0;
}

int stdio_add_track_meta(metaInfo_t *it, const char *track)
{
    char *slash = strchr(track, '/');
    if(slash)
    {
        // split into track/number
        *slash = 0;
        slash++;
        stdio_add_meta(it, "numtracks", slash);
    }
    stdio_add_meta(it, "track", track);
    return 0;
}

int stdio_add_disc_meta(metaInfo_t *it, const char *disc)
{
    char *slash = strchr(disc, '/');
    if(slash)
    {
        // split into track/number
        *slash = 0;
        slash++;
        stdio_add_meta(it, "numdiscs", slash);
    }
    stdio_add_meta(it, "disc", disc);
    return 0;
}

int stdio_load_comm_frame(int version_major, metaInfo_t *it, uint8_t *readptr, int synched_size)
{
    uint8_t enc = readptr[0];

    char *descr = convstr_id3v2(version_major, enc, readptr+4, synched_size-4);
    if(!descr)
    {
        return -1;
    }

    // find value
    char *value = descr;
    while(*value && *value != '\n')
    {
        value++;
    }

    if(*value != '\n')
    {
        free(descr);
        return -1;
    }

    *value = 0;
    value++;

    int len = strlen(descr) + strlen(value) + 3;
    char comment[len];

    if(*descr)
    {
        snprintf(comment, len, "%s: %s", descr, value);
    }
    else
    {
        strcpy(comment, value);
    }

    // skip utf8 BOM (can be produced by iconv FEFF/FFFE)
    int l = strlen(comment);
    uint8_t bom[] = { 0xEF, 0xBB, 0xBF };
    if(l >= 3 && !memcmp(comment, bom, 3))
    {
        stdio_append_meta(it, "comment", comment+3);
    }
    else
    {
        stdio_append_meta(it, "comment", comment);
    }

    free(descr);
    return 0;
}

int stdio_id3v2_sync_frame(uint8_t *data, int size)
{
    char *writeptr = data;
    int written = 0;
    while(size > 0)
    {
        *writeptr = *data;
        if(data[0] == 0xff && size >= 2 && data[1] == 0)
        {
            data++;
            size--;
        }
        writeptr++;
        data++;
        size--;
        written++;
    }
    return written;
}

int stdio_id3v2_add_genre(metaInfo_t *it, char *genre)
{
    int numeric = 0;
    if(genre[0] == '(')
    {
        // find matching parenthesis
        char *p = &genre[1];
        while(*p && *p != ')')
        {
            if(!isdigit(*p))
            {
                break;
            }
            p++;
        }

        if(*p == ')')
        {
            *p = 0;
            memmove(genre, genre+1, p-genre);
            numeric = 1;
        }
    }

    if(!numeric)
    {
        // check if it is numeric
        const char *p = genre;
        while(*p)
        {
            if(!isdigit(*p))
            {
                break;
            }
            p++;
        }

        if(*p == 0 && p > genre)
        {
            numeric = 1;
        }
    }

    if(numeric)
    {
        int genre_id = atoi(genre);
        if(genre_id >= 0)
        {
            const char *genre_str = NULL;
            if(genre_id < ID3V1_GENRE_COUNT)
            {
                genre_str = stdio_genretbl[genre_id];
            }
            else if(genre_id == 0xff)
            {
                // genre_str = "None";
            }

            if(genre_str)
            {
                stdio_add_meta(it, "genre", genre_str);
                return 0;
            }
        }
    }
    else if(!strcmp(genre, "CR"))
    {
        stdio_add_meta(it, "genre", "Cover");
    }
    else if(!strcmp(genre, "RX"))
    {
        stdio_add_meta(it, "genre", "Remix");
    }
    else
    {
        stdio_add_meta(it, "genre", genre);
    }

    return 0;
}

int stdio_id3v2_load_txx(int version_major, metaInfo_t *it, uint8_t *readptr, int synched_size)
{
    char *txx = convstr_id3v2(version_major, *readptr, readptr+1, synched_size-1);
    if(!txx)
    {
        return -1;
    }

    char *val = NULL;
    if(txx)
    {
        char *p;
        for(p = txx; *p; p++)
        {
            if(*p == '\n')
            {
                *p = 0;
                val = p+1;
                break;
            }
        }
    }

    if(val)
    {
        // skip utf8 BOM (can be produced by iconv FEFF/FFFE)
        int l = strlen(val);
        uint8_t bom[] = { 0xEF, 0xBB, 0xBF };
        if(l >= 3 && !memcmp(val, bom, 3))
        {
            val += 3;
        }

        if(strcasecmp(txx, "replaygain_album_gain") &&
           strcasecmp(txx, "replaygain_album_peak") &&
           strcasecmp(txx, "replaygain_track_gain") &&
           strcasecmp(txx, "replaygain_track_peak"))
        {
            if(!strcasecmp(txx, "date"))
            {
                // HACK: fb2k date support
                stdio_append_meta(it, "year", val);
            }
            else
            {
                stdio_append_meta(it, txx, val);
            }
        }
    }

    free(txx);

    return 0;
}

int stdio_id3v2_load_rva2(int version_major, metaInfo_t *it, uint8_t *readptr, int synched_size)
{
    uint8_t *rva_desc = readptr;
    unsigned rva_desc_len = 0;
    const uint8_t *p = rva_desc;
    while(*p++ && rva_desc_len < synched_size)
    {
        rva_desc_len++;
    }

    if(rva_desc_len == synched_size)
    { /* tag too short */
        return -1;
    }

    if(rva_desc_len != 5)
    { /* only support track or album labeled ones */
        return 0;
    }

    if(synched_size < rva_desc_len + 1 + 4)
    {
        return -1; /* at least 4 bytes after zero-terminated label */
    }

    uint8_t *rva_data = rva_desc + rva_desc_len + 1;
    uint8_t vol_type = rva_data[0];
    if(vol_type != 1)
    {
        return 0;
    }

    uint8_t peak_bits = rva_data[3];
    uint16_t peak_val = 0;

    if(peak_bits == 16 && synched_size >= rva_desc_len + 1 + 6)
    {
        peak_val = (uint16_t)((rva_data[4] << 8) | rva_data[5]);
    }

    return 0;
}

int stdio_id3v2_load_ufid(int version_major, metaInfo_t *it, uint8_t *readptr, int synched_size)
{
    char *owner = readptr;
    while(*readptr && synched_size > 0)
    {
        readptr++;
        synched_size--;
    }

    if(!synched_size)
    {
        return -1;
    }

    readptr++;
    synched_size--;
    char id[synched_size+1];
    memcpy(id, readptr, synched_size);
    id[synched_size] = 0;

    // verify that owner is musicbrainz and that content is ascii
    if(strcmp(owner, "http://musicbrainz.org"))
    {
        return -1;
    }

    int i;
    for(i = 0; i < synched_size; i++)
    {
        if(!isascii(id[i]))
        {
            return -1;
        }
    }

    stdio_replace_meta(it, "musicbrainz_trackid", id);
    return 0;
}

const char *stdio_detect_charset(const char *s)
{
    size_t len = strlen(s);
    return stdio_detect_charset_len(s, len);
}

const char *stdio_detect_charset_len(const char *s, int len)
{
    // check if that's already utf8
    if(u8_valid(s, len, NULL))
    {
        return NULL; // means no recoding required
    }
    // try shift-jis
    if(can_be_shift_jis(s, len))
    {
        return "shift-jis";
    }
    // hack to add cp936 support
    if(can_be_chinese(s, len))
    {
       return "cp936";
    }
    // check if that could be non-latin1 (too many nonascii chars)
    if(can_be_russian(s, len))
    {
        return "cp1251";
    }
    return "cp1252";
}

const char *stdio_find_meta_raw(metaInfo_t *it, const char *key)
{
    metaInfo_t *m = it;
    while(m)
    {
        if(m->key && !strcasecmp(key, m->key))
        {
            return m->value;
        }
        m = m->next;
    }
    return NULL;
}

void stdio_add_meta(metaInfo_t *it, const char *key, const char *value)
{
    if(!value || !*value)
    {
        return;
    }

    // check if it's already set
    metaInfo_t *tail = NULL;
    metaInfo_t *m = it;
    while(m)
    {
        if(m->key && !strcasecmp(key, m->key))
        {
            // duplicate key
            return;
        }
        // find end of properties
        tail = m;
        m = m->next;
    }
    // add
    m = malloc(sizeof(metaInfo_t));
    memset(m, 0, sizeof(metaInfo_t));
    m->key = metacache_add_string(key);
    m->value = metacache_add_string(value);

    if(!tail)
    {
        free(m);
        return;
    }
    tail->next = m;
}

void stdio_append_meta(metaInfo_t *it, const char *key, const char *value)
{
    const char *old = stdio_find_meta_raw(it, key);

    if(old && (!strcasecmp(key, "cuesheet") || !strcasecmp(key, "log")))
    {
        return;
    }

    size_t newlen = strlen(value);
    if(!old)
    {
        stdio_add_meta(it, key, value);
    }
    else
    {
        // check for duplicate data
        const char *str = old;
        size_t len;
        while(str)
        {
            char *next = strchr(str, '\n');

            if(next)
            {
                len = next - str;
                next++;
            }
            else
            {
                len = strlen(str);
            }

            if(len == newlen && !memcmp(str, value, len))
            {
                return;
            }

            str = next;
        }
        size_t sz = strlen(old) + newlen + 2;
        char out[sz];
        snprintf(out, sz, "%s\n%s", old, value);
        stdio_replace_meta(it, key, out);
    }
}

void stdio_replace_meta(metaInfo_t *it, const char *key, const char *value)
{
    // check if it's already set
    metaInfo_t *m = it;
    while(m)
    {
        if(m->key && !strcasecmp(key, m->key))
        {
            break;
        }
        m = m->next;
    }

    if(m)
    {
        metacache_remove_string(m->value);
        m->value = metacache_add_string(value);
        return;
    }
    else
    {
        stdio_add_meta(it, key, value);
    }
}

int stdio_apev2_read(metaInfo_t *it, apev2_tag_t *tag_store, FILE *fp)
{
    // try to read footer, position must be already at the EOF right before
    // id3v1 (if present)
    apev2_frame_t *tail = NULL;

    uint8_t header[32];
    if(stdio_seek(fp, -32, SEEK_END) == -1)
    {
        return -1; // something bad happened
    }

    if(stdio_read(header, 1, 32, fp) != 32)
    {
        return -1; // something bad happened
    }

    if(strncmp(header, "APETAGEX", 8))
    {
        // try to skip 128 bytes backwards (id3v1)
        if(stdio_seek(fp, -128-32, SEEK_END) == -1)
        {
            return -1; // something bad happened
        }

        if(stdio_read(header, 1, 32, fp) != 32)
        {
            return -1; // something bad happened
        }

        if(strncmp(header, "APETAGEX", 8))
        {
            return -1; // no ape tag here
        }
    }

    uint32_t version = extract_i32_le(&header[8]);
    int32_t size = extract_i32_le(&header[12]);
    uint32_t numitems = extract_i32_le(&header[16]);
    uint32_t flags = extract_i32_le(&header[20]);

    // now seek to beginning of the tag (exluding header)
    if(stdio_seek(fp, -size, SEEK_CUR) == -1)
    {
        return -1;
    }

    int i;
    for(i = 0; i < numitems; i++)
    {
        uint8_t buffer[8];
        if(stdio_read(buffer, 1, 8, fp) != 8)
        {
            return -1;
        }
        uint32_t itemsize = extract_i32_le(&buffer[0]);
        uint32_t itemflags = extract_i32_le(&buffer[4]);

        // read key until 0 (stupid and slow)
        char key[256];
        int keysize = 0;
        while(keysize <= 255)
        {
            if(stdio_read(&key[keysize], 1, 1, fp) != 1)
            {
                return -1;
            }

            if(key[keysize] == 0)
            {
                break;
            }

            if(key[keysize] < 0x20)
            {
                return -1; // non-ascii chars and chars with codes 0..0x1f not allowed in ape item keys
            }
            keysize++;
        }

        key[255] = 0;
        // read value
        if(itemsize <= MAX_APEV2_FRAME_SIZE) // just a sanity check
        {
            uint8_t *value = malloc(itemsize + 1);
            if(!value)
            {
                return -1;
            }
            if(stdio_read(value, 1, itemsize, fp) != itemsize)
            {
                free(value);
                return -1;
            }
            value[itemsize] = 0;

            if((flags & 6) == 0 && strncasecmp(key, "cover art ", 10))
            {
                // replace 0s with \n
                uint8_t *p = value;
                while(p < value + itemsize - 1)
                {
                    if(*p == 0)
                    {
                        *p = '\n';
                    }
                    p++;
                }
            }

            stdio_apev2_add_frame(it, tag_store, &tail, itemsize, itemflags, key, value);
            free(value);
        }
        else
        {
            // try to skip
            int err = stdio_seek(fp, itemsize, SEEK_CUR);
            if(0 != err)
            {
                return -1;
            }
        }
    }

    return 0;
}

int stdio_id3v2_read(metaInfo_t *it, id3v2_tag_t *tag_store, FILE *fp)
{
    id3v2_frame_t *tail = NULL;
    if(!fp)
    {
        return -1;
    }

    stdio_rewind (fp);
    uint8_t header[10];
    if(stdio_read(header, 1, 10, fp) != 10)
    {
        return -1; // too short
    }

    if(strncmp(header, "ID3", 3))
    {
        return -1; // no tag
    }

    uint8_t version_major = header[3];
    uint8_t version_minor = header[4];

    if(version_major > 4 || version_major < 2)
    {
        return -1; // unsupported
    }

    uint8_t flags = header[5];
    if(flags & 15) {
        return -1; // unsupported
    }
    int unsync = (flags & (1<<7)) ? 1 : 0;
    int extheader = (flags & (1<<6)) ? 1 : 0;

    // check for bad size
    if((header[9] & 0x80) || (header[8] & 0x80) || (header[7] & 0x80) || (header[6] & 0x80))
    {
        return -1; // bad header
    }

    uint32_t size = (header[9] << 0) | (header[8] << 7) | (header[7] << 14) | (header[6] << 21);
    if(size == 0)
    {
        return -1;
    }

    if(tag_store)
    {
        tag_store->version[0] = version_major;
        tag_store->version[1] = version_minor;
        tag_store->flags = flags;
        // remove unsync flag
        tag_store->flags &= ~ (1<<7);
    }

    uint8_t *tag = malloc(size);
    if(!tag)
    {
        goto error;
    }

    if(stdio_read(tag, 1, size, fp) != size)
    {
        goto error; // bad size
    }
    uint8_t *readptr = tag;

    if(extheader)
    {
        uint32_t sz = (readptr[3] << 0) | (readptr[2] << 7) | (readptr[1] << 14) | (readptr[0] << 21);
        if(size < sz)
        {
            goto error; // bad size
        }
        readptr += sz;
    }

    int err = -1;
    while(readptr - tag <= size - 4 && *readptr)
    {
        if(version_major == 3 || version_major == 4)
        {
            char frameid[5];
            memcpy(frameid, readptr, 4);
            frameid[4] = 0;
            readptr += 4;
            if(readptr - tag >= size - 4)
            {
                break;
            }

            uint32_t sz;
            if(version_major == 4)
            {
                sz = (readptr[3] << 0) | (readptr[2] << 7) | (readptr[1] << 14) | (readptr[0] << 21);
            }
            else if(version_major == 3) {
                sz = (readptr[3] << 0) | (readptr[2] << 8) | (readptr[1] << 16) | (readptr[0] << 24);
            }
            else
            {
                goto error;
            }

            readptr += 4;
            if(readptr - tag >= size - sz)
            {
                err = -1;
                goto error; // size of frame is more than size of tag
            }

            if(sz < 1)
            {
                break; // frame must be at least 1 byte long
            }

            uint8_t flags1 = readptr[0];
            uint8_t flags2 = readptr[1];
            readptr += 2;

            if(!strcmp(frameid, "APIC"))
            {
                if(sz > MAX_ID3V2_APIC_FRAME_SIZE)
                {
                    readptr += sz;
                    continue;
                }
            }
            else if(sz > MAX_ID3V2_FRAME_SIZE || readptr - tag + sz > size)
            {
                readptr += sz;
                continue;
            }

            int synched_size = sz;
            if(unsync)
            {
                synched_size = stdio_id3v2_sync_frame(readptr, sz);
            }

            if(tag_store)
            {
                id3v2_frame_t *frm = malloc(sizeof(id3v2_frame_t) + sz);
                if(!frm)
                {
                    goto error;
                }
                memset(frm, 0, sizeof(id3v2_frame_t));
                if(tail)
                {
                    tail->next = frm;
                }
                tail = frm;

                if(!tag_store->frames)
                {
                    tag_store->frames = frm;
                }
                strcpy(frm->id, frameid);
                memcpy(frm->data, readptr, sz);
                frm->size = synched_size;

                frm->flags[0] = flags1;
                frm->flags[1] = flags2;
            }

            if(version_major == 4)
            {
                if(flags1 & 0x8f)
                {
                    // unknown flags
                    readptr += sz;
                    continue;
                }
                if(flags2 & 0xb0)
                {
                    // unknown flags
                    readptr += sz;
                    continue;
                }

                if(flags2 & 0x40)
                { // group id
                    readptr++; // skip id
                    sz--;
                }
                if(flags2 & 0x08)
                { // compressed frame, ignore
                    readptr += sz;
                    continue;
                }
                if(flags2 & 0x04)
                { // encrypted frame, skip
                    readptr += sz;
                    continue;
                }
                if(flags2 & 0x02)
                { // unsync, just do nothing
                }
                if(flags2 & 0x01)
                { // data size
                    synched_size = (readptr[3] << 0) | (readptr[2] << 7) | (readptr[1] << 14) | (readptr[0] << 21);
                    readptr += 4;
                    sz -= 4;
                }
            }
            else if(version_major == 3)
            {
                if(flags1 & 0x1F)
                {
                    readptr += sz;
                    continue;
                }
                if(flags2 & 0x1F)
                {
                    readptr += sz;
                    continue;
                }
                if(flags2 & 0x80)
                {
                    readptr += sz;
                    continue;
                }
                if(flags2 & 0x40)
                {
                    readptr += sz;
                    continue;
                }
                if(flags2 & 0x20)
                {
                    readptr++; // skip id
                    sz--;
                }
            }

            // parse basic 2.3/2.4 text frames
            //const char *text_frames[] = { "TPE1", "TPE2", "TPOS", "TIT2", "TALB", "TCOP", "TCON", "TENC", "TPE3", "TCOM", "TRCK", "TYER", "TDRC", NULL };
            //char **text_holders[] = { &artist, &band, &disc, &title, &album, &copyright, &genre, &vendor, &performer, &composer, &track, version_major == 3 ? &year : NULL,  version_major == 4 ? &year : NULL, };
            if(it)
            {
                int added = 0;
                if(strcmp(frameid, "TXXX"))
                {
                    int f;
                    for(f = 0; frame_mapping[f]; f += FRAME_MAPPINGS)
                    {
                        const char *frm_name = version_major == 3 ? frame_mapping[f+MAP_ID3V23] : frame_mapping[f+MAP_ID3V24];
                        if(frm_name && !strcmp(frameid, frm_name))
                        {
                            added = 1;
                            if(synched_size > MAX_TEXT_FRAME_SIZE)
                            {
                                break;
                            }

                            char *text = convstr_id3v2(version_major, readptr[0], readptr+1, synched_size-1);

                            // couple of simple tests
                            //char *text = convstr_id3v2(4, 3, "текст1\0текст2", strlen("текст1")*2+2);
                            //const char ucstext[] = { 0x42, 0x04, 0x35, 0x04, 0x3a, 0x04, 0x41, 0x04, 0x42, 0x04, 0x31, 0x00, 0x00, 0x00, 0x42, 0x04, 0x35, 0x04, 0x3a, 0x04, 0x41, 0x04, 0x42, 0x04, 0x32, 0x00 };
                            //char *text = convstr_id3v2(4, 1, ucstext, sizeof(ucstext));

                            if(text && *text)
                            {
                                if(!strcmp(frameid, "TRCK"))
                                { // special case for track/totaltracks
                                    stdio_add_track_meta(it, text);
                                }
                                if(!strcmp(frameid, "TPOS"))
                                { // special case for disc/totaldiscs
                                    stdio_add_disc_meta(it, text);
                                }
                                else if(!strcmp(frameid, "TCON"))
                                {
                                    stdio_id3v2_add_genre(it, text);
                                }
                                else
                                {
                                    stdio_append_meta(it, frame_mapping[f + MAP_DDB], text);
                                }
                            }

                            if(text)
                            {
                                free(text);
                            }
                            break;
                        }
                    }
                }

                if(added)
                {
                    readptr += sz;
                    continue;
                }

                if(!strcmp(frameid, "COMM"))
                {
                    if(sz < 4)
                    {
                        readptr += sz; // bad tag
                        continue;
                    }
                    stdio_load_comm_frame(version_major, it, readptr, synched_size);
                }
                else if(it && !strcmp(frameid, "RVA2"))
                {
                    if(synched_size < 5)
                    {
                        readptr += sz; // bad tag
                        continue;
                    }
                    stdio_id3v2_load_rva2(version_major, it, readptr, synched_size);
                }
                else if(it && !strcmp(frameid, "UFID"))
                {
                    if(synched_size < 2)
                    {
                        readptr += sz; // bad tag
                        continue;
                    }
                    stdio_id3v2_load_ufid(version_major, it, readptr, synched_size);
                }
                else if(it && !strcmp(frameid, "TXXX"))
                {
                    if(synched_size < 2)
                    {
                        readptr += sz; // bad tag
                        continue;
                    }
                    stdio_id3v2_load_txx(version_major, it, readptr, synched_size);
                }
            }
            readptr += sz;
        }
        else if(version_major == 2)
        {
            char frameid[4];
            memcpy(frameid, readptr, 3);
            frameid[3] = 0;
            readptr += 3;
            if(readptr - tag >= size - 3)
            {
                break;
            }

            uint32_t sz = (readptr[2] << 0) | (readptr[1] << 8) | (readptr[0] << 16);
            readptr += 3;
            if(readptr - tag >= size - sz)
            {
                break; // size of frame is less than size of tag
            }
            if(sz < 1)
            {
                break; // frame must be at least 1 byte long
            }
            if(sz > MAX_ID3V2_FRAME_SIZE)
            {
                readptr += sz;
                continue;
            }
            int synched_size = sz;
            if(unsync)
            {
                synched_size = stdio_id3v2_sync_frame(readptr, sz);
            }

            if(tag_store)
            {
                id3v2_frame_t *frm = malloc(sizeof(id3v2_frame_t) + sz);
                if(!frm)
                {
                    goto error;
                }
                memset(frm, 0, sizeof(id3v2_frame_t));
                if(tail)
                {
                    tail->next = frm;
                }
                tail = frm;
                if(!tag_store->frames)
                {
                    tag_store->frames = frm;
                }
                strcpy(frm->id, frameid);
                memcpy(frm->data, readptr, synched_size);
                frm->size = sz;
            }

            // parse basic 2.2 text frames
            if(it)
            {
                int added = 0;
                if(strcmp(frameid, "TXX"))
                {
                    int f;
                    for(f = 0; frame_mapping[f]; f++)
                    {
                        if(frame_mapping[f+MAP_ID3V22] && !strcmp(frameid, frame_mapping[f+MAP_ID3V22]))
                        {
                            added = 1;
                            if(synched_size > MAX_TEXT_FRAME_SIZE)
                            {
                                break;
                            }

                            char *text = convstr_id3v2(version_major, readptr[0], readptr+1, synched_size-1);
                            if(text && *text)
                            {
                                if(!strcmp(frameid, "TRK"))
                                { // special case for track/totaltracks
                                    stdio_add_track_meta(it, text);
                                }
                                if(!strcmp(frameid, "TPA"))
                                { // special case for disc/totaldiscs
                                    stdio_add_disc_meta(it, text);
                                }
                                else if(!strcmp(frameid, "TCO"))
                                {
                                    stdio_id3v2_add_genre(it, text);
                                }
                                else
                                {
                                    stdio_append_meta(it, frame_mapping[f + MAP_DDB], text);
                                }
                                free(text);
                            }
                            break;
                        }
                    }
                }

                if(added)
                {
                    readptr += sz;
                    continue;
                }

                if(!strcmp(frameid, "COM"))
                {
                    if(synched_size < 6)
                    {
                        readptr += sz;
                        continue;
                    }
                    stdio_load_comm_frame(version_major, it, readptr, synched_size);
                }
                else if(it && !strcmp(frameid, "TXX"))
                {
                    if(synched_size < 2)
                    {
                        readptr += sz; // bad tag
                        continue;
                    }
                    stdio_id3v2_load_txx(version_major, it, readptr, synched_size);
                }
            }
            readptr += sz;
        }
    }
    err = 0;
error:
    if(tag)
    {
        free(tag);
    }

    if(tag_store && err != 0)
    {
        while(tag_store->frames)
        {
            id3v2_frame_t *next = tag_store->frames->next;
            free(tag_store->frames);
            tag_store->frames = next;
        }
    }
    return err;
}

int stdio_id3v1_read(metaInfo_t *it, FILE *fp)
{
    uint8_t id3[128];
    if(stdio_seek(fp, -128, SEEK_END) == -1)
    {
        return -1;
    }
    if(stdio_read(id3, 1, 128, fp) != 128)
    {
        return -1;
    }
    return stdio_id3v1_read_init(it, id3, NULL);
}

int stdio_id3v1_read_init(metaInfo_t *it, char *buffer, const char **charset)
{
    if(!buffer)
    {
        return -1;
    }

    if(it)
    {
        if(memcmp(buffer, "TAG", 3))
        {
            return -1; // no tag
        }
        const char *cs = NULL;
        charset = &cs;
        int res = stdio_id3v1_read_init(NULL, buffer, charset);
        if(res)
        {
            return res;
        }
    }

    if(!charset)
    {
        return -1;
    }

    char title[31];
    char artist[31];
    char album[31];
    char year[5];
    char comment[31];
    uint8_t genreid;
    uint8_t tracknum;
    const char *genre = NULL;

    memset(title, 0, 31);
    memset(artist, 0, 31);
    memset(album, 0, 31);
    memset(year, 0, 5);
    memset(comment, 0, 31);

    memcpy(title, &buffer[3], 30);
    str_trim_right(title, 30);
    memcpy(artist, &buffer[3+30], 30);
    str_trim_right(artist, 30);
    memcpy(album, &buffer[3+60], 30);
    str_trim_right(album, 30);
    memcpy(year, &buffer[3+90], 4);
    str_trim_right(year, 4);
    memcpy(comment, &buffer[3+94], 30);
    str_trim_right(comment, 30);
    genreid = buffer[3+124];
    tracknum = 0;

    if(it)
    {
        if(comment[28] == 0 && comment[29] != 0)
        {
            tracknum = comment[29];
        }

        // 255 = "None",
        // "CR" = "Cover" (id3v2)
        // "RX" = "Remix" (id3v2)
        if(genreid == 0xff)
        {
            //genre = "None";
        }
        else if(genreid < ID3V1_GENRE_COUNT)
        {
            genre = stdio_genretbl[genreid];
        }
    }

    // add meta
    if(!it)
    {
        char buf[129];
        char *p = buf;
        strcpy(p, title);
        p += strlen(title);
        strcpy(p, artist);
        p += strlen(artist);
        strcpy(p, album);
        p += strlen(album);
        strcpy(p, year);
        p += strlen(year);
        strcpy(p, comment);
        *charset = stdio_detect_charset(buf);
        return 0;
    }

    char utf8_value[150];

    if(*title)
    {
        stdio_add_meta(it, "title", convstr_id3v1(title, (int)strlen(title), *charset, utf8_value, sizeof(utf8_value)));
    }

    if(*artist)
    {
        stdio_add_meta(it, "artist", convstr_id3v1(artist, (int)strlen(artist), *charset, utf8_value, sizeof(utf8_value)));
    }

    if(*album)
    {
        stdio_add_meta(it, "album", convstr_id3v1(album, (int)strlen(album), *charset, utf8_value, sizeof(utf8_value)));
    }

    if(*year)
    {
        stdio_add_meta(it, "year", convstr_id3v1(year, (int)strlen(year), *charset, utf8_value, sizeof(utf8_value)));
    }

    if(*comment)
    {
        stdio_add_meta(it, "comment", convstr_id3v1(comment, (int)strlen(comment), *charset, utf8_value, sizeof(utf8_value)));
    }

    if(genre && *genre)
    {
        stdio_add_meta(it, "genre", convstr_id3v1(genre, (int)strlen(genre), *charset, utf8_value, sizeof(utf8_value)));
    }

    if(tracknum != 0)
    {
        char s[4];
        snprintf(s, 4, "%d", tracknum);
        stdio_add_meta(it, "track", s);
    }

    return 0;
}
