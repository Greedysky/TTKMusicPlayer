#ifndef MUSICCLOUDFILEMANAGERDIALOG_H
#define MUSICCLOUDFILEMANAGERDIALOG_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"
#include "musicabstracttablewidget.h"

/*! @brief The class of the local upload record item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_TOOL_EXPORT MusicUploadData
{
    enum State
    {
        Waited = 0,     /*!< 0 waited*/
        Successed,      /*!< 1 successed*/
        Errored         /*!< 2 error*/
    };

    QString m_path;
    QString m_name;
    State m_state;

    inline bool operator<(const MusicUploadData &other) const
    {
        return m_name < other.m_name;
    }

    inline bool operator==(const MusicUploadData &other) const
    {
        return !(*this < other || other < *this);
    }
}MusicUploadData;
TTK_DECLARE_LISTS(MusicUploadData)

namespace Ui {
class MusicCloudFileManagerDialog;
}


/*! @brief The class of the cloud file manager dialog.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicCloudFileManagerDialog : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicCloudFileManagerDialog(QWidget *parent = 0);

    virtual ~MusicCloudFileManagerDialog();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set reupload state.
     */
    void setReuploadState(bool state);
    /*!
     * Creat file manager item.
     */
    void creatFileManager(const MusicUploadData &data);
    /*!
     * Creat file manager items.
     */
    void creatFilesManager(const MusicUploadDatas &datas);
    /*!
     * Update item progress.
     */
    void updateItemProgress(int percent, const MusicUploadData &data);

public Q_SLOTS:
    /*!
     * Download button state changed.
     */
    void downloadStateChanged();
    /*!
     * Override exec function.
     */
    virtual int exec();
    /*!
     * Override exec function.
     */
    void show();

protected:
    /*!
     * Get icon by data state.
     */
    QIcon getIconByDataState(MusicUploadData::State state);

    Ui::MusicCloudFileManagerDialog *m_ui;

};

#endif // MUSICCLOUDFILEMANAGERDIALOG_H
