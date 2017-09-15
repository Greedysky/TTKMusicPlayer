#ifndef MUSICCOLORDIALOG_H
#define MUSICCOLORDIALOG_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

/*! @brief The class of the color hl palette.
 * @author ToDo <278488569@qq.com>
 */
class MUSIC_WIDGET_EXPORT MusicHlPalette : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicHlPalette(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Get color.
     */
    QColor color() const;
    /*!
     * Init parameter.
     */
    void init();

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

protected:
    /*!
     * Override the widget event.
     */
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
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
class MUSIC_WIDGET_EXPORT MusicHlSaturationPalette : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicHlSaturationPalette(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
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

protected:
    /*!
     * Override the widget event.
     */
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
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
class MUSIC_WIDGET_EXPORT MusicColorDialog : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicColorDialog(QWidget *parent = 0);

    virtual ~MusicColorDialog();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Get color static function.
     */
    static QColor getColor(QWidget *parent = 0);
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
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    Ui::MusicColorDialog *m_ui;

    int m_status;
    QColor m_color;

};

#endif // MUSICCOLORDIALOG_H
