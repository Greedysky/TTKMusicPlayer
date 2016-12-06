#ifndef MUSICCLOUDFILEMANAGERDIALOG_H
#define MUSICCLOUDFILEMANAGERDIALOG_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"
#include "musicabstracttablewidget.h"

typedef struct MUSIC_TOOL_EXPORT UploadData
{
    enum State{
        Waited = 0,     ///0 waited
        Successed,      ///1 successed
        Errored         ///2 error
    };

    QString m_path;
    QString m_name;
    State m_state;

    inline bool operator<(const UploadData &other) const
    {
        return m_name < other.m_name;
    }

    inline bool operator==(const UploadData &other) const
    {
        return !(*this < other || other < *this);
    }
}UploadData;
TTK_DECLARE_LISTS(UploadData)

namespace Ui {
class MusicCloudFileManagerDialog;
}

class MusicProgressBarDelegate;

/*! @brief The class of the cloud shared song table widget.
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


/*! @brief The class of the cloud file manager dialog.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicCloudFileManagerDialog : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicCloudFileManagerDialog(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicCloudFileManagerDialog();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setReuploadState(bool state);
    /*!
     * Set reupload state.
     */
    void creatFileManager(const UploadData &data);
    /*!
     * Creat file manager item.
     */
    void creatFilesManager(const UploadDatas &datas);
    /*!
     * Creat file manager items.
     */
    void updateItemProgress(int percent, const UploadData &data);
    /*!
     * Update item progress.
     */

public Q_SLOTS:
    virtual int exec();
    /*!
     * Override exec function.
     */
    void show();
    /*!
     * Override exec function.
     */

protected:
    QIcon getIconByDataState(UploadData::State state);
    /*!
     * Get icon by data state.
     */

    Ui::MusicCloudFileManagerDialog *m_ui;

};

#endif // MUSICCLOUDFILEMANAGERDIALOG_H
