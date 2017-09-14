#ifndef MUSICQUALITYCHOICEPOPWIDGET_H
#define MUSICQUALITYCHOICEPOPWIDGET_H

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
    /*!
     * Object contsructor.
     */
    explicit MusicQualityChoiceTableWidget(QWidget *parent = 0);

    virtual ~MusicQualityChoiceTableWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

public Q_SLOTS:
    /*!
     * Table widget list cell enter.
     */
    virtual void listCellEntered(int row, int column);
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Create Table widget item.
     */
    void createItems();

};


/*! @brief The class of the quality choice pop widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicQualityChoicePopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicQualityChoicePopWidget(QWidget *parent = 0);

    /*!
    * Get class object name.
    */
    static QString getClassName();

Q_SIGNALS:
    /*!
     * Research query by quality it changed.
     */
    void researchQueryByQuality(const QString &quality);

public Q_SLOTS:
    /*!
     * Current Table item clicked by index.
     */
    void listCellClicked(int row);

protected:
    /*!
     * Create all widget in layout.
     */
    void initWidget();

    QString m_currentQuality;

};

#endif // MUSICQUALITYCHOICEPOPWIDGET_H
