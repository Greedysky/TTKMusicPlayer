#include "musiccodecutils.h"
#include "musicobject.h"

#include <QTextCodec>

QString MusicUtils::Codec::toUnicode(const char *chars, const char *format)
{
    QTextCodec *codec = QTextCodec::codecForName(format);
    return codec->toUnicode(chars);
}

QString MusicUtils::Codec::toUnicode(const QByteArray &chars, const char *format)
{
    QTextCodec *codec = QTextCodec::codecForName(format);
    return codec->toUnicode(chars);
}

QByteArray MusicUtils::Codec::fromUnicode(const QString &chars, const char *format)
{
    QTextCodec *codec = QTextCodec::codecForName(format);
    return codec->fromUnicode(chars);
}

void MusicUtils::Codec::setLocalCodec(const char *format)
{
    QTextCodec *codec = QTextCodec::codecForName(format);
    QTextCodec::setCodecForLocale(codec);
#ifndef TTK_GREATER_NEW
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif
}

const char* MusicUtils::Codec::toLocal8Bit(const QString &str)
{
    return str.toLocal8Bit().constData();
}

const char* MusicUtils::Codec::toUtf8(const QString &str)
{
    return str.toUtf8().constData();
}
