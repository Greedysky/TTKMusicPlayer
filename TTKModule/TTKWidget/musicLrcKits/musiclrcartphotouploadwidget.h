#ifndef MUSICLRCARTPHOTOUPLOADWIDGET_H
#define MUSICLRCARTPHOTOUPLOADWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
    /*!
     * Object contsructor.
     */
    explicit MusicLrcArtPhotoUploadWidget(QWidget *parent = 0);

    virtual ~MusicLrcArtPhotoUploadWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

public Q_SLOTS:
    /*!
     * Delta value changed.
     */
    void deltaValueChanged(float v);
    /*!
     * Select button clicked.
     */
    void selectButtonClicked();
    /*!
     * Upload button clicked.
     */
    void uploadButtonClicked();
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    Ui::MusicLrcArtPhotoUploadWidget *m_ui;

};

#endif // MUSICLRCARTPHOTOUPLOADWIDGET_H
