#ifndef MUSICUSERLINEEDIT_H
#define MUSICUSERLINEEDIT_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLineEdit>
#include <QLabel>

#include "musiclibexportglobal.h"

enum LabelType{ User, Passwd, PasswdC, Mail};

class MUSIC_USER_EXPORT MusicUserLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MusicUserLineEdit(QWidget *parent = 0);

    void setLabel(LabelType ty, QLabel *&t, QLabel* &s);
    bool getMailStatus() const { return m_mailContains;}
    bool getStrStatus() const { return m_strContains;}

protected:
    virtual void focusInEvent(QFocusEvent *event);
    virtual void focusOutEvent(QFocusEvent *event);
    void checkTheInput();
    void showLabel();
    void showLabel(int s, int e);

    QLabel *m_tipsLabel;
    QLabel *m_statusLabel;
    LabelType m_type;
    bool m_mailContains;
    bool m_strContains;

};

#endif // MUSICUSERLINEEDIT_H
