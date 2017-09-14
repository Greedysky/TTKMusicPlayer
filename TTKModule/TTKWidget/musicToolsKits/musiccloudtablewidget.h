#ifndef MUSICCLOUDTABLEWIDGET_H
#define MUSICCLOUDTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicsongslistabstracttablewidget.h"
#include "musicdownloadrecordconfigmanager.h"

class MusicProgressBarDelegate;

/*! @brief The class of the cloud shared song upload table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicCloudUploadTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicCloudUploadTableWidget(QWidget *parent = 0);

    virtual ~MusicCloudUploadTableWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

public Q_SLOTS:
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;

protected:
    MusicProgressBarDelegate *m_progressBarDelegate;

};


/*! @brief The class of the cloud shared song download table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicCloudDownloadTableWidget : public MusicSongsListAbstractTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicCloudDownloadTableWidget(QWidget *parent = 0);

    virtual ~MusicCloudDownloadTableWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

public Q_SLOTS:
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Delete item from list at current row.
     */
    virtual void setDeleteItemAt() override;
    /*!
     * Update download percent\ total time and current time progress.
     */
    void downloadProgressChanged(float percent, const QString &total, qint64 time);
    /*!
     * Create download item from download name and total time.
     */
    void createDownloadItem(const QString &name, qint64 time);

protected:
    /*!
     * Read all config from file and insert items.
     */
    void musicSongsFileName();
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Create item by index and name and size and time.
     */
    void createItem(int index, const MusicDownloadRecord &record, qint64 time);

    MusicProgressBarDelegate *m_progressBarDelegate;
    MusicDownloadRecords m_musicRecords;

};

#endif // MUSICCLOUDTABLEWIDGET_H
