#ifndef MUSICTRANSFROMWIDGET_H
#define MUSICTRANSFROMWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QDialog>
#include <QMouseEvent>
#include <QTimer>
#include <QDir>

#include "musicobject.h"
#include "musicuiobject.h"
#include "musicabstractmovedialog.h"

class QMovie;
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
    explicit MusicTransformWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicTransformWidget();

Q_SIGNALS:

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
    void setCheckedControl(bool enable) const;
    /*!
     * Set control enable false when it begin.
     */

    Ui::MusicTransformWidget *ui;
    QProcess *m_process;
    QMovie *m_movie;
    QStringList m_path;

};

#endif // MUSICTRANSFROMWIDGET_H
