#ifndef MUSICCOLORDIALOG_H
#define MUSICCOLORDIALOG_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#include "musicabstractmovedialog.h"

/*! @brief The class of the color hl palette.
 * @author ToDo <278488569@qq.com>
 */
class TTK_MODULE_EXPORT MusicHlPalette : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicHlPalette)
public:
    /*!
     * Object constructor.
     */
    explicit MusicHlPalette(QWidget *parent = nullptr);

    /*!
     * Get color.
     */
    QColor color() const;
    /*!
     * Init parameter.
     */
    void initialize();

Q_SIGNALS:
    /*!
     * Current color has changed.
     */
    void colorChanged(const QColor &color);

public Q_SLOTS:
    /*!
     * Set current color saturation.
     */
    void setSaturation(double dblsaturation);

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;
    virtual void resizeEvent(QResizeEvent *event) override final;
    virtual void mousePressEvent(QMouseEvent *event) override final;
    virtual void mouseMoveEvent(QMouseEvent *event) override final;
    /*!
     * Calculate current color.
     */
    void calculateColor();

    QPoint m_ptVernierPos;
    QPointF m_ptfVernierPercentPos;
    QColor m_color;
    double m_dblSaturation;

};


/*! @brief The class of the color hl saturation palette.
 * @author ToDo <278488569@qq.com>
 */
class TTK_MODULE_EXPORT MusicHlSaturationPalette : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicHlSaturationPalette)
public:
    /*!
     * Object constructor.
     */
    explicit MusicHlSaturationPalette(QWidget *parent = nullptr);

    /*!
     * Get saturation.
     */
    double saturation() const;

Q_SIGNALS:
    /*!
     * Current saturation has changed.
     */
    void saturationChanged(double dblSaturation);

public Q_SLOTS:
    /*!
     * Set current color.
     */
    void setBaseColor(const QColor &color);

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;
    virtual void resizeEvent(QResizeEvent *event) override final;
    virtual void mousePressEvent(QMouseEvent *event) override final;
    virtual void mouseMoveEvent(QMouseEvent *event) override final;
    /*!
     * Calculate current saturation.
     */
    void calculateSuration();

    QColor m_color;
    double m_dblVernierX, m_dblVernierPercentX, m_dblSaturation;

};


namespace Ui {
class MusicColorDialog;
}

/*! @brief The class of the get color table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicColorDialog : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicColorDialog)
public:
    /*!
     * Object constructor.
     */
    explicit MusicColorDialog(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicColorDialog();

    /*!
     * Get color static function.
     */
    static QColor popup(QWidget *parent = nullptr);
    /*!
     * Get color.
     */
    QColor color() const;

public Q_SLOTS:
    /*!
     * Confirm or cancal button clicked, set the state.
     */
    void buttonClicked(int index);
    /*!
     * Current color has changed.
     */
    void colorChanged(const QColor &color);

private:
    Ui::MusicColorDialog *m_ui;
    QColor m_color;

};

#endif // MUSICCOLORDIALOG_H
