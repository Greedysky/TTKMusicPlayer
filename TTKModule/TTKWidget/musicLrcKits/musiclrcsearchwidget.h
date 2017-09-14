#ifndef MUSICLRCSEARCHWIDGET_H
#define MUSICLRCSEARCHWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicLrcSearchWidget;
}

/*! @brief The class of the lrc search widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcSearchWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcSearchWidget(QWidget *parent = 0);

    virtual ~MusicLrcSearchWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

public Q_SLOTS:
    /*!
     * Set lrc search finished.
     */
    void lrcSearchFinished() const;
    /*!
     * Lrc search button clicked.
     */
    void lrcSearchButtonClicked() const;
    /*!
     * Lrc search download clicked.
     */
    void lrcSearchDownloadClicked();
    /*!
     * Lrc download state is finished.
     */
    void lrcDownloadStateChanged(const QString &string);
    /*!
     * Set current song name.
     */
    void setCurrentSongName(const QString &name) const;
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    Ui::MusicLrcSearchWidget *m_ui;

};

#endif // MUSICLRCSEARCHWIDGET_H
