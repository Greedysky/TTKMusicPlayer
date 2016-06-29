#ifndef MUSICBARRAGEWIDGET_H
#define MUSICBARRAGEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicglobaldefine.h"

#include <QPropertyAnimation>
#include <QLabel>
#include <QLineEdit>

/*! @brief The class of the barrage animation.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicBarrageAnimation : public QPropertyAnimation
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
class MUSIC_WIDGET_EXPORT MusicBarrageWidget : public QObject
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

    void barrageStateChanged(bool on);
    /*!
     * Set current barrage animation on or not.
     */
    void setSize(const QSize &size);
    /*!
     * Set animation region size.
     */

    void setLabelBackground(const QColor &color);
    /*!
     * Set barrage background.
     */
    void setLabelTextSize(int size);
    /*!
     * Set barrage text size.
     */
    void addBarrage(const QString &string);
    /*!
     * Add barrage string.
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
    void createLabel(QLabel *label);
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
    void setLabelBackground(QLabel *label);
    /*!
     * Set barrage background by object.
     */
    void setLabelTextSize(QLabel *label);
    /*!
     * Set barrage text size by object.
     */
    void readBarrage();
    /*!
     * Read barrage text from local file.
     */
    void writeBarrage();
    /*!
     * Write barrage text to local file.
     */

    int m_fontSize;
    bool m_barrageState;
    QWidget *m_parentClass;
    QSize m_parentSize;
    QColor m_backgroundColor;
    QList<QLabel*> m_labels;
    QList<MusicBarrageAnimation*> m_animations;
    QStringList m_barrageLists;

};

#endif // MUSICBARRAGEWIDGET_H
