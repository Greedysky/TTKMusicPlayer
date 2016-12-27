#ifndef MUSICCLOUDTABLEWIDGET_H
#define MUSICCLOUDTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstracttablewidget.h"
#include "musicclouddownloadrecordconfigmanager.h"

class MusicProgressBarDelegate;

/*! @brief The class of the cloud shared song upload table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicCloudUploadTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicCloudUploadTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicCloudUploadTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

public Q_SLOTS:
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Table widget list cell click.
     */

protected:
    MusicProgressBarDelegate *m_progressBarDelegate;

};


/*! @brief The class of the cloud shared song download table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicCloudDownloadTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicCloudDownloadTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicCloudDownloadTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

public Q_SLOTS:
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Table widget list cell click.
     */
    void downloadProgressChanged(float percent, const QString &total, qint64 time);
    /*!
     * Update download percent\ total time and current time progress.
     */
    void createDownloadItem(const QString &name, qint64 time);
    /*!
     * Create download item from download name and total time.
     */

protected:
    void musicSongsFileName();
    /*!
     * Read all config from file and insert items.
     */
    void createItem(int index, const QString &name, const QString &size, qint64 time);
    /*!
     * Create item by index and name and size and time.
     */

    MusicProgressBarDelegate *m_progressBarDelegate;
    MusicCloudDownloadRecords m_musicRecords;

};

#endif // MUSICCLOUDTABLEWIDGET_H
