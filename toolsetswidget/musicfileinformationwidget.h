#ifndef MUSICFILEINFORMATIONWIDGET_H
#define MUSICFILEINFORMATIONWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QDialog>
#include <QMouseEvent>
#include "musicmovedialogabstract.h"

namespace Ui {
class MusicFileInformationWidget;
}

class MUSIC_WIDGET_EXPORT MusicFileInformationWidget : public MusicMoveDialogAbstract
{
    Q_OBJECT
public:
    explicit MusicFileInformationWidget(QWidget *parent = 0);
    ~MusicFileInformationWidget();

    void setFileInformation(const QString &name);

signals:
public slots:
    void musicOpenFileDir();

protected:
    Ui::MusicFileInformationWidget *ui;
    QString m_path;

};

#endif // MUSICFILEINFORMATIONWIDGET_H
