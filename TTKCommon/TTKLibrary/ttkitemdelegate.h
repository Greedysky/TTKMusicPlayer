#ifndef TTKITEMDELEGATE_H
#define TTKITEMDELEGATE_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2026 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QItemDelegate>
#include "ttkmoduleexport.h"

#define TTK_DATA_ROLE     Qt::UserRole + 101
#define TTK_TIME_ROLE     Qt::UserRole + 102
#define TTK_CHECKED_ROLE  Qt::UserRole + 103
#define TTK_DISPLAY_ROLE  Qt::UserRole + 104
#define TTK_ENABLED_ROLE  Qt::UserRole + 105
#define TTK_PROGRESS_ROLE Qt::UserRole + 110

class QLabel;
class QCheckBox;
class QPushButton;
class QProgressBar;

/*! @brief The class of the abstract item delegate.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKAbstractItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    enum Module
    {
        Null = 0x0,
        DisplayMode = 0x1,
        ElideMode = 0x2,
        TreeMode = 0x4
    };

    /*!
     * Object constructor.
     */
    explicit TTKAbstractItemDelegate(QObject* parent = nullptr);

    /*!
     * Set module mode.
     */
    inline void setModuleMode(TTKUInt8 mode) noexcept { m_mode = mode; }

    /*!
     * Set delegate item style.
     */
    void setStyleSheet(const QString &style) const;

    /*!
     * Override size hint.
     */
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &) const override final;
    /*!
     * Override paint.
     */
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

protected:
    TTKUInt8 m_mode;
    QWidget *m_container;

};


/*! @brief The class of the checkbox item delegate.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKCheckBoxItemDelegate : public TTKAbstractItemDelegate
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit TTKCheckBoxItemDelegate(QObject* parent = nullptr);
    /*!
     * Object destructor.
     */
    ~TTKCheckBoxItemDelegate();

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
class TTK_MODULE_EXPORT TTKProgressBarItemDelegate : public TTKAbstractItemDelegate
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit TTKProgressBarItemDelegate(QObject* parent = nullptr);
    /*!
     * Object destructor.
     */
    ~TTKProgressBarItemDelegate();

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
class TTK_MODULE_EXPORT TTKLabelItemDelegate : public TTKAbstractItemDelegate
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit TTKLabelItemDelegate(QObject* parent = nullptr);
    /*!
     * Object destructor.
     */
    ~TTKLabelItemDelegate();

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
class TTK_MODULE_EXPORT TTKPushButtonItemDelegate : public TTKAbstractItemDelegate
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit TTKPushButtonItemDelegate(QObject* parent = nullptr);
    /*!
     * Object destructor.
     */
    ~TTKPushButtonItemDelegate();

    /*!
     * Override paint.
     */
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override final;

private:
    QPushButton *m_pushButton;

};

#endif // TTKITEMDELEGATE_H
