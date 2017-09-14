#ifndef MUSICBACKGROUNDPALETTEWIDGET_H
#define MUSICBACKGROUNDPALETTEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
    /*!
     * Object contsructor.
     */
    explicit MusicBackgroundPalette(QWidget *parent = 0);

    ~MusicBackgroundPalette();

    /*!
     * Get class object name.
     */
    static QString getClassName();
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

protected:
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    /*!
     * Copy color to memory.
     */
    void copyColorToMemory(const QColor &color);

    QColor m_color;

};


/*! @brief The class of the background palette widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicBackgroundPaletteWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicBackgroundPaletteWidget(QWidget *parent = 0);

    ~MusicBackgroundPaletteWidget();

    /*!
    * Get class object name.
    */
    static QString getClassName();
    /*!
     * Update this bg when user change the current bg.
     */
    void updateBackground(const QString &text);

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

protected:
    Ui::MusicBackgroundPaletteWidget *m_ui;
    QList<QWidget*> m_widgets;
    QColor m_currentColor;
    QString m_previousBackground;
    bool m_confirmButtonClicked;

};

#endif // MUSICBACKGROUNDPALETTEWIDGET_H
