#ifndef MUSICLRCMAKERWIDGET_H
#define MUSICLRCMAKERWIDGET_H

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

#include "musicabstractmovewidget.h"

namespace Ui {
class MusicLrcMakerWidget;
}
class MusicLrcAnalysis;
class MusicLrcManagerForInterior;

/*! @brief The class of the lrc maker widget item.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcMakerWidgetItem : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcMakerWidgetItem)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcMakerWidgetItem(QWidget *ui = nullptr, QObject *parent = nullptr);

    /*!
     * Check the last index.
     */
    bool biggerThan(int value) const;
    /*!
     * Current text line make done.
     */
    bool done() const noexcept;
    /*!
     * Item move reset.
     */
    void reset();
    /*!
     * Item move up.
     */
    void moveUp();
    /*!
     * Item move down.
     */
    void moveDown();
    /*!
     * Item move left.
     */
    void moveLeft();
    /*!
     * Item move right.
     */
    void moveRight();

public Q_SLOTS:
    /*!
     * Override the setText.
     */
    void setText(const QString &string);

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;

    QObject *m_parent;
    bool m_leftDirection, m_painetLineDone;
    int m_intervalCount, m_itemDelta;
    int m_currentIndex, m_paintIndex;

};


/*! @brief The class of the lrc maker widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcMakerWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcMakerWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcMakerWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicLrcMakerWidget();

    /*!
     * Set current song name.
     */
    void setCurrentSongName(const QString &name);

public Q_SLOTS:
    /*!
     * Set current player position.
     */
    void positionChanged(qint64 position);
    /*!
     * Set current player duration.
     */
    void durationChanged(qint64 duration);
    /*!
     * Create current line finished.
     */
    void currentLineFinished();

private Q_SLOTS:
    /*!
     * Time slider value changed.
     */
    void timeSliderValueChanged(int value);
    /*!
     * Save lrc button clicked.
     */
    void saveButtonClicked();
    /*!
     * Remake lrc button clicked.
     */
    void reMakeButtonClicked();
    /*!
     * Back to make lrc widget.
     */
    void backToMakeLrcWidget();
    /*!
     * First widget state changed.
     */
    void firstWidgetStateButtonClicked();
    /*!
     * Third widget state changed.
     */
    void thirdWidgetStateButtonClicked();
    /*!
     * Change current widget to main widget.
     */
    void setCurrentMainWidget();
    /*!
     * Change current widget to first widget.
     */
    void setCurrentFirstWidget();
    /*!
     * Change current widget to second widget.
     */
    void setCurrentSecondWidget();
    /*!
     * Change current widget to third widget.
     */
    void setCurrentThirdWidget();
    /*!
     * Animation finished.
     */
    void updateAnimationLrc();
    /*!
     * Set lrc time speed changed slower.
     */
    void lrcSpeedSlower();
    /*!
     * Set lrc time speed changed faster.
     */
    void lrcSpeedFaster();

private:
    /*!
     * Override the widget event.
     */
    virtual void keyPressEvent(QKeyEvent *event) override final;
    virtual void keyReleaseEvent(QKeyEvent *event) override final;
    /*!
     * Create current line.
     */
    void createCurrentLine(int key);
    /*!
     * Create current main widget.
     */
    void createMainWidget();
    /*!
     * Create current first widget.
     */
    void createFirstWidget();
    /*!
     * Create current second widget.
     */
    void createSecondWidget();
    /*!
     * Create current third widget.
     */
    void createThirdWidget();
    /*!
     * Check input valid or not.
     */

    bool checkInputValid();
    /*!
     * Set controller enable or disable.
     */
    void setControlEnabled(bool enabled) const;
    /*!
     * Translate current long time to string.
     */
    QString translateTimeString(qint64 time);
    /*!
     * Reset to origin play mode.
     */
    void resetToOriginPlayMode();
    /*!
     * Update current lrc by given time.
     */
    void updateCurrentLrc(qint64 time);
    /*!
     * Set per lrc line style sheet.
     */
    void setItemStyleSheet(int index, int size, int transparent);

    Ui::MusicLrcMakerWidget *m_ui;
    QStringList m_plainText;
    QMap<int, qint64> m_times;
    int m_currentLine;
    TTK::PlayMode m_playMode;
    qint64 m_intervalTime;
    MusicLrcMakerWidgetItem *m_lineItem;
    MusicLrcAnalysis *m_analysis;
    QList<MusicLrcManagerForInterior*> m_lrcContainer;

};

#endif // MUSICLRCMAKERWIDGET_H
