#ifndef MUSICLRCPOSTERWIDGET_H
#define MUSICLRCPOSTERWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

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
    explicit MusicLrcPosterItemWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    bool hasScroll() const;
    void setImagePath(const QString &path);
    void setCurrentSongName(const QString &name);

public Q_SLOTS:
    void textChanged(const QStringList &data);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

    QString m_pixPath, m_title;
    QStringList m_data;

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


public Q_SLOTS:
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Table widget list cell click.
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


public Q_SLOTS:
    void openButtonClicked();
    void saveButtonClicked();

    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    Ui::MusicLrcPosterWidget *m_ui;
    MusicLrcPosterItemWidget *m_itemWidget;

};

#endif // MUSICLRCPOSTERWIDGET_H
