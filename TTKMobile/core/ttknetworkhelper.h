#ifndef TTKNETWORKHELPER_H
#define TTKNETWORKHELPER_H

#include <QObject>

class MusicDownLoadQueryMultipleThread;

class TTKNetworkHelper : public QObject
{
    Q_OBJECT
public:
    explicit TTKNetworkHelper(QObject *parent = 0);
    ~TTKNetworkHelper();

    Q_INVOKABLE void searchSong(const QString &text);
    Q_INVOKABLE void setCurrentIndex(int index);
    /*!
     * Set current play index.
     */

Q_SIGNALS:
    void clearAllItems();
    /*!
     * Clear all items before the new query start.
     */
    void createSearchedItems(const QString &songname, const QString &artistname);
    /*!
     * Create the current items by song name\ artist name and time.
     */
    void downloadFinished(const QString &path);

public slots:
    void searchDataDwonloadFinished();

protected:
    MusicDownLoadQueryMultipleThread *m_queryThread;

    int m_currentIndex;

};

#endif // TTKNETWORKHELPER_H
