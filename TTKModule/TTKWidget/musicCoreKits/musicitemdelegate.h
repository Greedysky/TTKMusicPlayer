#ifndef MUSICITEMDELEGATE_H
#define MUSICITEMDELEGATE_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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

#include <QItemDelegate>
#include "musicglobaldefine.h"

#define MUSIC_DATA_ROLE     Qt::UserRole + 101
#define MUSIC_TIME_ROLE     Qt::UserRole + 102
#define MUSIC_CHECKED_ROLE  Qt::UserRole + 103
#define MUSIC_DISPLAY_ROLE  Qt::UserRole + 104
#define MUSIC_ENABLED_ROLE  Qt::UserRole + 105
#define MUSIC_PROGRESS_ROLE Qt::UserRole + 110

class QLabel;
class QCheckBox;
class QProgressBar;
class QPushButton;

/*! @brief The class of the abstract item delegate.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAbstractDelegate : public QItemDelegate
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicAbstractDelegate)
public:
    enum Module
    {
        Null = 0x0,
        TextMode = 0x1,
        DisplayMode = 0x2,
        ElideMode = 0x4,
        TreeMode = 0x8
    };

    /*!
     * Object contsructor.
     */
    explicit MusicAbstractDelegate(QObject* parent = nullptr);
    ~MusicAbstractDelegate();

    /*!
     * Set module mode.
     */
    inline void setModuleMode(TTKUInt8 mode) { m_mode = mode; }

    /*!
     * Set delegate item style.
     */
    void setStyleSheet(const QString &style) const;

    /*!
     * Override size hint.
     */
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &) const override final;

protected:
    TTKUInt8 m_mode;
    QWidget *m_container;

};


/*! @brief The class of the checkbox item delegate.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicCheckBoxDelegate : public MusicAbstractDelegate
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicCheckBoxDelegate)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicCheckBoxDelegate(QObject* parent = nullptr);
    ~MusicCheckBoxDelegate();

    /*!
     * Override paint.
     */
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override final;

Q_SIGNALS:
    /*!
     * Button state changed.
     */
    void buttonChecked();

private:
    bool m_background;
    QCheckBox *m_checkBox;

};


/*! @brief The class of the progress bar item delegate.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicProgressBarDelegate : public MusicAbstractDelegate
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicProgressBarDelegate)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicProgressBarDelegate(QObject* parent = nullptr);
    ~MusicProgressBarDelegate();

    /*!
     * Override paint.
     */
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override final;

private:
    QProgressBar *m_progress;

};


/*! @brief The class of the label item delegate.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLabelDelegate : public MusicAbstractDelegate
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLabelDelegate)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLabelDelegate(QObject* parent = nullptr);
    ~MusicLabelDelegate();

    /*!
     * Set text alignment.
     */
    void setAlignment(Qt::Alignment alignment) const;

    /*!
     * Override paint.
     */
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override final;

private:
    QLabel *m_label;

};


/*! @brief The class of the push button item delegate.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPushButtonDelegate : public MusicAbstractDelegate
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPushButtonDelegate)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicPushButtonDelegate(QObject* parent = nullptr);
    ~MusicPushButtonDelegate();

    /*!
     * Override paint.
     */
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override final;

private:
    QPushButton *m_pushButton;

};


/*! @brief The class of the lineedit item delegate.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLineEditDelegate : public QItemDelegate
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLineEditDelegate)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLineEditDelegate(QObject *parent = nullptr);

    /*!
     * Override createEditor.
     */
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override final;

};

#endif // MUSICITEMDELEGATE_H
