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
