#ifndef TTKMUSICUTILS_H
#define TTKMUSICUTILS_H

#include <QObject>

class TTKMusicUtils : public QObject
{
    Q_OBJECT
public:
    explicit TTKMusicUtils(QObject *parent = 0);

    Q_INVOKABLE QString normalizeTime(qint64 time, const QString &format);

};

#endif // TTKMUSICUTILS_H
