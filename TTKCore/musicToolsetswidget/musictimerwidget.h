#ifndef MUSICTIMERWIDGET_H
#define MUSICTIMERWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
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
    explicit MusicTimerWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicTimerWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setSongStringList(const QStringList &list);
    /*!
     * Set current songs path list.
     */

Q_SIGNALS:
    void timerParameterChanged();
    /*!
     * Timer parameter changed it emit.
     */

public Q_SLOTS:
    void changeFirstWidget();
    /*!
     * Change to time to play widget.
     */
    void changeSecondWidget();
    /*!
     * Change to time to stop widget.
     */
    void changeThreeWidget();
    /*!
     * Change to time to shutdown widget.
     */
    void commitTheResults();
    /*!
     * Save the change results.
     */
    void buttonClicked(int index);
    /*!
     * Widget interface control enable or not.
     */
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    void initFirstWidget();
    /*!
     * Init time to play widget.
     */
    void initSecondWidget();
    /*!
     * Init time to stop widget.
     */
    void initThreeWidget();
    /*!
     * Init time to shutdown widget.
     */
    void setEnabledFirstControl(bool enable);
    /*!
     * Init play widget control enable or not.
     */
    void setEnabledSecondControl(bool enable);
    /*!
     * Init stop widget control enable or not..
     */
    void setEnabledThreeControl(bool enable);
    /*!
     * Init shutdown widget control enable or not..
     */
    void initComboParameter();
    /*!
     * Init combobox parameter.
     */
    void initParemeter();
    /*!
     * Init widget interface parameter.
     */
    void writeParemeter() const;
    /*!
     * Init widget interface parameter.
     */

    Ui::MusicTimerWidget *ui;
    QStringList m_hour;
    QStringList m_second;
    QStringList m_repeat;

};

#endif // MUSICTIMERWIDGET_H
