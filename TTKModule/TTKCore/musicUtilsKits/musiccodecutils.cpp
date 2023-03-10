#include "musiccodecutils.h"

#include <QTextCodec>

QString TTK::Codec::toUnicode(const char *chars, const char *format)
{
    QTextCodec *codec = QTextCodec::codecForName(format);
    return codec ? codec->toUnicode(chars) : QString();
}

QString TTK::Codec::toUnicode(const QByteArray &chars, const char *format)
{
    QTextCodec *codec = QTextCodec::codecForName(format);
    return codec ? codec->toUnicode(chars) : QString();
}

QByteArray TTK::Codec::fromUnicode(const QString &chars, const char *format)
{
    QTextCodec *codec = QTextCodec::codecForName(format);
    return codec ? codec->fromUnicode(chars) : QByteArray();
}

void TTK::Codec::setLocalCodec(const char *format)
{
    QTextCodec *codec = QTextCodec::codecForName(format);
    QTextCodec::setCodecForLocale(codec);
#if !TTK_QT_VERSION_CHECK(5,0,0)
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif
}
