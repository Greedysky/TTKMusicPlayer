#ifndef MUSICNETWORKTESTTHREAD_H
#define MUSICNETWORKTESTTHREAD_H

#include <QObject>
#include "musicglobaldefine.h"

/*! @brief The class of thread to test input url network.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicNetworkTestThread : public QObject
{
    Q_OBJECT
public:
    explicit MusicNetworkTestThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setUrl(const QString &url);
    /*!
     * Set current test url.
     */

    void start();
    /*!
     * Start to test input url.
     */

Q_SIGNALS:
    void networkConnectionTestChanged(bool state);
    /*!
     * Network connection test changed.
     */

protected:
    QString m_currentUrl;

};

#endif // MUSICNETWORKTESTTHREAD_H
