#ifndef TTKMUSICUTILS_H
#define TTKMUSICUTILS_H

#include <QObject>

#include "musicmobileglobaldefine.h"

/*! @brief The class of the music utils.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_MOBILE_EXPORT TTKMusicUtils : public QObject
{
    Q_OBJECT
public:
    explicit TTKMusicUtils(QObject *parent = 0);

    Q_INVOKABLE QString getRoot() const;
    Q_INVOKABLE QString normalizeTime(qint64 time, const QString &format);

};

#endif // TTKMUSICUTILS_H
