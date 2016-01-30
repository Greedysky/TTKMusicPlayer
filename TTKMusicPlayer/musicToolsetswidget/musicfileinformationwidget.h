#ifndef MUSICFILEINFORMATIONWIDGET_H
#define MUSICFILEINFORMATIONWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QDialog>
#include <QLineEdit>
#include <QMouseEvent>
#include "musicabstractmovedialog.h"

namespace Ui {
class MusicFileInformationWidget;
}

class MUSIC_TOOLSET_EXPORT MusicModifyLineEdit : public QLineEdit
{
public:
    explicit MusicModifyLineEdit(QWidget *parent = 0);
    virtual ~MusicModifyLineEdit();

protected:
    virtual void leaveEvent(QEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

};


class MUSIC_TOOLSET_EXPORT MusicFileInformationWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicFileInformationWidget(QWidget *parent = 0);
    virtual ~MusicFileInformationWidget();

    void setFileInformation(const QString &name);

public Q_SLOTS:
    void musicOpenFileDir();
    virtual int exec();

protected:
    Ui::MusicFileInformationWidget *ui;
    QString m_path;

};

#endif // MUSICFILEINFORMATIONWIDGET_H
