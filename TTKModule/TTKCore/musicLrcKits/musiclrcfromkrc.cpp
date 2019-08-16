#include "musiclrcfromkrc.h"
#include "musicnumberdefine.h"
#include "musicobject.h"
#include "musiccodecutils.h"

#ifdef Q_CC_GNU
#   pragma GCC diagnostic ignored "-Wwrite-strings"
#   pragma GCC diagnostic ignored "-Wsign-compare"
#endif

#include <QFile>
#include <sys/stat.h>

#include "zlib/zconf.h"
#include "zlib/zlib.h"

const wchar_t key[] = { L'@', L'G', L'a', L'w', L'^', L'2',
                        L't', L'G', L'Q', L'6', L'1', L'-',
                        L'Î', L'Ò', L'n', L'i'};

MusicLrcFromKrc::MusicLrcFromKrc()
{
    m_resultBytes = new uchar[MH_MB2B*MH_B2BS];
}

MusicLrcFromKrc::~MusicLrcFromKrc()
{
    delete[] m_resultBytes;
}

bool MusicLrcFromKrc::decode(const QString &input, const QString &output)
{
    FILE *fp = nullptr;
    struct stat st;
    size_t dstsize;

    if((fp = fopen(MusicUtils::Codec::toLocal8Bit(input), "rb")) == nullptr)
    {
        M_LOGGER_ERROR("open file error !");
        return false;
    }

    if(fstat(fileno(fp), &st))
    {
        M_LOGGER_ERROR("fstat file error !");
        fclose(fp);
        return false;
    }

    uchar *src = new uchar[st.st_size];
    if(fread(src, sizeof(uchar), st.st_size, fp) != st.st_size)
    {
        M_LOGGER_ERROR("fread file error !");
        delete[] src;
        fclose(fp);
        return false;
    }

    if(memcmp(src, "krc1", 4) != 0)
    {
        M_LOGGER_ERROR("error file format !");
        delete[] src;
        fclose(fp);
        return false;
    }

    src += 4;
    for(int i = 0; i < st.st_size; i++)
    {
        src[i] = (uchar)(src[i] ^ key[i % 16]);
    }

    decompression(src, st.st_size, &dstsize);
    createLrc(m_resultBytes, MStatic_cast(int, dstsize));

    delete[] src;
    fclose(fp);

    if(!output.isEmpty())
    {
        QFile file(output);
        if(file.open(QIODevice::WriteOnly))
        {
            QTextStream outstream(&file);
            outstream.setCodec("utf-8");
            outstream << m_data << endl;
            file.close();
        }
    }
    return true;
}

QByteArray MusicLrcFromKrc::getDecodeString() const
{
    return m_data;
}

int MusicLrcFromKrc::sncasecmp(char *s1, char *s2, size_t n)
{
    uint c1, c2;
    while(n)
    {
        c1 = (uint) *s1++;
        c2 = (uint) *s2++;

        c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
        c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;

        if(c1 == c2)
        {
            if(c1)
            {
                n--;
                continue;
            }
            return 0;
        }
        return c1 - c2;
    }
    return 0;
}

int MusicLrcFromKrc::decompression(uchar *src, size_t srcsize, size_t *dstsize)
{
    *dstsize = MH_MB2B*MH_B2BS;
    if(Z_OK != uncompress(m_resultBytes, (uLongf*)dstsize, src, srcsize))
    {
        return -1;
    }
    return 0;
}

int MusicLrcFromKrc::isfilter(char *tok)
{
    if(!sncasecmp(tok, const_cast<char*>(MString("[id").c_str()), 3))
    {
        return 1;
    }
    else if(!sncasecmp(tok, const_cast<char*>(MString("[by").c_str()), 3))
    {
        return 1;
    }
    else if(!sncasecmp(tok, const_cast<char*>(MString("[hash").c_str()), 5))
    {
        return 1;
    }
    else if(!sncasecmp(tok, const_cast<char*>(MString("[al").c_str()), 3))
    {
        return 1;
    }
    else if(!sncasecmp(tok, const_cast<char*>(MString("[sign").c_str()), 5))
    {
        return 1;
    }
    else if(!sncasecmp(tok, const_cast<char*>(MString("[total").c_str()), 6))
    {
        return 1;
    }
    else if(!sncasecmp(tok, const_cast<char*>(MString("[offset").c_str()), 7))
    {
        return 1;
    }
    return 0;
}

void MusicLrcFromKrc::createLrc(uchar *lrc, int lrclen)
{
    m_data.clear();
    int top = 0;
    for(int i = 0; i<lrclen; i++)
    {
        int len;
        if(top == 0)
        {
            switch(lrc[i])
            {
                case '<':
                    top++;
                    break;
                case '[':
                    len = (strchr((char*)&lrc[i], ']') - (char*)&lrc[i]) + 1;
                    for(int j = 0; j<len; j++)
                    {
                        if(lrc[i+j] == ':')
                        {
                            if(isfilter((char*)&lrc[i]))
                            {
                                while(lrc[++i] != '\n' && i < lrclen)
                                {

                                }
                            }
                            goto filter_done;
                        }
                    }

                    for(int j = 0; j<len; j++)
                    {
                        int ms;
                        if(lrc[i + j] == ',')
                        {
                            char ftime[14];
                            lrc[i + j] = 0;
                            ms = atoi((char*)&lrc[i + 1]);
                            sprintf(ftime, "[%.2d:%.2d.%.2d]", (ms % MT_H2MS) / MT_M2MS, (ms % MT_M2MS) / MT_S2MS, (ms % MT_M2MS) % 100);

                            for(j = 0; j < 10; j++)
                            {
                                m_data.append(ftime[j]);
                            }
                            i = i + len - 1;
                            break;
                        }
                    }
                    break;
        filter_done:
                default:
                    m_data.append(lrc[i]);
                    break;
            }

        }
        else if(top == 1 && lrc[i] == '>')
        {
            top--;
        }
    }
}
