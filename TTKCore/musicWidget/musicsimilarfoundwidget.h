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

    void setSongName(const QString &name);
    /*!
     * Set current name to search founds.
     */

public Q_SLOTS:
    void queryAllFinished();
    /*!
     * Query all quality musics is finished.
     */
    void recievedData(const QByteArray &data);
    /*!
     * Send recieved data from net.
     */
    void selectAllItems(bool all);
    void playButtonClicked();
    void downloadButtonClicked();
    void addButtonClicked();

protected:
    void createLabels();
    QList<int> foundCheckedItem();

    QWidget *m_mainWindow;
    QLabel *m_statusLabel;
    QList<QCheckBox*> m_checkBoxs;
    QList<DownloadData*> m_likeDownloadDatas;
    MusicDownLoadQueryThreadAbstract *m_downloadThread;

};

#endif // MUSICSIMILARFOUNDWIDGET_H
