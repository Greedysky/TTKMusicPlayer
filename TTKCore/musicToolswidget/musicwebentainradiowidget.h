#ifndef MUSICWEBENTAINRADIOWIDGET_H
#define MUSICWEBENTAINRADIOWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include "musicuiobject.h"
#include "musicabstractmovewidget.h"

class QListWidgetItem;
class MusicCoreMPlayer;
class MusicWebEntainRadioDatabase;

namespace Ui {
class MusicWebEntainRadioWidget;
}

/*! @brief The class of the web entain radio widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebEntainRadioWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    explicit MusicWebEntainRadioWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicWebEntainRadioWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

public Q_SLOTS:
    void radioPlay();
    /*!
     * Set radio to play.
     */
    void radioStop();
    /*!
     * Set radio to stop.
     */
    void radioVolume(int num);
    /*!
     * Set radio volume.
     */
    void timeout();
    /*!
     * Set time out to show gif effect.
     */
    void updateRadioList(const QString &category);
    /*!
     * Update radio list by given category.
     */
    void updateRecentList();
    /*!
     * Update radio recent list.
     */
    void updateFavouriteList();
    /*!
     * Update favourite list.
     */
    void itemHasDoubleClicked(QListWidgetItem *item);
    /*!
     * Radio list item has double clicked.
     */
    void radioStandardOutput();
    /*!
     * Radio standard output changed.
     */
    void radioColletButton();
    /*!
     * Current radio item has collet.
     */
    void radioDiscolletButton();
    /*!
     * Current radio item has discollet.
     */
    void show();
    /*!
     * Override show function.
     */

protected:
    virtual void closeEvent(QCloseEvent *event) override;
    /*!
     * Override the widget event.
     */
    void clearAllItems();
    /*!
     * Clear All Items.
     */

    Ui::MusicWebEntainRadioWidget *m_ui;
    int m_timerCount;
    MusicCoreMPlayer *m_radio;
    QTimer m_timer;
    QString m_radioUrl, m_currentRadioName;
    QIcon *m_collecticon, *m_discollecticon;
    MusicWebEntainRadioDatabase *m_database;

};

#endif // MUSICWEBENTAINRADIOWIDGET_H
