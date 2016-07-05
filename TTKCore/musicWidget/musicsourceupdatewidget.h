#ifndef MUSICSOURCEUPDATEWIDGET_H
#define MUSICSOURCEUPDATEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

#define VERSION_URL     "http://7xpa0g.com1.z0.glb.clouddn.com/version"
#define DOWNLOAD_URL    "http://7xpa0g.com1.z0.glb.clouddn.com/"
#define CSDN_URL        "http://download.csdn.net/album/detail/3094"

#define DD_TYPE_ZIP     ".zip"
#define LAST_UPDATE_URL "TTKUpdate.exe"
#define NEW_UPDATE_URL  "TTKUpdate.exe"

namespace Ui {
class MusicSourceUpdateWidget;
}

/*! @brief The class of the application upgrade widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSourceUpdateWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicSourceUpdateWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSourceUpdateWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

public Q_SLOTS:
    void upgradeButtonClicked();
    /*!
     * Upgrade button clicked.
     */
    void upgradeFailedClicked();
    /*!
     * Upgrade failed clicked.
     */
    void downLoadFinished(const QByteArray &data);
    /*!
     * Download data from kuwo net finished.
     */
    void downloadProgressChanged(float percent, const QString &total);
    /*!
     * Update download percent\ total size progress.
     */
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    Ui::MusicSourceUpdateWidget *ui;
    QString m_newVersionStr;

};

#endif // MUSICSOURCEUPDATEWIDGET_H
