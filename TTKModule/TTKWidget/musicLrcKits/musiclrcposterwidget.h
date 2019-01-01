#ifndef MUSICLRCPOSTERWIDGET_H
#define MUSICLRCPOSTERWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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
 ================================================= */

#include "musicwidgetheaders.h"
#include "musicabstractmovedialog.h"
#include "musicfillitemtablewidget.h"

namespace Ui {
class MusicLrcPosterWidget;
}

/*! @brief The class of the lrc art poster maker item widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcPosterItemWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcPosterItemWidget)
public:
    enum Type
    {
        Type_01,    /*!< thmeme 1*/
        Type_02,    /*!< thmeme 2*/
        Type_03,    /*!< thmeme 3*/
        Type_04,    /*!< thmeme 4*/
        Type_05,    /*!< thmeme 5*/
        Type_06,    /*!< thmeme 6*/
        Type_07,    /*!< thmeme 7*/
        Type_08,    /*!< thmeme 8*/
        Type_09,    /*!< thmeme 9*/
        Type_10,    /*!< thmeme 10*/
        Type_11,    /*!< thmeme 11*/
        Type_12,    /*!< thmeme 12*/
        Type_13,    /*!< thmeme 13*/
        Type_14,    /*!< thmeme 14*/
        Type_15,    /*!< thmeme 15*/
        Type_16,    /*!< thmeme 16*/
        Type_17     /*!< thmeme 17*/
    };

    /*!
     * Object contsructor.
     */
    explicit MusicLrcPosterItemWidget(QWidget *parent = nullptr);

    /*!
     * Check widget has scroll or not.
     */
    bool hasScroll() const;
    /*!
     * Set image path.
     */
    void setImagePath(const QString &path);
    /*!
     * Set current song name.
     */
    void setCurrentSongName(const QString &name);

public Q_SLOTS:
    /*!
     * Lrc text changed.
     */
    void textChanged(const QStringList &data);
    /*!
     * Theme type changed.
     */
    void currentTypeChanged(int type);

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;
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

    Type m_type;
    QStringList m_data;
    QPixmap m_pixmap;
    QString m_title;

};


/*! @brief The class of the lrc art poster table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcPosterTableWidget : public MusicFillItemTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcPosterTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcPosterTableWidget(QWidget *parent = nullptr);

    /*!
     * Create all items.
     */
    void createAllItems(const QStringList &lrcs);

Q_SIGNALS:
    /*!
     * Lrc text changed.
     */
    void textChanged(const QStringList &data);

public Q_SLOTS:
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;

};


/*! @brief The class of the lrc art poster theme list widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcPosterThemeListWidget : public QListWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcPosterThemeListWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcPosterThemeListWidget(QWidget *parent = nullptr);

public Q_SLOTS:
    /*!
     * Add tool sets list into list widget.
     */
    void addListWidgetItem();

};


/*! @brief The class of the lrc art poster maker widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcPosterWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcPosterWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcPosterWidget(QWidget *parent = nullptr);

    virtual ~MusicLrcPosterWidget();

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
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    Ui::MusicLrcPosterWidget *m_ui;
    MusicLrcPosterItemWidget *m_itemWidget;
    MusicLrcPosterThemeListWidget *m_themeWidget;

};

#endif // MUSICLRCPOSTERWIDGET_H
