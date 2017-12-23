#ifndef MUSICITEMDELEGATE_H
#define MUSICITEMDELEGATE_H

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

#include <QItemDelegate>
#include "musicglobaldefine.h"

#define MUSIC_CHECK_ROLE Qt::UserRole + 1
#define MUSIC_QUERY_ROLE Qt::UserRole + 2
#define MUSIC_PROCS_ROLE Qt::UserRole + 3
#define MUSIC_AUDIT_ROLE Qt::UserRole + 4
#define MUSIC_TEXTS_ROLE Qt::UserRole + 5
#define MUSIC_TIMES_ROLE Qt::UserRole + 6
#define MUSIC_DATAS_ROLE Qt::UserRole + 100

class QLabel;
class QCheckBox;
class QRadioButton;
class QProgressBar;
class QPushButton;

/*! @brief The class of the radio button item delegate.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicRadioButtonDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicRadioButtonDelegate(QObject* parent = 0);

    virtual ~MusicRadioButtonDelegate();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set delegate item style.
     */
    void setStyleSheet(const QString &style);
    /*!
     * Override size hint.
     */
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &) const;
    /*!
     * Override paint.
     */
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

protected:
    QRadioButton *m_radioButton;

};


/*! @brief The class of the checkbox item delegate.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicCheckBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicCheckBoxDelegate(QObject* parent = 0);

    virtual ~MusicCheckBoxDelegate();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set delegate item style.
     */
    void setStyleSheet(const QString &style);
    /*!
     * Override size hint.
     */
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &) const;
    /*!
     * Override paint.
     */
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

protected:
    QCheckBox *m_checkBox;

};


/*! @brief The class of the color item delegate.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicQueryTableDelegate : public MusicCheckBoxDelegate
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicQueryTableDelegate(QObject* parent = 0);

    virtual ~MusicQueryTableDelegate();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Override paint.
     */
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

};


/*! @brief The class of the progress bar item delegate.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicProgressBarDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicProgressBarDelegate(QObject* parent = 0);

    ~MusicProgressBarDelegate();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set delegate item style.
     */
    void setStyleSheet(const QString &style);
    /*!
     * Override size hint.
     */
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &) const;
    /*!
     * Override paint.
     */
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

private:
    QProgressBar *m_progress;

};


/*! @brief The class of the label item delegate.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicLabelDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLabelDelegate(QObject* parent = 0);

    virtual ~MusicLabelDelegate();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set delegate item style.
     */
    void setStyleSheet(const QString &style);
    /*!
     * Override size hint.
     */
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &) const;
    /*!
     * Override paint.
     */
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

protected:
    QLabel *m_label;

};


/*! @brief The class of the push button item delegate.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPushButtonDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicPushButtonDelegate(QObject* parent = 0);

    virtual ~MusicPushButtonDelegate();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set delegate item style.
     */
    void setStyleSheet(const QString &style);
    /*!
     * Override size hint.
     */
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &) const;
    /*!
     * Override paint.
     */
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

protected:
    QPushButton *m_pushButton;

};

#endif // MUSICITEMDELEGATE_H
