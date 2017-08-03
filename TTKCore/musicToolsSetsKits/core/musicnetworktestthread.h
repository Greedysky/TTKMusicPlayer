#ifndef MUSICNETWORKTESTTHREAD_H
#define MUSICNETWORKTESTTHREAD_H

#include <QThread>
#include "musicglobaldefine.h"

/*! @brief The class of thread to test input url network.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SUPER_EXPORT MusicNetworkTestThread : public QThread
{
    Q_OBJECT
public:
    explicit MusicNetworkTestThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicNetworkTestThread();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void stopAndQuitThread();
    /*!
     * Stop and quit current thread.
     */
    void setUrl(const QString &url);
    /*!
     * Set current test url.
     */

Q_SIGNALS:
    void networkConnectionTestChanged(bool state);
    /*!
     * Network connection test changed.
     */

public Q_SLOTS:
    void start();
    /*!
     * Strat thread now.
     */
    virtual void run() override;
    /*!
     * Thread run now.
     */

protected:
    QString m_currentUrl;
    bool m_run;

};

#endif // MUSICNETWORKTESTTHREAD_H
