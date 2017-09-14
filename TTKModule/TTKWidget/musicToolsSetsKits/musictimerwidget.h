#ifndef MUSICTIMERWIDGET_H
#define MUSICTIMERWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMouseEvent>
#include "musicobject.h"
#include "musicabstractmovedialog.h"

namespace Ui {
class MusicTimerWidget;
}

/*! @brief The class of the timer widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicTimerWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicTimerWidget(QWidget *parent = 0);

    virtual ~MusicTimerWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set current songs path list.
     */
    void setSongStringList(const QStringList &list);

public Q_SLOTS:
    /*!
     * Change to index widget.
     */
    void changeWidgetIndex(int index);
    /*!
     * Save the change results.
     */
    void commitTheResults();
    /*!
     * Widget interface control enable or not.
     */
    void buttonClicked(int index);
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    /*!
     * Init time to play widget.
     */
    void initFirstWidget();
    /*!
     * Init time to stop widget.
     */
    void initSecondWidget();
    /*!
     * Init time to shutdown widget.
     */
    void initThreeWidget();
    /*!
     * Init play widget control enable or not.
     */
    void setEnabledFirstControl(bool enable);
    /*!
     * Init stop widget control enable or not..
     */
    void setEnabledSecondControl(bool enable);
    /*!
     * Init shutdown widget control enable or not..
     */
    void setEnabledThreeControl(bool enable);
    /*!
     * Init combobox parameter.
     */
    void initComboParameter();
    /*!
     * Init widget interface parameter.
     */
    void initParemeter();
    /*!
     * Init widget interface parameter.
     */
    void writeParemeter() const;

    Ui::MusicTimerWidget *m_ui;
    QStringList m_hour;
    QStringList m_second;
    QStringList m_repeat;

};

#endif // MUSICTIMERWIDGET_H
