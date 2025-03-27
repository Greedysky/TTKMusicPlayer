#include "musiclrcparser.h"
#include "ttkabstractxml.h"

#include <QFile>
#include <sys/stat.h>

#include "zlib/zconf.h"
#include "zlib/zlib.h"

const QByteArray &MusicLrcFromInterface::data() const noexcept
{
    return m_data;
}


bool MusicLrcFromPlain::decode(const QString &input)
{
    QFile file(input);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    m_data = file.readAll();
    file.close();
    return true;
}


static constexpr wchar_t key[] = {
    L'@', L'G', L'a', L'w', L'^', L'2',
    L't', L'G', L'Q', L'6', L'1', L'-',
    L'Î', L'Ò', L'n', L'i'
};


MusicLrcFromKrc::MusicLrcFromKrc()
    : MusicLrcFromInterface(),
      m_resultBytes(new uchar[TTK_SN_MB2BT])
{

}

MusicLrcFromKrc::~MusicLrcFromKrc()
{
    delete[] m_resultBytes;
}

bool MusicLrcFromKrc::decode(const QString &input)
{
    return decode(input, {});
}

bool MusicLrcFromKrc::decode(const QString &input, const QString &output)
{
    FILE *fp = nullptr;
    struct stat st;
    size_t dstsize;

    if((fp = fopen(qPrintable(input), "rb")) == nullptr)
    {
        TTK_ERROR_STREAM("Open file error");
        return false;
    }

    if(fstat(fileno(fp), &st))
    {
        TTK_ERROR_STREAM("Fstat file error");
        fclose(fp);
        return false;
    }

    uchar *src = new uchar[st.st_size];
    uchar *ptr = src;

    if(fread(src, sizeof(uchar), st.st_size, fp) != (size_t)st.st_size)
    {
        TTK_ERROR_STREAM("Fread file error");
        delete[] src;
        fclose(fp);
        return false;
    }

    if(memcmp(src, "krc1", 4) != 0)
    {
        TTK_ERROR_STREAM("Error file format");
        delete[] src;
        fclose(fp);
        return false;
    }

    src += 4;
    for(int i = 0; i < st.st_size; ++i)
    {
        src[i] = (uchar)(src[i] ^ key[i % 16]);
    }

    decompression(src, st.st_size, &dstsize);
    createLrc(m_resultBytes, TTKStaticCast(int, dstsize));

    delete[] ptr;
    fclose(fp);

    if(!output.isEmpty())
    {
        QFile file(output);
        if(file.open(QIODevice::WriteOnly))
        {
            QTextStream outstream(&file);
            outstream.setCodec("UTF-8");
            outstream << m_data;
            outstream << QtNamespace(endl);
            file.close();
        }
    }
    return true;
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
    *dstsize = TTK_SN_MB2BT;
    if(Z_OK != uncompress(m_resultBytes, (uLongf*)dstsize, src, srcsize))
    {
        return -1;
    }
    return 0;
}

int MusicLrcFromKrc::isfilter(char *tok)
{
    if(!sncasecmp(tok, TTKConstCast(char*, TTKString("[id").c_str()), 3))
    {
        return 1;
    }
    else if(!sncasecmp(tok, TTKConstCast(char*, TTKString("[by").c_str()), 3))
    {
        return 1;
    }
    else if(!sncasecmp(tok, TTKConstCast(char*, TTKString("[hash").c_str()), 5))
    {
        return 1;
    }
    else if(!sncasecmp(tok, TTKConstCast(char*, TTKString("[al").c_str()), 3))
    {
        return 1;
    }
    else if(!sncasecmp(tok, TTKConstCast(char*, TTKString("[sign").c_str()), 5))
    {
        return 1;
    }
    else if(!sncasecmp(tok, TTKConstCast(char*, TTKString("[total").c_str()), 6))
    {
        return 1;
    }
    else if(!sncasecmp(tok, TTKConstCast(char*, TTKString("[offset").c_str()), 7))
    {
        return 1;
    }
    return 0;
}

void MusicLrcFromKrc::createLrc(uchar *lrc, int lrclen)
{
    m_data.clear();
    int top = 0;
    for(int i = 0; i < lrclen; ++i)
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
                    for(int j = 0; j < len; ++j)
                    {
                        if(lrc[i + j] == ':')
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

                    for(int j = 0; j < len; ++j)
                    {
                        int ms;
                        if(lrc[i + j] == ',')
                        {
                            char ftime[14];
                            lrc[i + j] = 0;
                            ms = atoi((char*)&lrc[i + 1]);
                            sprintf(ftime, "[%.2d:%.2d.%.2d]", (ms % TTK_DN_H2MS) / TTK_DN_M2MS, (ms % TTK_DN_M2MS) / TTK_DN_S2MS, (ms % TTK_DN_M2MS) % 100);

                            for(j = 0; j < 10; ++j)
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


bool MusicLrcFromQrc::decode(const QString &input)
{
    QFile file(input);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    m_data = file.readAll();
    file.close();

    TTKAbstractXml xml;
    if(xml.fromByteArray(m_data))
    {
        const QString &data = xml.readAttributeByTagName("Lyric_1", "LyricContent");
        if(!data.isEmpty())
        {
            m_data.clear();

            for(QString &text : data.split(TTK_LINEFEED))
            {
                const QRegExp regx("\\[(\\d+),\\d+\\]");
                if(regx.indexIn(text) != -1)
                {
                    text.replace(regx, "[" + TTKTime::toString(regx.cap(1).toInt(), "mm:ss.zzz") + "]");
                }

                text.remove(QRegExp("\\(\\d+,\\d+\\)"));
                m_data.append(text.toUtf8() + TTK_LINEFEED);
            }
        }
    }
    return true;
}


bool MusicLrcFromTrc::decode(const QString &input)
{
    QFile file(input);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    m_data = file.readAll();
    file.close();

    const QString &data = QString::fromUtf8(m_data);
    m_data.clear();

    for(QString &text : data.split(TTK_LINEFEED))
    {
        text.remove(QRegExp("<\\d+>"));
        m_data.append(text.toUtf8() + TTK_LINEFEED);
    }
    return true;
}


bool MusicLrcFromYrc::decode(const QString &input)
{
    QFile file(input);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    m_data = file.readAll();
    file.close();

    const QString &data = QString::fromUtf8(m_data);
    m_data.clear();

    for(QString &text : data.split(TTK_LINEFEED))
    {
        if(text.startsWith("{")) // json info
        {
            continue;
        }

        const QRegExp regx("\\[(\\d+),\\d+\\]");
        if(regx.indexIn(text) != -1)
        {
            text.replace(regx, "[" + TTKTime::toString(regx.cap(1).toInt(), "mm:ss.zzz") + "]");
        }

        text.remove(QRegExp("\\(\\d+,\\d+,\\d+\\)"));
        m_data.append(text.toUtf8() + TTK_LINEFEED);
    }
    return true;
}

