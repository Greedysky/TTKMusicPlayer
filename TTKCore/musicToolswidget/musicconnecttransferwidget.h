#ifndef MUSICCONNECTTRANSFERWIDGET_H
#define MUSICCONNECTTRANSFERWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicsong.h"
#include "musicabstractmovedialog.h"

namespace Ui {
class MusicConnectTransferWidget;
}

/*! @brief The class of the transfer file to mobile widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicConnectTransferWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicConnectTransferWidget(QWidget *parent = 0);
    virtual ~MusicConnectTransferWidget();

Q_SIGNALS:
    void getMusicLists(MusicSongsList &songs, QStringList &names);
    /*!
     * Get music datas from container.
     */

public Q_SLOTS:
    void currentPlayListSelected(int index);
    /*!
     * Current playList selected.
     */
    void selectedAllItems(bool check);
    /*!
     * Select all items.
     */
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    void initColumns();

    Ui::MusicConnectTransferWidget *ui;

};

#endif // MUSICCONNECTTRANSFERWIDGET_H
