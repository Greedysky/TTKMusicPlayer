#ifndef MUSICITEMDELEGATE_H
#define MUSICITEMDELEGATE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QItemDelegate>
#include "musicglobaldefine.h"

#define MUSIC_CHECK_ROLE Qt::UserRole + 1
#define MUSIC_QUERY_ROLE Qt::UserRole + 2
#define MUSIC_PROCS_ROLE Qt::UserRole + 3
#define MUSIC_AUDIT_ROLE Qt::UserRole + 4

class QCheckBox;
class QProgressBar;

/*! @brief The class of the checkbox item delegate.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicCheckBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit MusicCheckBoxDelegate(QObject* parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicCheckBoxDelegate();

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &) const;
    /*!
     * Override size hint.
     */
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    /*!
     * Override paint.
     */

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
    explicit MusicQueryTableDelegate(QObject* parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicQueryTableDelegate();

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    /*!
     * Override paint.
     */

};


/*! @brief The class of the progress bar item delegate.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicProgressBarDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit MusicProgressBarDelegate(QObject* parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicProgressBarDelegate();

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &) const;
    /*!
     * Override size hint.
     */
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    /*!
     * Override paint.
     */

private:
    QProgressBar *m_progress;

};

#endif // MUSICITEMDELEGATE_H
