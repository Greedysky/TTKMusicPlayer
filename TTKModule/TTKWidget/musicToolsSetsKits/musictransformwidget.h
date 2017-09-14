#ifndef MUSICTRANSFROMWIDGET_H
#define MUSICTRANSFROMWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicuiobject.h"
#include "musicabstractmovedialog.h"

#define LINE_WIDTH 380

class QProcess;

namespace Ui {
class MusicTransformWidget;
}

/*! @brief The class of the transform widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicTransformWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    enum TransformType
    {
        Music,  /*!< Music category*/
        Lrc,    /*!< Lrc category*/
    };
    /*!
     * Object contsructor.
     */
    explicit MusicTransformWidget(QWidget *parent = 0);

    virtual ~MusicTransformWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

public Q_SLOTS:
    /*!
     * Selected input path.
     */
    void initInputPath();
    /*!
     * Selected out path.
     */
    void initOutputPath();
    /*!
     * Start to transform.
     */
    void startTransform();
    /*!
     * Transform finished.
     */
    void transformFinish();
    /*!
     * Input is dir not file.
     */
    void folderBoxChecked();
    /*!
     * Transform krc file to lrc file.
     */
    void krc2lrcBoxChecked(bool check);
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    /*!
     * Get transform song name.
     */
    QString getTransformSongName() const;
    /*!
     * If input is dir just get all files in this dir.
     */
    QFileInfoList getFileList(const QString &path);
    /*!
     * Init control parameter.
     */
    void initControlParameter() const;
    /*!
     * Start a process to transform.
     */
    bool processTransform(const QString &para) const;
    /*!
     * Set music control enable or false when trans lrc.
     */
    void setMusicCheckedControl(bool enable);
    /*!
     * Set control enable false when it begin.
     */
    void setCheckedControl(bool enable);

    Ui::MusicTransformWidget *m_ui;
    QProcess *m_process;
    QStringList m_path;
    TransformType m_currentType;

};

#endif // MUSICTRANSFROMWIDGET_H
