#ifndef MUSICPROGRESSWIDGET_H
#define MUSICPROGRESSWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QProgressDialog>
#include "musicglobaldefine.h"

class QLabel;

/*! @brief The class of the show progress widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicProgressWidget : public QProgressDialog
{
    Q_OBJECT
public:
    explicit MusicProgressWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicProgressWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setTitle(const QString &name);
    /*!
     * Set widget title.
     */

public Q_SLOTS:
    void show();
    /*!
     * Override show function.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    /*!
     * Override the widget event.
     */
    void initWidget();
    /*!
     * Create all widget in layout.
     */

    QLabel *m_background, *m_textLabel;
    QProgressBar *m_progressBar;

};

#endif // MUSICPROGRESSWIDGET_H
