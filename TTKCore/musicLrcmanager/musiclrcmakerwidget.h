#ifndef MUSICLRCMAKERWIDGET_H
#define MUSICLRCMAKERWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovewidget.h"

namespace Ui {
class MusicLrcMakerWidget;
}
class MusicLrcAnalysis;
class MusicLRCManagerForInline;

/*! @brief The class of the lrc maker widget item.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcMakerWidgetItem : public QLabel
{
    Q_OBJECT
public:
    explicit MusicLrcMakerWidgetItem(QWidget *ui = 0, QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    bool biggerThan(int value) const;
    /*!
     * Check the last index.
     */
    bool done() const;
    /*!
     * Current text line make done.
     */
    void reset();
    /*!
     * Item move reset.
     */
    void moveUp();
    /*!
     * Item move up.
     */
    void moveDown();
    /*!
     * Item move down.
     */
    void moveLeft();
    /*!
     * Item move left.
     */
    void moveRight();
    /*!
     * Item move right.
     */

public Q_SLOTS:
    void setText(const QString &string);
    /*!
     * Override the setText.
     */

private:
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */

    QObject *m_parentObject;
    bool m_leftDirection, m_painetLineDone;
    int m_intervalCount, m_itemDelta;
    int m_currentIndex, m_paintIndex;

};


/*! @brief The class of the lrc maker widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcMakerWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    explicit MusicLrcMakerWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicLrcMakerWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setCurrentSongName(const QString &name);
    /*!
     * Set current song name.
     */

public Q_SLOTS:
    void positionChanged(qint64 position);
    /*!
     * Set current player position.
     */
    void durationChanged(qint64 duration);
    /*!
     * Set current player duration.
     */
    void currentLineFinished();
    /*!
     * Create current line finished.
     */
    void show();
    /*!
     * Override show function.
     */

private Q_SLOTS:
    void timeSliderValueChanged(int value);
    /*!
     * Time slider value changed.
     */
    void saveButtonClicked();
    /*!
     * Save lrc button clicked.
     */
    void reMakeButtonClicked();
    /*!
     * Remake lrc button clicked.
     */
    void backToMakeLrcWidget();
    /*!
     * Back to make lrc widget.
     */
    void firstWidgetStateButtonClicked();
    /*!
     * First widget state changed.
     */
    void thirdWidgetStateButtonClicked();
    /*!
     * Third widget state changed.
     */
    void setCurrentMainWidget();
    /*!
     * Change current widget to main widget.
     */
    void setCurrentFirstWidget();
    /*!
     * Change current widget to first widget.
     */
    void setCurrentSecondWidget();
    /*!
     * Change current widget to second widget.
     */
    void setCurrentThirdWidget();
    /*!
     * Change current widget to third widget.
     */
    void updateAnimationLrc();
    /*!
     * Animation finished.
     */
    void lrcSpeedSlower();
    /*!
     * Set lrc time speed changed slower.
     */
    void lrcSpeedFaster();
    /*!
     * Set lrc time speed changed faster.
     */

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    /*!
     * Override the widget event.
     */
    void createCurrentLine(int key);
    /*!
     * Create current line.
     */
    void createMainWidget();
    /*!
     * Create current main widget.
     */
    void createFirstWidget();
    /*!
     * Create current first widget.
     */
    void createSecondWidget();
    /*!
     * Create current second widget.
     */
    void createThirdWidget();
    /*!
     * Create current third widget.
     */

    bool checkInputValid();
    /*!
     * Check input valid or not.
     */
    void setControlEnable(bool enable) const;
    /*!
     * Set controller enable or disable.
     */
    QString translateTimeString(qint64 time);
    /*!
     * Translate current long time to string.
     */
    void resetToOriginPlayMode();
    /*!
     * Reset to origin play mode.
     */
    void updateCurrentLrc(qint64 time);
    /*!
     * Update current lrc by given time.
     */
    void setItemStyleSheet(int index, int size, int transparent);
    /*!
     * Set per lrc line style sheet.
     */

    Ui::MusicLrcMakerWidget *m_ui;
    QStringList m_plainText;
    QMap<int, qint64> m_times;

    int m_currentLine, m_playMode;
    qint64 m_intervalTime;
    MusicLrcMakerWidgetItem *m_lineItem;
    MusicLrcAnalysis *m_analysis;
    QList<MusicLRCManagerForInline*> m_musicLrcContainer;

};

#endif // MUSICLRCMAKERWIDGET_H
