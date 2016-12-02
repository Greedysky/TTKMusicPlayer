#ifndef MUSICLRCARTPHOTOUPLOADWIDGET_H
#define MUSICLRCARTPHOTOUPLOADWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicLrcArtPhotoUploadWidget;
}

/*! @brief The class of the lrc art photo upload.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcArtPhotoUploadWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicLrcArtPhotoUploadWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicLrcArtPhotoUploadWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

public Q_SLOTS:
    void selectButtonClicked();
    /*!
     * Select button clicked.
     */
    void uploadButtonClicked();
    /*!
     * Upload button clicked.
     */
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    Ui::MusicLrcArtPhotoUploadWidget *ui;

};

#endif // MUSICLRCARTPHOTOUPLOADWIDGET_H
