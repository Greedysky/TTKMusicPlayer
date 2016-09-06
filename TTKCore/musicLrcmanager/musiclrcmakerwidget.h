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
    void show();
    /*!
     * Override show function.
     */

private Q_SLOTS:
    void saveButtonClicked();
    /*!
     * Save lrc button clicked.
     */
    void reMakeButtonClicked();
    /*!
     * Save lrc button clicked.
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

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    /*!
     * Override the widget event.
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

    Ui::MusicLrcMakerWidget *ui;
    QFile m_file;
    QStringList m_plainText;
    QMap<int, QString> m_times;
    int m_currentLine, m_playMode;

};

#endif // MUSICLRCMAKERWIDGET_H
