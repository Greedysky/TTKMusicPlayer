#ifndef MUSICALBUMFOUNDWIDGET_H
#define MUSICALBUMFOUNDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include "musicabstracttablewidget.h"
#include "musicdownloadquerythreadabstract.h"

class MusicQueryTableDelegate;

/*! @brief The class of the album music found table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicAlbumFoundTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicAlbumFoundTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicAlbumFoundTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    MusicObject::MIntList getSelectedItems() const;
    /*!
     * Get selected items.
     */

public Q_SLOTS:
    void listCellClicked(int row, int column);
    /*!
     * Subclass should implement this function.
     */
    void clearAllItems();
    /*!
     * Clear All Items.
     */
    void createSearchedItems(const QString &songname, const QString &artistname,
                             const QString &time);
    /*!
     * Create searched items.
     */
    void setSelectedAllItems(bool all);
    /*!
     * Set select all items.
     */

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    /*!
     * Override the widget event.
     */

    MusicQueryTableDelegate *m_checkBoxDelegate;

};


/*! @brief The class of album music found widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicAlbumFoundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicAlbumFoundWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicAlbumFoundWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setSongName(const QString &name);
    /*!
     * Set current name to search founds.
     */

Q_SIGNALS:
    void muiscSongToPlayListChanged(const QString &name, const QString &time,
                                    const QString &format, bool play);
    /*!
     * Add current network music to download to local.
     */

public Q_SLOTS:
    void queryAllFinished();
    /*!
     * Query all quality musics is finished.
     */
    void queryAlbumFinished();
    /*!
     * Query album musics is finished.
     */
    void downLoadFinished(const QByteArray &data);
    /*!
     * Send recieved data from net.
     */
    void playAllButtonClicked();
    /*!
     * Play all button clicked now.
     */
    void shareButtonClicked();
    /*!
     * Share button clicked now.
     */
    void playButtonClicked();
    /*!
     * Play button clicked now.
     */
    void downloadButtonClicked();
    /*!
     * Download button clicked now.
     */
    void addButtonClicked();
    /*!
     * Add button clicked now.
     */

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    /*!
     * Override the widget event.
     */
    void createNoAlbumLabel();
    /*!
     * Create no album label widget.
     */
    void downloadDataFrom(bool play);
    /*!
     * Download data from net and just play or not.
     */

    QString m_songNameFull;
    QWidget *m_mainWindow;
    QLabel *m_statusLabel, *m_iconLabel;
    QList<QLabel*> m_resizeWidget;
    MusicAlbumFoundTableWidget *m_albumTableWidget;
    MusicDownLoadQueryThreadAbstract *m_downloadThread, *m_albumThread;
    MusicObject::MusicSongInfomations m_albumDatas;

};

#endif // MUSICALBUMFOUNDWIDGET_H
