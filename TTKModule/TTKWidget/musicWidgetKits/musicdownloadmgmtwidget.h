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
    /*!
     * Object contsructor.
     */
    explicit MusicDownloadResetWidget(QWidget *parent = 0);

    ~MusicDownloadResetWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set current name to search and download musics.
     */
    void setSongName(const QString &name);

Q_SIGNALS:
    /*!
     * To open stacked download widget once.
     */
    void openStackedDownloadWidget();

public Q_SLOTS:
    /*!
     * Override show function.
     */
    void show();
    /*!
     * Restart to dwonload music data.
     */
    void restartToDownload();
    /*!
     * Open file details information by song.
     */
    void openDetailInfo();
    /*!
     * Open file location in dir.
     */
    void openFileLocation();

protected:
    Ui::MusicDownloadResetWidget* m_ui;

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
    /*!
     * Object contsructor.
     */
    explicit MusicDownloadMgmtWidget(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set current name to search and download musics.
     */
    void setSongName(const QString &name, MusicDownLoadQueryThreadAbstract::QueryType type);

protected:
    QWidget *m_parentClass;

};

#endif // MUSICDOWNLOADMGMTWIDGET_H
