#ifndef MUSICCONNECTTRANSFERTABLEWIDGET_H
#define MUSICCONNECTTRANSFERTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicsong.h"
#include "musicfillitemtablewidget.h"

/*! @brief The class of the transfer file list table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicConnectTransferTableWidget : public MusicFillItemTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicConnectTransferTableWidget(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Create all items.
     */
    void createAllItems(const MusicSongs &songs);
    /*!
     * Select all items.
     */
    void selectedAllItems(bool check);

};

#endif // MUSICCONNECTTRANSFERTABLEWIDGET_H
