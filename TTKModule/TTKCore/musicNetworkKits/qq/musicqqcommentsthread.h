#ifndef MUSICQQCOMMENTSTHREAD_H
#define MUSICQQCOMMENTSTHREAD_H

#include "musicdownloadcommentsthread.h"

/*! @brief The class to qq query song comments download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicQQCommentsThread : public MusicDownLoadCommentsThread
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicQQCommentsThread(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Start to Search data from name.
     */
    virtual void startToSearch(const QString &name) override;
    /*!
     * Start to search data from name and type bt paging.
     */
    virtual void startToPage(int offset) override;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;

};

#endif // MUSICQQCOMMENTSTHREAD_H
