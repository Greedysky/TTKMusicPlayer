#ifndef TTKFILESEARCHCORE_H
#define TTKFILESEARCHCORE_H

#include <QThread>
#include <QFileInfoList>
#include "musicmobileglobaldefine.h"

/*! @brief The class of the file search core.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_MOBILE_EXPORT TTKFileSearchCore : public QThread
{
    Q_OBJECT
public:
    explicit TTKFileSearchCore(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~TTKFileSearchCore();

    Q_INVOKABLE void search(const QStringList &path);
    Q_INVOKABLE void search(const QString &path);

Q_SIGNALS:
    void finished(const QStringList &path);
    void findFilePath(const QString &path);

public Q_SLOTS:
    void start();
    /*!
     * Strat thread now.
     */
    void run();
    /*!
     * Thread run now.
     */

private:
    void stopAndQuitThread();
    QFileInfoList findFile(const QString &path, const QStringList &filter);

    bool m_run;
    QStringList m_path;

};

#endif // TTKFILESEARCHCORE_H
