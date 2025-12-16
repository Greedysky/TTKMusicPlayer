#include "qmmptextcodec.h"
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#  include <iconv.h>
#  include <errno.h>
#else
#  include <QTextCodec>
#endif

class QmmpTextCodecPrivate
{
public:
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    ~QmmpTextCodecPrivate()
    {
        if(to)
            iconv_close(to);
        if(from)
            iconv_close(from);
    }

    iconv_t to = nullptr, from = nullptr;
#else
    QTextCodec *m_codec = nullptr;
#endif
    QByteArray m_name;
};


QmmpTextCodec::QmmpTextCodec(const QByteArray &charset)
    : d_ptr(new QmmpTextCodecPrivate)
{
    Q_D(QmmpTextCodec);
    d->m_name = charset.toUpper();

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    if(d->m_name == "UTF-8" || d->m_name == "UTF-16")
        return;

    if((d->to = iconv_open(d->m_name.constData(), "UTF-16")) == (iconv_t)(-1))
    {
        qWarning("error: %s", strerror(errno));
        d->to = nullptr;
    }

    if((d->from = iconv_open("UTF-16", d->m_name.constData())) == (iconv_t)(-1))
    {
        qWarning("error: %s", strerror(errno));
        d->from = nullptr;
    }
#else
    d->m_codec = QTextCodec::codecForName(d->m_name);
#endif
}

QmmpTextCodec::~QmmpTextCodec()
{
    delete d_ptr;
}

QByteArray QmmpTextCodec::name() const
{
    Q_D(const QmmpTextCodec);
    return d->m_name;
}

QString QmmpTextCodec::toUnicode(const QByteArray &a) const
{
    Q_D(const QmmpTextCodec);

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    if(d->m_name == "UTF-16")
        return QString::fromUtf16(reinterpret_cast<const char16_t*>(a.data()), a.size() / 2);
    if(!d->from || d->m_name == "UTF-8")
        return QString::fromUtf8(a);

    size_t inBytesLeft = 0;
    size_t outBytesLeft = 0;

    // reset state
    iconv(d->from, nullptr, &inBytesLeft, nullptr, &outBytesLeft);

    char *inBytes = const_cast<char*>(a.data());
    inBytesLeft = a.size();
    outBytesLeft = a.size() * 2 + 2;
    QByteArray ba(outBytesLeft, Qt::Uninitialized);
    char *outBytes = ba.data();
    outBytesLeft = ba.size();

    while(inBytesLeft > 0)
    {
        size_t ret = iconv(d->from, &inBytes, &inBytesLeft, &outBytes, &outBytesLeft);

        if(ret == (size_t) -1)
        {
            if(errno == E2BIG) //increase buffer size
            {
                int offset = ba.size() - outBytesLeft;
                ba.resize(ba.size() * 2);
                outBytes = ba.data() + offset;
                outBytesLeft = ba.size() - offset;
                continue;
            }

            if(errno == EINVAL)
                break;

            if(errno == EILSEQ)
            {
                // skip the next character
                ++inBytes;
                --inBytesLeft;
                continue;
            }

            //fallback
            return QString::fromLatin1(a);
        }
    }

    return QString::fromUtf16(reinterpret_cast<const char16_t*>(ba.constData()), (ba.size() - outBytesLeft) / 2);
#else
    return d->m_codec->toUnicode(a);
#endif
}

QString QmmpTextCodec::toUnicode(const char *chars) const
{
    return toUnicode(QByteArray(chars));
}

QByteArray QmmpTextCodec::fromUnicode(const QString &str) const
{
    Q_D(const QmmpTextCodec);

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    if(d->m_name == "UTF-16")
        return QByteArray(reinterpret_cast<const char*>(str.utf16()), str.size() * 2);
    if(!d->from || d->m_name == "UTF-8")
        return str.toUtf8();

    size_t inBytesLeft = 0;
    size_t outBytesLeft = 0;

    // reset state
    iconv(d->to, nullptr, &inBytesLeft, nullptr, &outBytesLeft);

    char *inBytes = const_cast<char*>(reinterpret_cast<const char*>(str.utf16()));
    inBytesLeft = str.size() * 2;
    outBytesLeft = str.size() * 2;

    QByteArray ba(outBytesLeft, Qt::Uninitialized);
    char *outBytes = ba.data();
    outBytesLeft = ba.size();

    while(inBytesLeft > 0)
    {
        size_t ret = iconv(d->to, &inBytes, &inBytesLeft, &outBytes, &outBytesLeft);

        if(ret == (size_t) -1)
        {
            if(errno == E2BIG) //increase buffer size
            {
                int offset = ba.size() - outBytesLeft;
                ba.resize(ba.size() * 2);
                outBytes = ba.data() + offset;
                outBytesLeft = ba.size() - offset;
                continue;
            }

            if(errno == EINVAL)
                break;

            if(errno == EILSEQ)
            {
                // skip the next character
                ++inBytes;
                --inBytesLeft;
                continue;
            }

            //fallback
            return str.toLatin1();
        }
    }

    ba.resize(ba.size() - outBytesLeft);
    return ba;
#else
    return d->m_codec->fromUnicode(str);
#endif
}

QStringList QmmpTextCodec::availableCharsets()
{
//#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    static const QStringList charsets = {
        "BIG5",
        "EUC-JP",
        "EUC-KR",
        "GB18030",
        "GBK",
        "IBM866",
        "ISO-2022-JP",
        "ISO-8859-10",
        "ISO-8859-13",
        "ISO-8859-14",
        "ISO-8859-15",
        "ISO-8859-16",
        "ISO-8859-1",
        "ISO-8859-2",
        "ISO-8859-3",
        "ISO-8859-4",
        "ISO-8859-5",
        "ISO-8859-6",
        "ISO-8859-7",
        "ISO-8859-8",
        "ISO-8859-8-I",
        "KOI8-R",
        "KOI8-U",
        "MACINTOSH",
        "SHIFT_JIS",
        "UTF-32",
        "UTF-32LE",
        "UTF-32BE",
        "UTF-16",
        "UTF-16LE",
        "UTF-16BE",
        "UTF-8",
        "WINDOWS-1250",
        "WINDOWS-1251",
        "WINDOWS-1252",
        "WINDOWS-1253",
        "WINDOWS-1254",
        "WINDOWS-1255",
        "WINDOWS-1256",
        "WINDOWS-1257",
        "WINDOWS-1258",
        "WINDOWS-874"
    };
//#else
//    QStringList charsets;
//    for(const QByteArray &str : QTextCodec::availableCodecs())
//    {
//        charsets << QString::fromUtf8(str.data()).toUpper();
//    }
//#endif
    return charsets;
}
