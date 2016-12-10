#ifndef MUSICQUALITYCHOICEWIDGET_H
#define MUSICQUALITYCHOICEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musictoolmenuwidget.h"
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

    static QString getClassName();
    /*!
     * Get class object name.
     */

public Q_SLOTS:
    virtual void listCellEntered(int row, int column);
    /*!
     * Table widget list cell enter.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Table widget list cell click.
     */
    void createItems();
    /*!
     * Create Table widget item.
     */

};


/*! @brief The class of the quality choice widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicQualityChoiceWidget : public MusicToolMenuWidget
{
    Q_OBJECT
public:
    explicit MusicQualityChoiceWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
    * Get class object name.
    */

Q_SIGNALS:
    void researchQueryByQuality(const QString &quality);
    /*!
     * Research query by quality it changed.
     */

public Q_SLOTS:
    void listCellClicked(int row);
    /*!
     * Current Table item clicked by index.
     */

protected:
    void initWidget();
    /*!
     * Create all widget in layout.
     */

    QString m_currentQuality;

};

#endif // MUSICQUALITYCHOICEWIDGET_H
