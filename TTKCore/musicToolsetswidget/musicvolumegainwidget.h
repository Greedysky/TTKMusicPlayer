#ifndef MUSICVOLUMEGAINWIDGET_H
#define MUSICVOLUMEGAINWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicVolumeGainWidget;
}

class QProcess;
/*! @brief The class of the volume gain widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicVolumeGainWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicVolumeGainWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicVolumeGainWidget();

Q_SIGNALS:
    void createItemFinished();

public Q_SLOTS:
    void addFileButtonClicked();
    void addFilesButtonClicked();
    void rmFileButtonClicked();
    void rmFilesButtonClicked();
    void readyReadStandardOutput();
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    void createItemFinished(const QString &track, const QString &album);

    Ui::MusicVolumeGainWidget *ui;
    QProcess *m_process;
    QStringList m_paths;
    int m_currentIndex;

};

#endif // MUSICVOLUMEGAINWIDGET_H
