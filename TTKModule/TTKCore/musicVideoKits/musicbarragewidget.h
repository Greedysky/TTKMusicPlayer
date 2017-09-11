#ifndef MUSICBARRAGEWIDGET_H
#define MUSICBARRAGEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicglobaldefine.h"
#include "musicbarragerecordconfigmanager.h"

#include <QLabel>
#include <QLineEdit>
#include <QPropertyAnimation>

/*! @brief The class of the barrage animation.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SUPER_EXPORT MusicBarrageAnimation : public QPropertyAnimation
{
    Q_OBJECT
public:
    explicit MusicBarrageAnimation(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    MusicBarrageAnimation(QObject *target, const QByteArray &propertyName, QObject *parent = 0);
    /*!
     * Object contsructor by tagrget and propertyName.
     */
    ~MusicBarrageAnimation() = default;

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setSize(const QSize &size);
    /*!
     * Set animation region size.
     */

public Q_SLOTS:
    void animationFinished();
    /*!
     * Current animation is finished.
     */

protected:
    void init();
    /*!
     * Init parameters.
     */

    QSize m_parentSize;

};


/*! @brief The class of the barrage widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SUPER_EXPORT MusicBarrageWidget : public QObject
{
    Q_OBJECT
public:
    explicit MusicBarrageWidget(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicBarrageWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void start();
    /*!
     * Start barrage animation.
     */
    void pause();
    /*!
     * Pause barrage animation.
     */
    void stop();
    /*!
     * Stop barrage animation.
     */

    void setSize(const QSize &size);
    /*!
     * Set animation region size.
     */
    void barrageStateChanged(bool on);
    /*!
     * Set current barrage animation on or not.
     */
    void addBarrage(const MusicBarrageRecord &record);
    /*!
     * Add barrage record.
     */

protected:
    void deleteItems();
    /*!
     * Clear all items.
     */
    void createLabel();
    /*!
     * Create barrage item label.
     */
    QLabel *createLabel(const MusicBarrageRecord &record);
    /*!
     * Create barrage item label by object.
     */
    void createAnimation();
    /*!
     * Create barrage animation.
     */
    void createAnimation(QLabel *label);
    /*!
     * Create barrage animation by object.
     */
    void readBarrage();
    /*!
     * Read barrage text from local file.
     */
    void writeBarrage();
    /*!
     * Write barrage text to local file.
     */

    bool m_barrageState;
    QSize m_parentSize;
    QWidget *m_parentClass;
    QList<QLabel*> m_labels;
    QList<MusicBarrageAnimation*> m_animations;
    MusicBarrageRecords m_barrageRecords;

};

#endif // MUSICBARRAGEWIDGET_H
