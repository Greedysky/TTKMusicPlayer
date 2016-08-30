#ifndef MUSICIDENTIFYSONGSWIDGET_H
#define MUSICIDENTIFYSONGSWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include <QPushButton>
#include "musicglobaldefine.h"

class QMovie;
class QStackedWidget;

/*! @brief The class of the song identify widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicIdentifySongsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicIdentifySongsWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicIdentifySongsWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

public Q_SLOTS:
    void detectedButtonClicked();
    void detectedTimeOut();

protected:
    void createDetectedWidget();
    void createDetectedSuccessedWidget();
    void createDetectedFailedWidget();

    QStackedWidget *m_mainWindow;
    QLabel *m_detectedLabel;
    QMovie *m_detectedMovie;
    QPushButton *m_detectedButton;

};

#endif // MUSICIDENTIFYSONGSWIDGET_H
