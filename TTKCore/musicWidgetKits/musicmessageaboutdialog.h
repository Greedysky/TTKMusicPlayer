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
    explicit MusicMessageAboutDialog(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicMessageAboutDialog();

    static QString getClassName();
    /*!
     * Get class object name.
     */

public Q_SLOTS:
    void musicGetCounterFinished(const QString &data);
    /*!
     * Get counter pv finished.
     */
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    Ui::MusicMessageAboutDialog *m_ui;
    MusicDownloadCounterPVThread *m_counterPVThread;

};

#endif // MUSICMESSAGEABOUTDIALOG_H
