#ifndef MUSICGIFLABELWIDGET_H
#define MUSICGIFLABELWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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

#include <QLabel>
#include "musicglobaldefine.h"

class QTimer;

/*! @brief The class of the float gif label widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicGifLabelWidget : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicGifLabelWidget)
public:
    enum Type
    {
        BallonWhite,         /*!< gif ballon white*/
        CicleBlue,           /*!< gif cicle blue*/
        RiceFontWhite,       /*!< gif rice font white*/
        RiceFontBlack,       /*!< gif rice font black*/
        RiceFontBlackBig,    /*!< gif rice font black big*/
        HourglassWhite,      /*!< gif hourglass white*/
        RadioBlue,           /*!< gif radio blue*/
        CheckBlue,           /*!< gif check blue*/
        RecordRed,           /*!< gif record red*/
        CloseWhite           /*!< gif close white*/
    };

    /*!
     * Object contsructor.
     */
    explicit MusicGifLabelWidget(QWidget *parent = nullptr);
    /*!
     * Object contsructor.
     */
    explicit MusicGifLabelWidget(Type type, QWidget *parent = nullptr);
    ~MusicGifLabelWidget();

    /*!
     * Set the gif type.
     */
    void setType(Type type);
    /*!
     * Get the gif type.
     */
    Type type() const;

    /*!
     * Set the gif interval.
     */
    void setInterval(int value);
    /*!
     * Get the gif interval.
     */
    int interval() const;

    /*!
     * Set the gif infinited mode.
     */
    void setInfinited(bool s);
    /*!
     * Get the gif infinited mode.
     */
    bool infinited() const;

    /*!
     * Run the gif.
     */
    void run(bool run);
    /*!
     * Get current running state.
     */
    inline bool isRunning() const { return m_isRunning; }

    /*!
     * Start the gif.
     */
    void start();
    /*!
     * Stop the gif.
     */
    void stop();

public Q_SLOTS:
    /*!
     * Change the current gif index.
     */
    void timeout();

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;
    /*!
     * Infinited mode check.
     */
    bool infinitedModeCheck();

    int m_index;
    Type m_type;
    QTimer *m_timer;
    QPixmap m_renderer;
    bool m_isRunning, m_infinited;

};


/*! @brief The class of the float gif label mask widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicGifLabelMaskWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicGifLabelMaskWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicGifLabelMaskWidget(QWidget *parent = nullptr);
    ~MusicGifLabelMaskWidget();

    /*!
     * Set the gif type.
     */
    void setType(MusicGifLabelWidget::Type type);
    /*!
     * Get the gif type.
     */
    MusicGifLabelWidget::Type type() const;

    /*!
     * Run the gif.
     */
    void run(bool run);

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;

    MusicGifLabelWidget *m_gifLabel;

};

#endif // MUSICGIFLABELWIDGET_H
