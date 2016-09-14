#ifndef MUSICLRCCONTAINERFORDESKTOP_H
#define MUSICLRCCONTAINERFORDESKTOP_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QBoxLayout>
#include <QToolButton>
#include <QPushButton>
#include "musiclrccontainer.h"

#define TOOLBAR_MAIN_HEIGHT 35
#define TOOLBAR_HEIGHT      23
#define TOOLBAR_WIDTH       31
#define TOOLBAR_TEXT_LENGTH 58


class MusicLRCManagerForDesktop;

/*! @brief The class of the desktop lrc container.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcContainerForDesktop : public MusicLrcContainer
{
    Q_OBJECT
public:
    explicit MusicLrcContainerForDesktop(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicLrcContainerForDesktop();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void startTimerClock() override;
    /*!
     * Start timer clock to draw lrc.
     */
    virtual void stopLrcMask() override;
    /*!
     * Stop timer clock to draw lrc.
     */
    virtual void setMaskLinearGradientColor(const QList<QColor> &colors) const override;
    /*!
     * Set mask linear gradient color.
     */
    virtual void setSettingParameter() override;
    /*!
     * Set setting parameter.
     */

    virtual void initCurrentLrc() const;
    /*!
     * Init current lrc when the first show.
     */
    void showPlayStatus(bool status) const;
    /*!
     * Set current play state button.
     */
    bool getPlayStatus() const;
    /*!
     * Get current play state button.
     */
    void updateCurrentLrc(const QString &first, const QString &second, qint64 time);
    /*!
     * Update current lrc by first and second and time.
     */
    bool getWindowType() const { return m_verticalWindow;}
    /*!
     * Get current desktop lrc window type.
     */

Q_SIGNALS:
    void desktopLrcClosed();
    /*!
     * Desktop lrc close state changed emit.
     */
    void setWindowLockedChanged(bool lock);
    /*!
     * Lock current desktop lrc state changed emit.
     */
    void setWindowLrcTypeChanged();
    /*!
     * Set current desktop lrc window type changed.
     */

public Q_SLOTS:
    void setWindowLockedChanged();
    /*!
     * Lock current desktop lrc state changed.
     */
    void setLrcBiggerChanged();
    /*!
     * Set lrc bigger changed.
     */
    void setLrcSmallerChanged();
    /*!
     * Set lrc smaller changed.
     */
    void toolStyleChanged();
    /*!
     * Set current color style changed.
     */

protected:
    void setSelfGeometry() const;
    /*!
     * Set self geometry.
     */
    void creatToolBarWidget();
    /*!
     * Creat toolBar widget.
     */
    virtual void resizeLrcSizeArea() = 0;
    /*!
     * Resize lrc size area by change size.
     * Subclass should implement this function.
     */
    void resizeLrcSizeArea(bool resize);
    /*!
     * Resize lrc size area to bigger or smaller.
     */
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void closeEvent(QCloseEvent *event) override;
    /*!
     * Override the widget event.
     */

    bool m_verticalWindow;
    bool m_windowLocked, m_reverse;
    int m_currentLrcFontSize;
    QPoint m_offset, m_geometry;

    QBoxLayout *m_toolBarLayout;
    QWidget *m_toolBarWidget;
    QToolButton *m_toolPlayButton;

};


/*! @brief The class of the desktop horizontal lrc container.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcContainerHorizontalDesktop : public MusicLrcContainerForDesktop
{
    Q_OBJECT
public:
    explicit MusicLrcContainerHorizontalDesktop(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void initCurrentLrc() const override;
    /*!
     * Init current lrc when the first show.
     */

protected:
    virtual void resizeLrcSizeArea() override;
    /*!
     * Resize lrc size area by change size.
     */
};


/*! @brief The class of the desktop vertical lrc container.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcContainerVerticalDesktop : public MusicLrcContainerForDesktop
{
    Q_OBJECT
public:
    explicit MusicLrcContainerVerticalDesktop(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void initCurrentLrc() const override;
    /*!
     * Init current lrc when the first show.
     */

protected:
    virtual void resizeLrcSizeArea() override;
    /*!
     * Resize lrc size area by change size.
     */
};

#endif // MUSICLRCCONTAINERFORDESKTOP_H
