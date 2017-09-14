#ifndef MUSICMESSAGEABOUTDIALOG_H
#define MUSICMESSAGEABOUTDIALOG_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

class MusicDownloadCounterPVThread;

namespace Ui {
    class MusicMessageAboutDialog;
}
/*! @brief The class of the about application info dialog.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicMessageAboutDialog : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicMessageAboutDialog(QWidget *parent = 0);

    virtual ~MusicMessageAboutDialog();

    /*!
     * Get class object name.
     */
    static QString getClassName();

public Q_SLOTS:
    /*!
     * Get counter pv finished.
     */
    void musicGetCounterFinished(const QString &data);
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    Ui::MusicMessageAboutDialog *m_ui;
    MusicDownloadCounterPVThread *m_counterPVThread;

};

#endif // MUSICMESSAGEABOUTDIALOG_H
