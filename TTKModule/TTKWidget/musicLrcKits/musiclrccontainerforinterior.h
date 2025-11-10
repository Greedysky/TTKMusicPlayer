#ifndef MUSICLRCCCONTAINERFORINTERIOR_H
#define MUSICLRCCCONTAINERFORINTERIOR_H

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

#include "musiclrccontainer.h"

class TTKClickedLabel;
class MusicLrcFloatWidget;
class MusicLrcFloatPlayWidget;
class MusicLrcCommentsWidget;
class MusicLrcTranslatedWidget;
class MusicVLayoutAnimationWidget;

/*! @brief The class of the interior lrc container.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcContainerForInterior : public MusicLrcContainer
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcContainerForInterior(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicLrcContainerForInterior();

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
     * Update current lrc by given time.
     */
    void updateCurrentLrc(qint64 time);
    /*!
     * Update current lrc by given state.
     */
    void updateCurrentLrc(MusicLrcAnalysis::State state);
    /*!
     * Get current lrc text in middle.
     */
    QString text() const;
    /*!
     * Find song time position by given time, return new time.
     */
    qint64 findTimePosition(qint64 time);

    /*!
     * Set lrc display expand option.
     */
    inline void setLrcDisplayExpand(bool expand) noexcept { m_lrcDisplayAll = expand; }
    /*!
     * Get lrc display expand option.
     */
    inline bool lrcDisplayExpand() const noexcept { return m_lrcDisplayAll; }
    /*!
     * Get state of background is artist shown.
     */
    inline bool isShowArtistBackground() const noexcept { return m_showArtistBackground; }

    /*!
     * Set current lrc size.
     */
    void setLrcSize(int size);
    /*!
     * Get current lrc size.
     */
    int lrcSize() const noexcept;
    /*!
     * Resize window bound by resize called.
     */
    void resizeWindow();

    /*!
     * Create float play widget.
     */
    void createFloatPlayWidget();
    /*!
     * Show full container or not.
     */
    void lrcWidgetShowFullScreen();

Q_SIGNALS:
    /*!
     * The art background state has changed.
     */
    void backgroundChanged();
    /*!
     * Update current play time when user adjust the progress.
     */
    void updateCurrentTime(qint64 time);

public Q_SLOTS:
    /*!
     * Set current lrc size by action.
     */
    void lrcSizeChanged(QAction *action);
    /*!
     * Set lrc time position changed by action.
     */
    void lrcTimePositionChanged(QAction *action);
    /*!
     * Revert lrc time position.
     */
    void revertTimePosition();
    /*!
     * Save lrc time changed to current lrc file.
     */
    void saveLrcTimeChanged();
    /*!
     * The artist background state has changed.
     */
    void artistBackgroundChanged();
    /*!
     * Open current lrc file location.
     */
    void lrcOpenFileDir() const;
    /*!
     * Copy all lrcs to clipboard.
     */
    void lrcCopyClipboard() const;
    /*!
     * Show local link widget.
     */
    void showLocalLinkWidget();
    /*!
     * Show song comments widget.
     */
    void showSongCommentsWidget();
    /*!
     * Show lrc poster widget.
     */
    void showLrcPosterWidget();

private Q_SLOTS:
    /*!
     * Show video movie widget.
     */
    void showSongMovieWidget();
    /*!
     * Animation finished.
     */
    void updateAnimationLrc();
    /*!
     * Get all lrcs from tr container.
     */
    void translatedLrcData();
    /*!
     * Update voice remove mode.
     */
    void updateVoiceRemove(bool v = true);

private:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override final;
    virtual void mousePressEvent(QMouseEvent *event) override final;
    virtual void mouseMoveEvent(QMouseEvent *event) override final;
    virtual void mouseReleaseEvent(QMouseEvent *event) override final;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override final;
    virtual void paintEvent(QPaintEvent *event) override final;
    virtual void resizeEvent(QResizeEvent *event) override final;

    /*!
     * Init function label widget.
     */
    void initFunctionLabel();
    /*!
     * Create color menu.
     */
    void createColorMenu(QMenu *menu);
    /*!
     * Set lrc time position by pos.
     */
    void setTimePosition(qint64 pos);
    /*!
     * Create no lrc current information.
     */
    void createNoLrcCurrentInfo();
    /*!
     * Show no lrc current information.
     */
    void showNoLrcCurrentInfo();
    /*!
     * Init current lrc when the first show.
     */
    void initCurrentLrc(const QString &str);
    /*!
     * Set per lrc line style sheet.
     */
    void setItemStyleSheet();
    /*!
     * Set per lrc line style sheet by index and size and transparent.
     */
    void setItemStyleSheet(int index, int size, int transparent);
    /*!
     * Map lrc size property to line index.
     */
    int mapLrcSizeProperty(int size);
    /*!
     * Set current lrc size property.
     */
    void setLrcSizeProperty(int property);
    /*!
     * Resize width bound by given width.
     */
    void resizeWidth(int w, int h);

    QPoint m_mousePressedAt;
    int m_lrcChangeOffset, m_lrcChangeDelta;
    bool m_mouseLeftPressed, m_lrcChangeState, m_mouseMoved;

    bool m_lrcDisplayAll, m_showArtistBackground;
    int m_animationFreshTime, m_lrcSizeProperty;

    qint64 m_timePositionOffset;
    QWidget *m_functionLabel, *m_mufflerButton;
    MusicLrcFloatWidget *m_lrcFloatWidget;
    MusicLrcFloatPlayWidget *m_floatPlayWidget;
    TTKClickedLabel *m_noLrcCurrentInfo;
    MusicLrcCommentsWidget *m_commentsWidget;
    MusicLrcTranslatedWidget *m_translatedWidget;
    MusicVLayoutAnimationWidget *m_layoutWidget;

};

#endif // MUSICLRCCCONTAINERFORINTERIOR_H
