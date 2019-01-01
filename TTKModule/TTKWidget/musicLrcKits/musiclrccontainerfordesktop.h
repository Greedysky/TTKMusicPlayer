#ifndef MUSICLRCCONTAINERFORDESKTOP_H
#define MUSICLRCCONTAINERFORDESKTOP_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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
 ================================================= */

#include "musiclrccontainer.h"
#include "musicwidgetheaders.h"

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
    TTK_DECLARE_MODULE(MusicLrcContainerForDesktop)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcContainerForDesktop(QWidget *parent = nullptr);

    virtual ~MusicLrcContainerForDesktop();

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
    TTK_DECLARE_MODULE(MusicLrcContainerHorizontalDesktop)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcContainerHorizontalDesktop(QWidget *parent = nullptr);

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
    TTK_DECLARE_MODULE(MusicLrcContainerVerticalDesktop)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcContainerVerticalDesktop(QWidget *parent = nullptr);

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
