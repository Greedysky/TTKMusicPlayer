#ifndef MUSICEMOJILABELWIDGET_H
#define MUSICEMOJILABELWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
    /*!
     * Object contsructor.
     */
    explicit MusicEMOJILabelWidget(QWidget *parent = 0);

    ~MusicEMOJILabelWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

Q_SIGNALS:
    /*!
     * Get current label emoji.
     */
    void dataChanged(const QString &data);

private Q_SLOTS:
    /*!
     * Paging button changed.
     */
    void buttonClicked(int index);
    /*!
     * Label selected state changed.
     */
    void labelClicked(int index);

protected:
    /*!
     * Override the widget event.
     */
    virtual void leaveEvent(QEvent *event) override;

    int m_currentPage;
    QList<QLabel*> m_labelItems;
    QList<QToolButton*> m_buttonItems;
    QStringList m_datas;

};

#endif // MUSICEMOJILABELWIDGET_H
