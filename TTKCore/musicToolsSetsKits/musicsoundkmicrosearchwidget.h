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

/*! @brief The class of the sound kmicro search table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSoundKMicroSearchTableWidget : public MusicQueryItemTableWidget
{
    Q_OBJECT
public:
    explicit MusicSoundKMicroSearchTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSoundKMicroSearchTableWidget();

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
    void setQueryMVFlag(bool flag);
    /*!
     * Set query MV flag.
     */

Q_SIGNALS:
    void mvURLChanged(bool mv, const QString &url, const QString &lrcUrl);
    /*!
     * Set current media url.
     */

public Q_SLOTS:
    virtual void clearAllItems() override;
    /*!
     * Clear All Items.
     */
    virtual void createSearchedItems(const MusicSearchedItem &songItem) override;
    /*!
     * Create searched items.
     */
    virtual void itemDoubleClicked(int row, int column) override;
    /*!
     * Item has double clicked.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Table widget list cell click.
     */

protected:
    bool m_queryMv;

};


class MusicLocalSongSearchEdit;

/*! @brief The class of the sound kmicro search widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSoundKMicroSearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSoundKMicroSearchWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicSoundKMicroSearchWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void connectTo(QObject *obj);
    /*!
     * Connect to parent.
     */
    void startSeachKMicro(const QString &name);
    /*!
     * Start seach KMicro.
     */

public Q_SLOTS:
    void startToSearch();
    /*!
     * Start to search.
     */
    void setQueryMVFlag(int flag);
    /*!
     * Set query MV flag.
     */

protected:
    bool m_queryMv;
    MusicLocalSongSearchEdit *m_searchEdit;
    MusicSoundKMicroSearchTableWidget *m_searchTableWidget;

};

#endif // MUSICSOUNDKMICROSEARCHWIDGET_H
