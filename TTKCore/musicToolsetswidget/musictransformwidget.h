#ifndef MUSICTRANSFROMWIDGET_H
#define MUSICTRANSFROMWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
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
        Music,  ///*Music category*/
        Lrc,    ///*Lrc category*/
    };
    explicit MusicTransformWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicTransformWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

public Q_SLOTS:
    void initInputPath();
    /*!
     * Selected input path.
     */
    void initOutputPath();
    /*!
     * Selected out path.
     */
    void startTransform();
    /*!
     * Start to transform.
     */
    void transformFinish();
    /*!
     * Transform finished.
     */
    void folderBoxChecked();
    /*!
     * Input is dir not file.
     */
    void krc2lrcBoxChecked(bool check);
    /*!
     * Transform krc file to lrc file.
     */
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    QString getTransformSongName() const;
    /*!
     * Get transform song name.
     */
    QFileInfoList getFileList(const QString &path);
    /*!
     * If input is dir just get all files in this dir.
     */
    void initControlParameter() const;
    /*!
     * Init control parameter.
     */
    bool processTransform(const QString &para) const;
    /*!
     * Start a process to transform.
     */
    void setMusicCheckedControl(bool enable);
    /*!
     * Set music control enable or false when trans lrc.
     */
    void setCheckedControl(bool enable);
    /*!
     * Set control enable false when it begin.
     */

    Ui::MusicTransformWidget *ui;
    QProcess *m_process;
    QStringList m_path;
    TransformType m_currentType;

};

#endif // MUSICTRANSFROMWIDGET_H
