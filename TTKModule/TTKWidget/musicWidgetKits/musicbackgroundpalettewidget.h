#ifndef MUSICBACKGROUNDPALETTEWIDGET_H
#define MUSICBACKGROUNDPALETTEWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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
#include "musicabstractmovedialog.h"

namespace Ui {
class MusicBackgroundPaletteWidget;
}

/*! @brief The class of the background palette label.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicBackgroundPalette : public QLabel
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicBackgroundPalette(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicBackgroundPalette();

    /*!
     * Set current label background by color.
     */
    void setPixmap(const QColor &color);

Q_SIGNALS:
    /*!
     * Current color confirm changed.
     */
    void currentColorToFileChanged(const QColor &color);
    /*!
     * Current color memory confirm changed.
     */
    void currentColorToMemoryChanged(const QString &path);

private:
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QtEnterEvent *event) override final;
    virtual void mousePressEvent(QMouseEvent *event) override final;
    /*!
     * Copy color to memory.
     */
    void copyColorToMemory(const QColor &color);

    QColor m_color;

};


/*! @brief The class of the background palette widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicBackgroundPaletteWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicBackgroundPaletteWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicBackgroundPaletteWidget();

    /*!
     * Update this background when user change the current background.
     */
    void updateBackground(const QString &path);

Q_SIGNALS:
    /*!
     * Current color path confirm changed.
     */
    void currentColorToFileChanged(const QString &path);
    /*!
     * Current color memory confirm changed.
     */
    void currentColorToMemoryChanged(const QString &path);

public Q_SLOTS:
    /*!
     * Palette color confirm.
     */
    void paletteColorClicked();
    /*!
     * Change index to palette dialog.
     */
    void showPaletteDialog();
    /*!
     * Current color confirm.
     */
    void currentColorToFile(const QColor &color);
    /*!
     * Current color memory confirm.
     */
    void currentColorToMemory(const QString &color);
    /*!
     * Override exec function.
     */
    virtual int exec();

private:
    Ui::MusicBackgroundPaletteWidget *m_ui;
    QWidgetList m_container;
    QColor m_currentColor;
    QString m_previousBackground;
    bool m_confirmButtonClicked;

};

#endif // MUSICBACKGROUNDPALETTEWIDGET_H
