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
    /*!
     * Object contsructor.
     */
    explicit MusicQueryFoundTableWidget(QWidget *parent = 0);

    virtual ~MusicQueryFoundTableWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Start search query by text.
     */
    virtual void startSearchQuery(const QString &text) override;
    /*!
     * Data download to local file.
     */
    virtual void musicDownloadLocal(int row) override;
    /*!
     * Return the current song container.
     */
    const MusicObject::MusicSongInformations& getMusicSongInfos() const;
    /*!
     * Download data from net and just play or not.
     */
    void downloadDataFrom(bool play);
    /*!
     * Resize window bound by widgte resize called.
     */
    void resizeWindow();

public Q_SLOTS:
    /*!
     * Table widget list cell enter.
     */
    virtual void listCellEntered(int row, int column) override;
    /*!
     * Subclass should implement this function.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Clear All Items.
     */
    virtual void clearAllItems() override;
    /*!
     * Create searched items.
     */
    virtual void createSearchedItems(const MusicSearchedItem &songItem) override;

protected:
    /*!
     * Override the widget event.
     */
    virtual void resizeEvent(QResizeEvent *event) override;
    /*!
     * Add search music to play list by index.
     */
    void addSearchMusicToPlayList(int row, bool play);
    /*!
     * Download data from net and just play or not.
     */
    bool downloadDataFrom(const MusicObject::MusicSongInformation &downloadInfo, bool play);

};

#endif // MUSICQUERYFOUNDTABLEWIDGET_H
