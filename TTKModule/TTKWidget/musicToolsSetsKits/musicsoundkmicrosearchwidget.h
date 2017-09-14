#ifndef MUSICSOUNDKMICROSEARCHWIDGET_H
#define MUSICSOUNDKMICROSEARCHWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicquerytablewidget.h"
#include "musicabstractmovewidget.h"

/*! @brief The class of the sound kmicro search table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSoundKMicroSearchTableWidget : public MusicQueryItemTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSoundKMicroSearchTableWidget(QWidget *parent = 0);

    virtual ~MusicSoundKMicroSearchTableWidget();

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
     * Set query MV flag.
     */
    void setQueryMVFlag(bool flag);

Q_SIGNALS:
    /*!
     * Set current media url.
     */
    void mvURLChanged(bool mv, const QString &url, const QString &lrcUrl);

public Q_SLOTS:
    /*!
     * Clear All Items.
     */
    virtual void clearAllItems() override;
    /*!
     * Create searched items.
     */
    virtual void createSearchedItems(const MusicSearchedItem &songItem) override;
    /*!
     * Item has double clicked.
     */
    virtual void itemDoubleClicked(int row, int column) override;
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;

protected:
    bool m_queryMv;

};


class MusicLocalSongSearchEdit;

/*! @brief The class of the sound kmicro search widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSoundKMicroSearchWidget : public MusicAbstractMoveSingleWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSoundKMicroSearchWidget(QWidget *parent = 0);

    ~MusicSoundKMicroSearchWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Connect to parent.
     */
    void connectTo(QObject *obj);
    /*!
     * Start seach KMicro.
     */
    void startSeachKMicro(const QString &name);

public Q_SLOTS:
    /*!
     * Start to search.
     */
    void startToSearch();
    /*!
     * Set query MV flag.
     */
    void setQueryMVFlag(int flag);

protected:
    bool m_queryMv;
    MusicLocalSongSearchEdit *m_searchEdit;
    MusicSoundKMicroSearchTableWidget *m_searchTableWidget;

};

#endif // MUSICSOUNDKMICROSEARCHWIDGET_H
