#ifndef MUSICLRCCONTAINERFORDESKTOP_H
#define MUSICLRCCONTAINERFORDESKTOP_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QBoxLayout>
#include <QToolButton>
#include <QPushButton>
#include "musiclrccontainer.h"

#define TOOLBAR_MAIN_HEIGHT     35
#define TOOLBAR_HEIGHT          23
#define TOOLBAR_WIDTH           31
#define TOOLBAR_TEXT_LENGTH     58


class MusicLRCManagerForDesktop;

/*! @brief The class of the desktop lrc container.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcContainerForDesktop : public MusicLrcContainer
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcContainerForDesktop(QWidget *parent = 0);

    virtual ~MusicLrcContainerForDesktop();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Start timer clock to draw lrc.
     */
    virtual void startTimerClock() override;
    /*!
     * Stop timer clock to draw lrc.
     */
    virtual void stopLrcMask() override;
    /*!
     * Set setting parameter.
     */
    virtual void setSettingParameter() override;

    /*!
     * Init current lrc when the first show.
     */
    virtual void initCurrentLrc() const;
    /*!
     * Set current play state button.
     */
    void showPlayStatus(bool status) const;
    /*!
     * Get current play state button.
     */
    bool getPlayStatus() const;
    /*!
     * Update current lrc by first and second and time.
     */
    void updateCurrentLrc(const QString &first, const QString &second, qint64 time);
    /*!
     * Get current desktop lrc window type.
     */
    bool getWindowType() const { return m_verticalWindow;}

Q_SIGNALS:
    /*!
     * Lock current desktop lrc state changed emit.
     */
    void setWindowLockedChanged(bool lock);
    /*!
     * Set current desktop lrc window type changed.
     */
    void setWindowLrcTypeChanged();

public Q_SLOTS:
    /*!
     * Lock current desktop lrc state changed.
     */
    void setWindowLockedChanged();
    /*!
     * Set lrc bigger changed.
     */
    void setLrcBiggerChanged();
    /*!
     * Set lrc smaller changed.
     */
    void setLrcSmallerChanged();
    /*!
     * Set current color style changed.
     */
    void toolStyleChanged();
    /*!
     * Set single line type changed.
     */
    virtual void setSingleLineTypeChanged();

protected:
    /*!
     * Create color menu.
     */
    void createColorMenu(QMenu &menu);
    /*!
     * Set self geometry.
     */
    void setSelfGeometry() const;
    /*!
     * Creat toolBar widget.
     */
    void creatToolBarWidget();
    /*!
     * Resize lrc size area by change size.
     * Subclass should implement this function.
     */
    virtual void resizeLrcSizeArea() = 0;
    /*!
     * Resize lrc size area to bigger or smaller.
     */
    void resizeLrcSizeArea(bool resize);
    /*!
     * Override the widget event.
     */
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void closeEvent(QCloseEvent *event) override;

    bool m_verticalWindow, m_singleLineType;
    bool m_windowLocked, m_reverse;
    int m_currentLrcFontSize, m_widgetWidth;
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
    /*!
     * Object contsructor.
     */
    explicit MusicLrcContainerHorizontalDesktop(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Init current lrc when the first show.
     */
    virtual void initCurrentLrc() const override;

public Q_SLOTS:
    /*!
     * Set single line type changed.
     */
    virtual void setSingleLineTypeChanged() override;

protected:
    /*!
     * Resize lrc size area by change size.
     */
    virtual void resizeLrcSizeArea() override;

};


/*! @brief The class of the desktop vertical lrc container.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcContainerVerticalDesktop : public MusicLrcContainerForDesktop
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcContainerVerticalDesktop(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Init current lrc when the first show.
     */
    virtual void initCurrentLrc() const override;

public Q_SLOTS:
    /*!
     * Set single line type changed.
     */
    virtual void setSingleLineTypeChanged() override;

protected:
    /*!
     * Resize lrc size area by change size.
     */
    virtual void resizeLrcSizeArea() override;

};

#endif // MUSICLRCCONTAINERFORDESKTOP_H
