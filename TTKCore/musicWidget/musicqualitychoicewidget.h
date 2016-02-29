#ifndef MUSICQUALITYCHOICEWIDGET_H
#define MUSICQUALITYCHOICEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QToolButton>
#include "musicabstracttablewidget.h"

/*! @brief The class of the quality choice table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicQualityChoiceTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicQualityChoiceTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicQualityChoiceTableWidget();

public Q_SLOTS:
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Table widget list cell click.
     */
    void createItems();
    /*!
     * Create Table widget item.
     */

};


class QMenu;

/*! @brief The class of the quality choice widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicQualityChoiceWidget : public QToolButton
{
    Q_OBJECT
public:
    explicit MusicQualityChoiceWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicQualityChoiceWidget();

Q_SIGNALS:
    void researchQueryByQuality();
    /*!
     * Research query by quality it changed emit.
     */

public Q_SLOTS:
    void listCellClicked(int row);
    /*!
     * Current Table item clicked by index.
     */
    void getQualityString(QString &string);
    /*!
     * Get current selected quality string.
     */

protected:
    void initWidget();
    /*!
     * Create all widget in layout.
     */

    QMenu *m_menu;
    QString m_currentQuality;

};

#endif // MUSICQUALITYCHOICEWIDGET_H
