#ifndef MUSICQUERYFOUNDTABLEWIDGET_H
#define MUSICQUERYFOUNDTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicquerytablewidget.h"

/*! @brief The class of the query found table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicQueryFoundTableWidget : public MusicQueryTableWidget
{
    Q_OBJECT
public:
    explicit MusicQueryFoundTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicQueryFoundTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void startSearchQuery(const QString &text) override;
    /*!
     * Start search query by text.
     */
    virtual void musicDownloadLocal(int row) override;
    /*!
     * Data download to local file.
     */
    const MusicObject::MusicSongInfomations& getMusicSongInfos() const;
    /*!
     * Return the current song container.
     */
    void downloadDataFrom(bool play);
    /*!
     * Download data from net and just play or not.
     */

public Q_SLOTS:
    virtual void listCellEntered(int row, int column) override;
    /*!
     * Table widget list cell enter.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Subclass should implement this function.
     */
    virtual void clearAllItems() override;
    /*!
     * Clear All Items.
     */
    virtual void createSearchedItems(const QString &songname, const QString &artistname,
                                     const QString &time) override;
    /*!
     * Create searched items.
     */

protected:
    void addSearchMusicToPlayList(int row, bool play);
    /*!
     * Add search music to play list by index.
     */
    bool downloadDataFrom(const MusicObject::MusicSongInfomation &downloadInfo, bool play);
    /*!
     * Download data from net and just play or not.
     */

};

#endif // MUSICQUERYFOUNDTABLEWIDGET_H
