#ifndef MUSICLOCALSONGSTABLEWIDGET_H
#define MUSICLOCALSONGSTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicsongslistabstracttablewidget.h"

/*! @brief The class of the lcal songs table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicLocalSongsTableWidget : public MusicSongsListAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicLocalSongsTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicLocalSongsTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void clear();
    /*!
     * Clear current items.
     */
    void addItems(const QFileInfoList &path);
    /*!
     * Add show list items.
     */
    inline void setFiles(const QFileInfoList &path) { m_fileNames = path; }
    /*!
     * Set files container.
     */
    inline const QFileInfoList &getFiles() const { return m_fileNames; }
    /*!
     * Get files container.
     */

public Q_SLOTS:
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Table widget list cell click.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */

protected:
    QFileInfoList m_fileNames;

};

typedef QMap<QString , QFileInfoList> MusicInfoData;
typedef QMapIterator<QString , QFileInfoList> MusicInfoDataIterator;

/*! @brief The class of the lcal songs info table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicLocalSongsInfoTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicLocalSongsInfoTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void clear();
    /*!
     * Clear current items.
     */
    void addItems(const MusicInfoData &data);
    /*!
     * Add show list item.
     */

Q_SIGNALS:
    void updateFileLists(const QFileInfoList &list);
    /*!
     * Update file lists.
     */

public Q_SLOTS:
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Table widget list cell click.
     */
    void listCellDoubleClicked(int row, int column);
    /*!
     * Table widget list double cell click.
     */
};

#endif // MUSICLOCALSONGSTABLEWIDGET_H
