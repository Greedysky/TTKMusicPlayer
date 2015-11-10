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
    m_tipsLabel = NULL;
    m_statusLabel = NULL;
}

void MusicUserLineEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    if(m_tipsLabel)
    {
        m_tipsLabel->show();
        m_tipsLabel->setStyleSheet(MusicUIObject::MCustomStyle13);
    }
}

void MusicUserLineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    checkTheInput();
}

void MusicUserLineEdit::setLabel(LabelType ty, QLabel *t, QLabel *s)
{
    m_tipsLabel = t;
    m_statusLabel = s;
    m_type = ty;
}

void MusicUserLineEdit::labelCheck(bool check)
{
    if(check)
    {
        if(m_tipsLabel) m_tipsLabel->hide();
        if(m_statusLabel) m_statusLabel->show();
    }
    else
    {
        if(m_statusLabel) m_statusLabel->hide();
        if(m_tipsLabel)
        {
            m_tipsLabel->show();
            m_tipsLabel->setStyleSheet(MusicUIObject::MCustomStyle14);
        }
    }
}

void MusicUserLineEdit::showLabel(int s, int e)
{
    int textLength = text().length();
    labelCheck(m_strContains = (s <= textLength && textLength <= e));
}

void MusicUserLineEdit::showLabel()
{
    QRegExp mailRx("^[0-9a-z][a-z0-9\._-]{1,}@[a-z0-9-]{1,}[a-z0-9]\.[a-z\.]{1,}[a-z]+$");
    labelCheck(m_mailContains = (text().contains(mailRx)));
}

void MusicUserLineEdit::checkTheInput()
{
    switch(m_type)
    {
        case User:
             showLabel(4, 20);
             break;
        case Passwd:
             checkPwdStrength();
        case PwdConfirm:
             showLabel(6, 16);
             break;
        case Mail:
             showLabel();
             break;
    }
}

void MusicUserLineEdit::checkPwdStrength()
{
    bool onlyNum, onlyChar, onlySp;
    onlyNum = text().contains(QRegExp("[0-9]"));
    onlyChar = text().contains(QRegExp("[a-zA-z]"));;
    onlySp = text().contains(QRegExp("[`~!@#$^&*()=|{}':;',\\[\\].<>/?~£¡@#"
                                     "£¤¡­¡­&*£¨£©&mdash;¡ª|{}¡¾¡¿¡®£»£º¡±¡°'¡££¬¡¢£¿]"));
    if( ( onlyNum && !onlyChar && !onlySp) ||
        (!onlyNum &&  onlyChar && !onlySp) ||
        (!onlyNum && !onlyChar &&  onlySp) ||
        (!onlyNum && !onlyChar && !onlySp) )
    {
        emit checkPwdStrength(0);
    }
    else if( ( onlyNum &&  onlyChar && !onlySp) ||
             ( onlyNum && !onlyChar &&  onlySp) ||
             (!onlyNum &&  onlyChar &&  onlySp) )
    {
        emit checkPwdStrength(1);
    }
    else
    {
        emit checkPwdStrength(2);
    }

}
