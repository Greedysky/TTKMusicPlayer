#ifndef MUSICLRCPOSTERWIDGET_H
#define MUSICLRCPOSTERWIDGET_H

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

#include "musicwidgetheaders.h"
#include "musicabstractmovedialog.h"
#include "musicfillitemtablewidget.h"

namespace Ui {
class MusicLrcPosterWidget;
}

/*! @brief The class of the lrc art poster maker item widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcPosterItemWidget : public QWidget
{
    Q_OBJECT
public:
    enum class Format
    {
        Type01,    /*!< Theme type 1 */
        Type02,    /*!< Theme type 2 */
        Type03,    /*!< Theme type 3 */
        Type04,    /*!< Theme type 4 */
        Type05,    /*!< Theme type 5 */
        Type06,    /*!< Theme type 6 */
        Type07,    /*!< Theme type 7 */
        Type08,    /*!< Theme type 8 */
        Type09,    /*!< Theme type 9 */
        Type10,    /*!< Theme type 10 */
        Type11,    /*!< Theme type 11 */
        Type12,    /*!< Theme type 12 */
        Type13,    /*!< Theme type 13 */
        Type14,    /*!< Theme type 14 */
        Type15,    /*!< Theme type 15 */
        Type16,    /*!< Theme type 16 */
        Type17     /*!< Theme type 17 */
    };

    /*!
     * Object constructor.
     */
    explicit MusicLrcPosterItemWidget(QWidget *parent = nullptr);

    /*!
     * Check widget has scroll or not.
     */
    bool hasScroll() const noexcept;
    /*!
     * Set image path.
     */
    void setImagePath(const QString &path);
    /*!
     * Set current song name.
     */
    void setCurrentSongName(const QString &name) noexcept;

public Q_SLOTS:
    /*!
     * Lrc text changed.
     */
    void textChanged(const QStringList &data);
    /*!
     * Theme type changed.
     */
    void currentTypeChanged(int type);

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;
    /*!
     * Draw theme.
     */
    void drawTheme1(QPainter *painter);
    /*!
     * Draw theme.
     */
    void drawTheme2(QPainter *painter);
    /*!
     * Draw theme.
     */
    void drawTheme3(QPainter *painter);
    /*!
     * Draw theme.
     */
    void drawTheme4(QPainter *painter);
    /*!
     * Draw theme.
     */
    void drawTheme5(QPainter *painter);
    /*!
     * Draw theme.
     */
    void drawTheme6(QPainter *painter);
    /*!
     * Draw theme.
     */
    void drawTheme7(QPainter *painter);
    /*!
     * Draw theme.
     */
    void drawTheme8(QPainter *painter);
    /*!
     * Draw theme.
     */
    void drawTheme9(QPainter *painter);
    /*!
     * Draw theme.
     */
    void drawTheme10(QPainter *painter);
    /*!
     * Draw theme.
     */
    void drawTheme11(QPainter *painter);
    /*!
     * Draw theme.
     */
    void drawTheme12(QPainter *painter);
    /*!
     * Draw theme.
     */
    void drawTheme13(QPainter *painter);
    /*!
     * Draw theme.
     */
    void drawTheme14(QPainter *painter);
    /*!
     * Draw theme.
     */
    void drawTheme15(QPainter *painter);
    /*!
     * Draw theme.
     */
    void drawTheme16(QPainter *painter);
    /*!
     * Draw theme.
     */
    void drawTheme17(QPainter *painter);

    Format m_type;
    QStringList m_data;
    QPixmap m_pixmap;
    QString m_title;

};


/*! @brief The class of the lrc art poster table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcPosterTableWidget : public MusicFillItemTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcPosterTableWidget(QWidget *parent = nullptr);

    /*!
     * Create cell items by input data.
     */
    void addCellItems(const QStringList &lrcs);

Q_SIGNALS:
    /*!
     * Lrc text changed.
     */
    void textChanged(const QStringList &data);

public Q_SLOTS:
    /*!
     * Table widget item cell click.
     */
    virtual void itemCellClicked(int row, int column) override final;

};


/*! @brief The class of the lrc art poster theme list widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcPosterThemeListWidget : public QListWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcPosterThemeListWidget(QWidget *parent = nullptr);

private:
    /*!
     * Add tool sets list into list widget.
     */
    void addCelltItems();

};


/*! @brief The class of the lrc art poster maker widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcPosterWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcPosterWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicLrcPosterWidget();

    /*!
     * Set current lrc and song name.
     */
    void setCurrentLrcs(const QStringList &lrcs, const QString &name);


public Q_SLOTS:
    /*!
     * Open button clicked.
     */
    void openButtonClicked();
    /*!
     * Save button clicked.
     */
    void saveButtonClicked();

private:
    Ui::MusicLrcPosterWidget *m_ui;
    MusicLrcPosterItemWidget *m_itemWidget;
    MusicLrcPosterThemeListWidget *m_themeWidget;

};

#endif // MUSICLRCPOSTERWIDGET_H
