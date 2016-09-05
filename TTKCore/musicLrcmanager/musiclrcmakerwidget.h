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
    void reviewButtonClicked();
    /*!
     * Review lrc button clicked.
     */
    void firstWidgetStateButtonClicked();
    void thirdWidgetStateButtonClicked();
    void setCurrentMainWidget();
    void setCurrentFirstWidget();
    void setCurrentSecondWidget();
    void setCurrentThirdWidget();

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    /*!
     * Override the widget event.
     */
    void createMainWidget();
    void createFirstWidget();
    void createSecondWidget();
    void createThirdWidget();

    bool checkInputValid();

    void setControlEnable(bool enable) const;
    /*!
     * Set controller enable or disable.
     */
    QString translateTimeString(qint64 time);
    /*!
     * Translate current long time to string.
     */

    Ui::MusicLrcMakerWidget *ui;

    int m_currentLine;
    QFile m_file;
    QStringList m_plainText;

};

#endif // MUSICLRCMAKERWIDGET_H
