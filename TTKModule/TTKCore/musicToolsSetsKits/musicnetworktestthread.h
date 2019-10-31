#ifndef MUSICNETWORKTESTTHREAD_H
#define MUSICNETWORKTESTTHREAD_H

#include "musicabstractthread.h"

/*! @brief The class of thread to test input url network.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicNetworkTestThread : public MusicAbstractThread
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicNetworkTestThread)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicNetworkTestThread(QObject *parent = nullptr);

    /*!
     * Set current test url.
     */
    void setUrl(const QString &url);

Q_SIGNALS:
    /*!
     * Network connection test changed.
     */
    void networkConnectionTestChanged(bool state);

protected:
    /*!
     * Thread run now.
     */
    virtual void run() override;

protected:
    QString m_currentUrl;

};

#endif // MUSICNETWORKTESTTHREAD_H
