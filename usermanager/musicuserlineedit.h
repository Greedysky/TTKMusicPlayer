#ifndef MUSICUSERLINEEDIT_H
#define MUSICUSERLINEEDIT_H

#include <QLineEdit>
#include <QLabel>

#include "musiclibexportglobal.h"

enum LabelType{ User, Passwd, PasswdC, Mail};

class MUSIC_WIDGET_EXPORT MusicUserLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MusicUserLineEdit(QWidget *parent = 0);

    void setLabel(LabelType ty,QLabel* &t, QLabel* &s);

    bool getMailStatus() const { return m_mailContains;}
    bool getStrStatus() const { return m_strContains;}

signals:

public slots:

protected:
    void checkTheInput();
    void showLabel();
    void showLabel(int s, int e);
    virtual void focusInEvent(QFocusEvent *);
    virtual void focusOutEvent(QFocusEvent *);
    QLabel* m_tipsLabel;
    QLabel* m_statusLabel;
    LabelType m_type;
    bool m_mailContains;
    bool m_strContains;

};

#endif // MUSICUSERLINEEDIT_H
