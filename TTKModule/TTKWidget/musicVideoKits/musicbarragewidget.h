#ifndef MUSICBARRAGEWIDGET_H
#define MUSICBARRAGEWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#include "musicwidgetheaders.h"
#include "musicbarragerecord.h"

#include <QPropertyAnimation>

/*! @brief The class of the barrage animation.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicBarrageAnimation : public QPropertyAnimation
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicBarrageAnimation(QObject *parent = nullptr);
    MusicBarrageAnimation(QObject *target, const QByteArray &name, QObject *parent = nullptr);

    /*!
     * Set animation region size.
     */
    void setSize(const QSize &size);

private Q_SLOTS:
    /*!
     * Current animation is finished.
     */
    void animationFinished();

private:
    /*!
     * Init widget and parameter.
     */
    void initialize();

    QSize m_parentSize;

};


class MusicBarrageRequest;

/*! @brief The class of the barrage widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicBarrageWidget : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicBarrageWidget(QObject *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicBarrageWidget();

    /*!
     * Start barrage animation.
     */
    void start();
    /*!
     * Pause barrage animation.
     */
    void pause();
    /*!
     * Stop barrage animation.
     */
    void stop();

    /*!
     * Set animation region size.
     */
    void setSize(const QSize &size);
    /*!
     * Set barrage play data.
     */
    void setBarrage(const QString &name, const QString &id);

public Q_SLOTS:
    /*!
     * Set current barrage animation on or not.
     */
    void barrageStateChanged(bool on);
    /*!
     * Add barrage record.
     */
    void addBarrage(const MusicBarrageRecord &record);

private Q_SLOTS:
    /*!
     * Region size changed.
     */
    void sizeChanged();
    /*!
     * Download data from net finished.
     */
    void downLoadFinished(const QByteArray &bytes);

private:
    /*!
     * Clear all items.
     */
    void clearItems();
    /*!
     * Create barrage item label.
     */
    void createLabel();
    /*!
     * Clear all barrage.
     */
    void clearBarrages();
    /*!
     * Create barrage item label by object.
     */
    QLabel *createLabel(const MusicBarrageRecord &record);
    /*!
     * Create barrage animation.
     */
    void createAnimation();
    /*!
     * Create barrage animation by object.
     */
    void createAnimation(QLabel *label);

    bool m_state;
    QSize m_parentSize;
    QWidget *m_parent;
    QTimer *m_sizeTimer;
    QString m_lastQueryID;
    QList<QLabel*> m_labels;
    QList<MusicBarrageAnimation*> m_animations;
    MusicBarrageRecordList m_barrageRecords;
    MusicBarrageRequest *m_networkRequest;

};

#endif // MUSICBARRAGEWIDGET_H
