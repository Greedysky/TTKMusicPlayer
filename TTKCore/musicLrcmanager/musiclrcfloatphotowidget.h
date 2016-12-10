#ifndef MUSICLRCFLOATPHOTOWIDGET_H
#define MUSICLRCFLOATPHOTOWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicfloatabstractwidget.h"

#define PHOTO_WIDTH     110
#define PHOTO_HEIGHT    65
#define PHOTO_PERLINE   3

class QPushButton;
class QCheckBox;
class MusicClickedLabel;

/*! @brief The class of the lrc photo float widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcFloatPhotoWidget : public MusicFloatAbstractWidget
{
    Q_OBJECT
public:
    explicit MusicLrcFloatPhotoWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicLrcFloatPhotoWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void resizeWindow(int width, int height) override;
    /*!
     * Resize window bound by given width and height.
     */

public Q_SLOTS:
    void show();
    /*!
     * Override show function.
     */
    void close();
    /*!
     * Override close function.
     */
    void parentClose() { QLabel::close(); }
    /*!
     * Close current widget.
     */
    void confirmButtonClicked();
    /*!
     * Confirm button clicked that show selected artist pics.
     */
    void photoPrevious();
    /*!
     * To selected previous artist pic.
     */
    void photoNext();
    /*!
     * To selected next artist pic.
     */
    void artHasChanged();
    /*!
     * Update current artist pic.
     */
    void sendUserSelectArtBg1();
    /*!
     * User selected the first plane pic.
     */
    void sendUserSelectArtBg2();
    /*!
     * User selected the second plane pic.
     */
    void sendUserSelectArtBg3();
    /*!
     * User selected the third plane pic.
     */
    void userSelectCheckBoxChecked(int index);
    /*!
     * User select check box checked by index.
     */
    void selectAllStateChanged(bool state);
    /*!
     * User select check box checked by state.
     */

protected:
    virtual void enterEvent(QEvent *) override {}
    virtual void leaveEvent(QEvent *) override {}
    /*!
     * Override the widget event.
     */
    void showPhoto() const;
    /*!
     * Show all artist pics in displaying.
     */

    int m_currentIndex;
    MusicObject::MIntSet m_selectNum;
    QStringList m_artPath;
    QWidget *m_filmBGWidget;
    QCheckBox *m_checkBox;
    QPushButton *m_previous, *m_next, *m_confirmButton;
    MusicClickedLabel *m_plane1, *m_plane2, *m_plane3;
    QCheckBox *m_radio1, *m_radio2, *m_radio3;

};


#endif // MUSICLRCFLOATPHOTOWIDGET_H
