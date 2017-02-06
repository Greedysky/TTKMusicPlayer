#ifndef MUSICDOWNLOADMGMTWIDGET_H
#define MUSICDOWNLOADMGMTWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovewidget.h"
#include "musicdownloadquerythreadabstract.h"

namespace Ui{
class MusicDownloadResetWidget;
}

/*! @brief The class of the download file exist widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicDownloadResetWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    explicit MusicDownloadResetWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicDownloadResetWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setSongName(const QString &name);
    /*!
     * Set current name to search and download musics.
     */

Q_SIGNALS:
    void openStackedDownloadWidget();
    /*!
     * To open stacked download widget once.
     */

public Q_SLOTS:
    void show();
    /*!
     * Override show function.
     */
    void restartToDownload();
    /*!
     * Restart to dwonload music data.
     */
    void openDetailInfo();
    /*!
     * Open file details information by song.
     */
    void openFileLocation();
    /*!
     * Open file location in dir.
     */

protected:
    Ui::MusicDownloadResetWidget* ui;

    QWidget *m_parentClass;
    QString m_currentName;

};


/*! @brief The class of the download manager widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicDownloadMgmtWidget : public QObject
{
    Q_OBJECT
public:
    explicit MusicDownloadMgmtWidget(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setSongName(const QString &name, MusicDownLoadQueryThreadAbstract::QueryType type);
    /*!
     * Set current name to search and download musics.
     */

protected:
    QWidget *m_parentClass;

};

#endif // MUSICDOWNLOADMGMTWIDGET_H
