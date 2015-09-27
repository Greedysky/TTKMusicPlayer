#include "musicuserlineedit.h"
#include "musicuiobject.h"

#if defined(Q_CC_MSVC)
#   pragma warning(disable:4129)
#endif

MusicUserLineEdit::MusicUserLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    m_mailContains = false;
    m_strContains = false;
}

void MusicUserLineEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    m_tipsLabel->show();
    m_tipsLabel->setStyleSheet(MusicUIObject::MCustomStyle13);
}

void MusicUserLineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    checkTheInput();
}

void MusicUserLineEdit::setLabel(LabelType ty,
                                 QLabel* &t, QLabel* &s)
{
    m_tipsLabel = t;
    m_statusLabel = s;
    m_type = ty;
}

void MusicUserLineEdit::showLabel(int s, int e)
{
    int textLength = text().length();
    if( s <= textLength && textLength <= e)
    {
        m_strContains = true;
        m_tipsLabel->hide();
        m_statusLabel->show();
    }
    else
    {
        m_strContains = false;
        m_tipsLabel->show();
        m_statusLabel->hide();
        m_tipsLabel->setStyleSheet(MusicUIObject::MCustomStyle14);
    }
}

void MusicUserLineEdit::showLabel()
{
    QRegExp mailRx("^[0-9a-z][a-z0-9\._-]{1,}@[a-z0-9-]{1,}[a-z0-9]\.[a-z\.]{1,}[a-z]+$");
    m_mailContains = text().contains(mailRx);
    if(m_mailContains)
    {
        m_tipsLabel->hide();
        m_statusLabel->show();
    }
    else
    {
        m_tipsLabel->show();
        m_statusLabel->hide();
        m_tipsLabel->setStyleSheet(MusicUIObject::MCustomStyle14);
    }
}

void MusicUserLineEdit::checkTheInput()
{
    switch(m_type)
    {
        case User:
             showLabel(4,20);
             break;
        case Passwd:
             showLabel(6,16);
             break;
        case PasswdC:
             showLabel(6,16);
             break;
        case Mail:
             showLabel();
             break;
    }
}
