#ifndef MUSICLRCFLOATPHOTOWIDGET_H
#define MUSICLRCFLOATPHOTOWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicclickedlabel.h"
#include "musicfloatabstractwidget.h"

class QCheckBox;
class QPushButton;

/*! @brief The class of the lrc photo float pixmap item.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcFloatPhotoItem : public MusicClickedLabel
{
    Q_OBJECT
public:
    explicit MusicLrcFloatPhotoItem(int index, QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicLrcFloatPhotoItem();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setPhoto(const QString &path);
    /*!
     * Set photo path and pix.
     */
    void setBoxChecked(bool check);
    /*!
     * Set check box checked state.
     */
    bool boxChecked();
    /*!
     * Get check box checked state.
     */
    void setBoxVisible(bool v);
    /*!
     * Set check box vidible.
     */

Q_SIGNALS:
    void itemClicked(int index);
    /*!
     * Current plane item clicked.
     */
    void boxClicked(int index);
    /*!
     * Current check box clicked.
     */

public Q_SLOTS:
    void sendUserBoxClicked();
    /*!
     * User selected the check box state.
     */
    void sendUserSelectArt();
    /*!
     * User selected the plane pic.
     */
    void exportArtPixmap();
    /*!
     * Export art pixmap.
     */

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */

    int m_index;
    QString m_pixPath;
    QCheckBox *m_checkBox;

};


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
    void sendUserSelectArtBg(int index);
    /*!
     * User selected the plane pic.
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
    QList<MusicLrcFloatPhotoItem*> m_planes;

};


#endif // MUSICLRCFLOATPHOTOWIDGET_H
