#include "musicuserlineedit.h"
#include "musicuiobject.h"

#if defined(Q_CC_MSVC)
#   pragma warning(disable:4129)
#elif defined(Q_CC_GNU)
#   pragma GCC diagnostic ignored "-Wswitch"
#endif

MusicUserLineEdit::MusicUserLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    m_mailContains = false;
    m_strContains = false;
    m_tipsLabel = nullptr;
    m_statusLabel = nullptr;
}

void MusicUserLineEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    if(m_tipsLabel)
    {
        m_tipsLabel->show();
        m_tipsLabel->setStyleSheet(MusicUIObject::MColorStyle04);
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

    if(ty == PasswdNew)
    {
        connect(this, SIGNAL(textChanged(QString)), SLOT(checkPwdStrength()));
    }
}

void MusicUserLineEdit::labelCheck(bool check)
{
    if(check)
    {
        if(m_tipsLabel)
        {
            m_tipsLabel->hide();
        }

        if(m_statusLabel)
        {
            m_statusLabel->show();
        }
    }
    else
    {
        if(m_statusLabel)
        {
            m_statusLabel->hide();
        }

        if(m_tipsLabel)
        {
            m_tipsLabel->show();
            m_tipsLabel->setStyleSheet(MusicUIObject::MColorStyle05);
        }
    }
}

void MusicUserLineEdit::showLabel(int s, int e)
{
    const int textLength = text().length();
    labelCheck(m_strContains = (s <= textLength && textLength <= e));
}

void MusicUserLineEdit::showLabel()
{
    QRegExp mailRx("([0-9A-Za-z\\-_\\.]+)@([0-9a-z]+\\.[a-z]{2,3}(\\.[a-z]{2})?)");
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
        case PasswdConfirm:
             showLabel(6, 16);
             break;
        case Mail:
             showLabel();
             break;
        default: break;
    }
}

void MusicUserLineEdit::checkPwdStrength()
{
    if(text().isEmpty())
    {
        emit checkPwdStrength(-1);
        return;
    }

    bool onlyNum, onlyChar, onlySp;
    onlyNum = text().contains(QRegExp("[0-9]"));
    onlyChar = text().contains(QRegExp("[a-zA-z]"));
    onlySp = text().contains(QRegExp("[`~!@#$^&*()=|{}':;',\\[\\].<>/?~@#]"));

    if((onlyNum && !onlyChar && !onlySp) || (!onlyNum &&  onlyChar && !onlySp) || (!onlyNum && !onlyChar &&  onlySp) || (!onlyNum && !onlyChar && !onlySp))
    {
        emit checkPwdStrength(0);
    }
    else if((onlyNum &&  onlyChar && !onlySp) || (onlyNum && !onlyChar &&  onlySp) || (!onlyNum &&  onlyChar &&  onlySp))
    {
        emit checkPwdStrength(1);
    }
    else
    {
        emit checkPwdStrength(2);
    }

}
