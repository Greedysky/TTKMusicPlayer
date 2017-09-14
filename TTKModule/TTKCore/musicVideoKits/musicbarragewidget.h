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
    /*!
     * Object contsructor.
     */
    explicit MusicBarrageAnimation(QObject *parent = 0);
    /*!
     * Object contsructor by tagrget and propertyName.
     */
    MusicBarrageAnimation(QObject *target, const QByteArray &propertyName, QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set animation region size.
     */
    void setSize(const QSize &size);

public Q_SLOTS:
    /*!
     * Current animation is finished.
     */   void animationFinished();

protected:
    /*!
     * Init parameters.
     */
    void init();

    QSize m_parentSize;

};


/*! @brief The class of the barrage widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SUPER_EXPORT MusicBarrageWidget : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicBarrageWidget(QObject *parent = 0);

    ~MusicBarrageWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

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
     * Set current barrage animation on or not.
     */
    void barrageStateChanged(bool on);
    /*!
     * Add barrage record.
     */
    void addBarrage(const MusicBarrageRecord &record);

protected:
    /*!
     * Clear all items.
     */
    void deleteItems();
    /*!
     * Create barrage item label.
     */
    void createLabel();
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
    /*!
     * Read barrage text from local file.
     */
    void readBarrage();
    /*!
     * Write barrage text to local file.
     */
    void writeBarrage();

    bool m_barrageState;
    QSize m_parentSize;
    QWidget *m_parentClass;
    QList<QLabel*> m_labels;
    QList<MusicBarrageAnimation*> m_animations;
    MusicBarrageRecords m_barrageRecords;

};

#endif // MUSICBARRAGEWIDGET_H
