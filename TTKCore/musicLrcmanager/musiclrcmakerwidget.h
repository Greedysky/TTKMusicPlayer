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
    void setCurrentPosition(qint64 pos) ;
    /*!
     * Save player current play position.
     */
    void makeButtonClicked();
    /*!
     * make lrc button clicked.
     */
    void saveButtonClicked();
    /*!
     * Save lrc button clicked.
     */
    void reviewButtonClicked();
    /*!
     * Review lrc button clicked.
     */
    void show();
    /*!
     * Override show function.
     */

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    /*!
     * Override the widget event.
     */
    void setControlEnable(bool enable) const;
    /*!
     * Set controller enable or disable.
     */
    QString translateTimeString(qint64 time);
    /*!
     * Translate current long time to string.
     */

    int m_currentLine;
    Ui::MusicLrcMakerWidget *ui;
    QFile m_file;
    QStringList m_plainText;
    qint64 m_position;

};

#endif // MUSICLRCMAKERWIDGET_H
