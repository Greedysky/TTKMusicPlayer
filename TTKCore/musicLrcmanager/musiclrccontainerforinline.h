#ifndef MUSICLRCCCONTAINERFORINLINELR_H
#define MUSICLRCCCONTAINERFORINLINELR_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QBoxLayout>
#include "musiclrccontainer.h"

class MusicLrcFloatWidget;
class MusicClickedLabel;
class MusicLrcAnalysis;
class MusicLrcCommentsWidget;
class MusicLrcTranslatedWidget;
class MusicLayoutAnimation;

/*! @brief The class of the inline lrc container.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcContainerForInline : public MusicLrcContainer
{
    Q_OBJECT
public:
    explicit MusicLrcContainerForInline(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicLrcContainerForInline();

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
    virtual void setSettingParameter();
    /*!
     * Set setting parameter.
     */

    void updateCurrentLrc(qint64 time);
    /*!
     * Update current lrc by given time.
     */
    bool transLyricFileToTime(const QString &lrcFileName);
    /*!
     * Analysis lyric file to map return the state.
     */
    QString text() const;
    /*!
     * Get current lrc text in middle.
     */
    qint64 setSongSpeedAndSlow(qint64 time);
    /*!
     * Set song speed and slow by given time, return new time.
     */
    bool findText(qint64 total, QString &pre, QString &last, qint64 &interval) const;
    /*!
     * Get current lrc and next lrc in container by current time.
     */

    inline void setLrcDisplayExpand(bool expand) { m_lrcDisplayAll = expand;}
    /*!
     * Get state of background is artist shown.
     */
    inline bool lrcDisplayExpand() const { return m_lrcDisplayAll;}
    /*!
     * Get state of background is artist shown.
     */
    inline bool artBackgroundIsShow() const { return m_showArtBackground;}
    /*!
     * Get state of background is artist shown.
     */
    void setLrcSize(MusicLRCManager::LrcSizeTable = MusicLRCManager::Middle) const;
    /*!
     * Set current lrc size.
     */
    int getLrcSize() const;
    /*!
     * Get current lrc size.
     */

Q_SIGNALS:
    void updateCurrentTime(qint64 time);
    /*!
     * Update current play time when user adjust the progress.
     */
    void theArtBgHasChanged();
    /*!
     * The art background state has changed emit.
     */

public Q_SLOTS:
    void lrcSizeChanged(QAction *action) const;
    /*!
     * Set current lrc size by action.
     */
    void lrcTimeSpeedChanged(QAction *action);
    /*!
     * Set lrc time speed changed by action.
     */
    void revertLrcTimeSpeed();
    /*!
     * Revert lrc time speed.
     */
    void saveLrcTimeChanged();
    /*!
     * Save lrc time changed to current lrc file.
     */
    void theArtBgChanged();
    /*!
     * The art background state has changed.
     */
    void theArtBgUploaded();
    /*!
     * The art background has uploaded.
     */
    void lrcOpenFileDir() const;
    /*!
     * Open current lrc file location.
     */
    void lrcCopyClipboard() const;
    /*!
     * Copy all lrcs to clipboard.
     */
    void showLocalLinkWidget();
    /*!
     * Show local link widget.
     */
    void showSongCommentsWidget();
    /*!
     * Show song comments widget.
     */

private Q_SLOTS:
    void getTranslatedLrcFinished(const QString &data);
    /*!
     * Get translated lrc finished.
     */
    void musicSongMovieClicked();
    /*!
     * Video button clicked.
     */
    void updateAnimationLrc();
    /*!
     * Animation finished.
     */

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    /*!
     * Override the widget event.
     */
    void changeLrcPostion(const QString &type);
    /*!
     * Change lrc postion by diff type mouse or wheel.
     */
    void revertLrcTimeSpeed(qint64 pos);
    /*!
     * Revert lrc time speed by pos.
     */
    void createNoLrcCurrentInfo();
    /*!
     * Create no lrc current information.
     */
    void showNoLrcCurrentInfo();
    /*!
     * Show no lrc current information.
     */
    void initCurrentLrc(const QString &str);
    /*!
     * Init current lrc when the first show.
     */
    void initFunctionLabel();
    /*!
     * Init function label widget.
     */
    void setItemStyleSheet();
    /*!
     * Set per lrc line style sheet.
     */
    void setItemStyleSheet(int index, int size, int transparent);
    /*!
     * Set per lrc line style sheet by index and size and transparent.
     */
    void resizeWidth(int w, int h);
    /*!
     * Resize width bound by given width.
     */

    QPoint m_mousePressedAt, m_mouseMovedAt;
    bool m_mouseLeftPressed, m_showArtBackground;
    bool m_lrcDisplayAll;
    int m_animationFreshTime;
    qint64 m_changeSpeedValue;
    MusicLrcFloatWidget *m_lrcFloatWidget;
    MusicClickedLabel *m_noLrcCurrentInfo;
    MusicLrcAnalysis *m_lrcAnalysis;
    MusicLrcCommentsWidget *m_commentsWidget;
    MusicLrcTranslatedWidget *m_translatedWidget;
    MusicLayoutAnimation *m_layoutWidget;

};

#endif // MUSICLRCCCONTAINERFORINLINELR_H
