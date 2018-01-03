#ifndef MUSICLRCFLOATPHOTOWIDGET_H
#define MUSICLRCFLOATPHOTOWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

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
    /*!
     * Object contsructor.
     */
    explicit MusicLrcFloatPhotoItem(int index, QWidget *parent = 0);

    virtual ~MusicLrcFloatPhotoItem();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set photo path and pix.
     */
    void setPhoto(const QString &path);
    /*!
     * Set check box checked state.
     */
    void setBoxChecked(bool check);
    /*!
     * Get check box checked state.
     */
    bool boxChecked();
    /*!
     * Set check box vidible.
     */
    void setBoxVisible(bool v);

Q_SIGNALS:
    /*!
     * Current plane item clicked.
     */
    void itemClicked(int index);
    /*!
     * Current check box clicked.
     */
    void boxClicked(int index);

public Q_SLOTS:
    /*!
     * User selected the check box state.
     */
    void sendUserBoxClicked();
    /*!
     * User selected the plane pic.
     */
    void sendUserSelectArt();
    /*!
     * Export art pixmap.
     */
    void exportArtPixmap();

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

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
    /*!
     * Object contsructor.
     */
    explicit MusicLrcFloatPhotoWidget(QWidget *parent = 0);

    virtual ~MusicLrcFloatPhotoWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Resize window bound by given width and height.
     */
    virtual void resizeWindow(int width, int height) override;

public Q_SLOTS:
    /*!
     * Override show function.
     */
    void show();
    /*!
     * Override close function.
     */
    void close();
    /*!
     * Close current widget.
     */
    void parentClose() { QLabel::close(); }
    /*!
     * Confirm button clicked that show selected artist pics.
     */
    void confirmButtonClicked();
    /*!
     * To selected previous artist pic.
     */
    void photoPrevious();
    /*!
     * To selected next artist pic.
     */
    void photoNext();
    /*!
     * Update current artist pic.
     */
    void artHasChanged();
    /*!
     * User selected the plane pic.
     */
    void sendUserSelectArtBg(int index);
    /*!
     * User select check box checked by index.
     */
    void userSelectCheckBoxChecked(int index);
    /*!
     * User select check box checked by state.
     */
    void selectAllStateChanged(bool state);

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void enterEvent(QEvent *) override {}
    virtual void leaveEvent(QEvent *) override {}
    /*!
     * Show all artist pics in displaying.
     */
    void showPhoto() const;

    int m_currentIndex;
    MusicObject::MIntSet m_selectNum;
    QStringList m_artPath;
    QWidget *m_filmBGWidget;
    QCheckBox *m_checkBox;
    QPushButton *m_previous, *m_next, *m_confirmButton;
    QList<MusicLrcFloatPhotoItem*> m_planes;

};


#endif // MUSICLRCFLOATPHOTOWIDGET_H
