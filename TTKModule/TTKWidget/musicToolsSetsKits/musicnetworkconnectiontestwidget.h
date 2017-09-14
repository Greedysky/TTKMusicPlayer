#ifndef MUSICNETWORKCONNECTIONTESTWIDGET_H
#define MUSICNETWORKCONNECTIONTESTWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicabstractmovewidget.h"

class MusicNetworkTestThread;

/*! @brief The class of the network connection item Widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicNetworkConnectionItem : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicNetworkConnectionItem(QWidget *parent = 0);

    ~MusicNetworkConnectionItem();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set label text name.
     */
    void setText(const QString &text);
    /*!
     * Set current test url.
     */
    void setUrl(const QString &url);

    /*!
     * Start to test network.
     */
    void start();
    /*!
     * Stop to test network.
     */
    void stop();

Q_SIGNALS:
    /*!
     * Network connection test changed.
     */
    void networkConnectionTestChanged();

private Q_SLOTS:
    /*!
     * Test network finished.
     */
    void testFinshed(bool state);

protected:
    MusicNetworkTestThread *m_testThread;
    QLabel *m_iconLabel, *m_nameText, *m_stateText;

};


namespace Ui {
class MusicNetworkConnectionTestWidget;
}

/*! @brief The class of the network connection test widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicNetworkConnectionTestWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicNetworkConnectionTestWidget(QWidget *parent = 0);

    virtual ~MusicNetworkConnectionTestWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();


Q_SIGNALS:
    /*!
     * Reset window open flag.
     */
    void resetFlag(MusicObject::ToolsType flag);

public Q_SLOTS:
    /*!
     * Override show function.
     */
    void show();
    /*!
     * Button state changed.
     */
    void buttonStateChanged();
    /*!
     * Test network finished.
     */
    void testFinshed();

protected:
    /*!
     * Override the widget event.
     */
    virtual void closeEvent(QCloseEvent *event) override;

    int m_countIndex;
    Ui::MusicNetworkConnectionTestWidget *m_ui;
    QList<MusicNetworkConnectionItem*> m_connectionItems;

};

#endif // MUSICNETWORKCONNECTIONTESTWIDGET_H
