#ifndef MUSICEMOJILABELWIDGET_H
#define MUSICEMOJILABELWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include "musicglobaldefine.h"

class QToolButton;

/*! @brief The class of the emoji label widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicEMOJILabelWidget : public QLabel
{
    Q_OBJECT
public:
    explicit MusicEMOJILabelWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicEMOJILabelWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

Q_SIGNALS:
    void dataChanged(const QString &data);
    /*!
     * Get current label emoji.
     */

private Q_SLOTS:
    void buttonClicked(int index);
    /*!
     * Paging button changed.
     */
    void labelClicked(int index);
    /*!
     * Label selected state changed.
     */

protected:
    virtual void leaveEvent(QEvent *event) override;
    /*!
     * Override the widget event.
     */

    int m_currentPage;
    QList<QLabel*> m_labelItems;
    QList<QToolButton*> m_buttonItems;
    QStringList m_datas;

};

#endif // MUSICEMOJILABELWIDGET_H
