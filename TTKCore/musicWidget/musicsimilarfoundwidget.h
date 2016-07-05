#ifndef MUSICSIMILARFOUNDWIDGET_H
#define MUSICSIMILARFOUNDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#ifndef USE_MULTIPLE_QUERY
#  include "musicdownloadquerysinglethread.h"
#else
#  include "musicdownloadquerymultiplethread.h"
#endif

class QCheckBox;

/*! @brief The class of similar music found widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSimilarFoundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSimilarFoundWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicSimilarFoundWidget();

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
    void downLoadFinished(const QByteArray &data);
    /*!
     * Send recieved data from net.
     */
    void selectAllItems(bool all);
    /*!
     * Select all items or not.
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
    /*!
     * Override the widget event.
     */
    void createLabels();
    /*!
     * Create init interface lables.
     */
    MusicObject::MIntList foundCheckedItem();
    /*!
     * Found which items is checked.
     */
    void downloadDataFrom(bool play);
    /*!
     * Download data from net and just play or not.
     */

    QString m_songNameFull;
    QWidget *m_mainWindow;
    QLabel *m_statusLabel;
    QList<QCheckBox*> m_checkBoxs;
    QList<QLabel*> m_iconLabels;
    QList<DownloadData*> m_likeDownloadDatas;
    MusicDownLoadQueryThreadAbstract *m_downloadThread;

};

#endif // MUSICSIMILARFOUNDWIDGET_H
