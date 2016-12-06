#ifndef MUSICBACKGROUNDPALETTEWIDGET_H
#define MUSICBACKGROUNDPALETTEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include "musicabstractmovedialog.h"

namespace Ui {
class MusicBackgroundPaletteWidget;
}

/*! @brief The class of the background palette label.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicBackgroundPalette : public QLabel
{
    Q_OBJECT
public:
    explicit MusicBackgroundPalette(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicBackgroundPalette();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setPixmap(const QColor &color);
    /*!
     * Set current label background by color.
     */

Q_SIGNALS:
    void currentColorToFileChanged(const QColor &color);
    /*!
     * Current color confirm changed.
     */
    void currentColorToMemoryChanged(const QString &path);
    /*!
     * Current color memory confirm changed.
     */

protected:
    virtual void enterEvent(QEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    /*!
     * Override the widget event.
     */
    void copyColorToMemory(const QColor &color);
    /*!
     * Copy color to memory.
     */

    QColor m_color;

};


/*! @brief The class of the background palette widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicBackgroundPaletteWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicBackgroundPaletteWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicBackgroundPaletteWidget();

    static QString getClassName();
    /*!
    * Get class object name.
    */
    void updateBackground(const QString &text);
    /*!
     * Update this bg when user change the current bg.
     */

Q_SIGNALS:
    void currentColorToFileChanged(const QString &path);
    /*!
     * Current color path confirm changed.
     */
    void currentColorToMemoryChanged(const QString &path);
    /*!
     * Current color memory confirm changed.
     */

public Q_SLOTS:
    void paletteColorClicked();
    /*!
     * Palette color confirm.
     */
    void showPaletteDialog();
    /*!
     * Change index to palette dialog.
     */
    void currentColorToFile(const QColor &color);
    /*!
     * Current color confirm.
     */
    void currentColorToMemory(const QString &color);
    /*!
     * Current color memory confirm.
     */
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    Ui::MusicBackgroundPaletteWidget *m_ui;
    QList<QWidget*> m_widgets;
    QColor m_currentColor;
    bool m_confirmButtonClicked;

};

#endif // MUSICBACKGROUNDPALETTEWIDGET_H
