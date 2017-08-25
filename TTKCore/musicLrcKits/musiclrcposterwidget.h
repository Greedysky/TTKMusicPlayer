#ifndef MUSICLRCPOSTERWIDGET_H
#define MUSICLRCPOSTERWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QListWidget>
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
public:
    enum Type
    {
        Type_01,
        Type_02
    };

    explicit MusicLrcPosterItemWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    bool hasScroll() const;
    /*!
     * Check widget has scroll or not.
     */
    void setImagePath(const QString &path);
    /*!
     * Set image path.
     */
    void setCurrentSongName(const QString &name);
    /*!
     * Set current song name.
     */

public Q_SLOTS:
    void textChanged(const QStringList &data);
    /*!
     * Lrc text changed.
     */
    void currentTypeChanged(int type);
    /*!
     * Theme type changed.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */
    void drawTheme1(QPainter *painter);
    void drawTheme2(QPainter *painter);

    Type m_type;
    QStringList m_data;
    QString m_pixPath, m_title;

};


/*! @brief The class of the lrc art poster table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcPosterTableWidget : public MusicFillItemTableWidget
{
    Q_OBJECT
public:
    explicit MusicLrcPosterTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void createAllItems(const QStringList &lrcs);
    /*!
     * Create all items.
     */

Q_SIGNALS:
    void textChanged(const QStringList &data);
    /*!
     * Lrc text changed.
     */

public Q_SLOTS:
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Table widget list cell click.
     */

};


/*! @brief The class of the lrc art poster theme list widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcPosterThemeListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MusicLrcPosterThemeListWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

public Q_SLOTS:
    void addListWidgetItem();
    /*!
     * Add tool sets list into list widget.
     */

};


/*! @brief The class of the lrc art poster maker widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcPosterWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicLrcPosterWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicLrcPosterWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setCurrentLrcs(const QStringList &lrcs, const QString &name);
    /*!
     * Set current lrc and song name.
     */

public Q_SLOTS:
    void openButtonClicked();
    /*!
     * Open button clicked.
     */
    void saveButtonClicked();
    /*!
     * Save button clicked.
     */
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    Ui::MusicLrcPosterWidget *m_ui;
    MusicLrcPosterItemWidget *m_itemWidget;
    MusicLrcPosterThemeListWidget *m_themeWidget;

};

#endif // MUSICLRCPOSTERWIDGET_H
