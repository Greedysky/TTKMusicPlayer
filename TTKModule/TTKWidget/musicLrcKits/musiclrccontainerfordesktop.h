#ifndef MUSICLRCCONTAINERFORDESKTOP_H
#define MUSICLRCCONTAINERFORDESKTOP_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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
class TTK_MODULE_EXPORT MusicLrcContainerForDesktop : public MusicLrcContainer
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcContainerForDesktop)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcContainerForDesktop(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicLrcContainerForDesktop();

    /*!
     * Start timer clock to draw lrc.
     */
    virtual void startDrawLrc() override final;
    /*!
     * Stop timer clock to draw lrc.
     */
    virtual void stopDrawLrc() override final;
    /*!
     * Set setting parameter.
     */
    virtual void applyParameter() override final;

    /*!
     * Init current lrc when the first show.
     */
    virtual void initCurrentLrc() const;

    /*!
     * Set current play state button.
     */
    void setCurrentPlayStatus(bool status) const;
    /*!
     * Update current lrc by first and second and time.
     */
    void updateCurrentLrc(const QString &first, const QString &second, qint64 time);
    /*!
     * Get current desktop lrc window type is vertical or not.
     */
    inline bool isVerticalWindowType() const { return m_verticalWindow; }
    /*!
     * Make object status copy.
     */
    void makeStatusCopy(MusicLrcContainerForDesktop *other);

Q_SIGNALS:
    /*!
     * Lock current desktop lrc state changed.
     */
    void setWindowLockedChanged(bool lock);

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
     * Set self position.
     */
    void setSelfPosition() const;
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
    virtual void mousePressEvent(QMouseEvent *event) override final;
    virtual void mouseMoveEvent(QMouseEvent *event) override final;
    virtual void contextMenuEvent(QContextMenuEvent *event) override final;
    virtual void enterEvent(QtEnterEvent *event) override final;
    virtual void leaveEvent(QEvent *event) override final;
    virtual void closeEvent(QCloseEvent *event) override final;

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
class TTK_MODULE_EXPORT MusicLrcContainerHorizontalDesktop : public MusicLrcContainerForDesktop
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcContainerHorizontalDesktop)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcContainerHorizontalDesktop(QWidget *parent = nullptr);

    /*!
     * Init current lrc when the first show.
     */
    virtual void initCurrentLrc() const override final;

public Q_SLOTS:
    /*!
     * Set single line type changed.
     */
    virtual void setSingleLineTypeChanged() override final;

private:
    /*!
     * Resize lrc size area by change size.
     */
    virtual void resizeLrcSizeArea() override final;

};


/*! @brief The class of the desktop vertical lrc container.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcContainerVerticalDesktop : public MusicLrcContainerForDesktop
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcContainerVerticalDesktop)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcContainerVerticalDesktop(QWidget *parent = nullptr);

    /*!
     * Init current lrc when the first show.
     */
    virtual void initCurrentLrc() const override final;

public Q_SLOTS:
    /*!
     * Set single line type changed.
     */
    virtual void setSingleLineTypeChanged() override final;

private:
    /*!
     * Resize lrc size area by change size.
     */
    virtual void resizeLrcSizeArea() override final;

};

#endif // MUSICLRCCONTAINERFORDESKTOP_H
