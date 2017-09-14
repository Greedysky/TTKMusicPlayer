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
    /*!
     * Object contsructor.
     */
    explicit MusicLocalSongsTableWidget(QWidget *parent = 0);

    virtual ~MusicLocalSongsTableWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Clear current items.
     */
    void clear();
    /*!
     * Add show list items.
     */
    void addItems(const QFileInfoList &path);
    /*!
     * Set files container.
     */
    inline void setFiles(const QFileInfoList &path) { m_fileNames = path; }
    /*!
     * Get files container.
     */
    inline const QFileInfoList &getFiles() const { return m_fileNames; }

public Q_SLOTS:
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

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
    /*!
     * Object contsructor.
     */
    explicit MusicLocalSongsInfoTableWidget(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Clear current items.
     */
    void clear();
    /*!
     * Add show list item.
     */
    void addItems(const MusicInfoData &data);

Q_SIGNALS:
    /*!
     * Update file lists.
     */
    void updateFileLists(const QFileInfoList &list);

public Q_SLOTS:
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Table widget list double cell click.
     */
    void listCellDoubleClicked(int row, int column);

};

#endif // MUSICLOCALSONGSTABLEWIDGET_H
