#ifndef MUSICCOMMENTSWIDGET_H
#define MUSICCOMMENTSWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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
 ***************************************************************************/

#include <QLabel>
#include "musiccommentsrequest.h"

class QTextEdit;
class MusicPageQueryWidget;

/*! @brief The class of the song comment item.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicCommentsItem : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicCommentsItem)
public:
    /*!
     * Object constructor.
     */
    explicit MusicCommentsItem(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicCommentsItem();

    /*!
     * Create cell item by input data.
     */
    void addCellItem(const MusicResultDataItem &item);

private Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    void downLoadFinished(const QByteArray &bytes);

private:
    QTextEdit *m_userCommit;
    QLabel *m_userName, *m_timerLabel;
    QLabel *m_iconLabel, *m_starLabel;

};


/*! @brief The class of the song comment widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicCommentsWidget : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicCommentsWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicCommentsWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicCommentsWidget();

    /*!
     * Init widget.
     */
    virtual void initialize(bool isPain);
    /*!
     * Set current song name.
     */
    void setCurrentSongName(const QString &name);

public Q_SLOTS:
    /*!
     * Create the current song comment.
     */
    void createCommentItem(const MusicResultDataItem &item);
    /*!
     * Page widget button has changed.
     */
    void buttonClicked(int index);
    /*!
     * Create emoji label widget.
     */
    void createEMOJILabelWidget();
    /*!
     * Current EMOJI changed.
     */
    void currentEMOJIchanged(const QString &data);

protected:
    /*!
     * Override the widget event.
     */
    virtual void mousePressEvent(QMouseEvent *event) override final;
    virtual void mouseMoveEvent(QMouseEvent *event) override final;
    virtual void mouseReleaseEvent(QMouseEvent *event) override final;
    /*!
     * Int current comment label text.
     */
    void initLabel(const QString &name, int total);
    /*!
     * Delete comments items.
     */
    void deleteCommentsItems();
    /*!
     * Create page items.
     */
    void createPageWidget();

    bool m_isPain;
    QTextEdit *m_messageEdit;
    QLabel *m_topLabel, *m_commentsLabel;
    QWidget *m_messageComments;
    MusicPageQueryWidget *m_pageQueryWidget;
    QList<MusicCommentsItem*> m_commentsItems;
    MusicCommentsRequest *m_networkRequest;

};

#endif // MUSICCOMMENTSWIDGET_H
