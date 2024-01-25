#include "musicitemrenameedit.h"
#include "musicstringutils.h"
#include "musicuiobject.h"
#include "musictoastlabel.h"

MusicItemRenameEidt::MusicItemRenameEidt(QWidget *parent)
    : QLineEdit(parent),
      m_focusBlock(false)
{
    setGeometry(1, 0, 330, 20);
    setStyleSheet(TTK::UI::LineEditStyle01);
    setFocus(Qt::MouseFocusReason);
    setFocusPolicy(Qt::ClickFocus);

    connect(this, SIGNAL(textChanged(QString)), SLOT(textChanged(QString)));
    connect(this, SIGNAL(editingFinished()), SLOT(renameFinished()));
}

MusicItemRenameEidt::MusicItemRenameEidt(const QString &text, QWidget *parent)
    : MusicItemRenameEidt(parent)
{
    setText(text);
}

void MusicItemRenameEidt::textChanged(const QString &text)
{
    if(TTK::String::isCharValid(text))
    {
        backspace();
        m_focusBlock = true;

        MusicToastLabel *toast = new MusicToastLabel(this);
        connect(toast, SIGNAL(finished()), SLOT(animationCloseChanged()));

        toast->setText(tr("Illegal chars %1").arg(TTK::String::characters().join("")));
        toast->popup();
    }
}

void MusicItemRenameEidt::renameFinished()
{
    Q_EMIT renameFinished(text());
}

void MusicItemRenameEidt::animationCloseChanged()
{
    m_focusBlock = false;
    setFocus(Qt::MouseFocusReason);
}

void MusicItemRenameEidt::focusOutEvent(QFocusEvent *event)
{
    if(!m_focusBlock)
    {
        QLineEdit::focusOutEvent(event);
        Q_EMIT renameFinished(text());
        close();
    }
    else
    {
        Q_UNUSED(event);
    }
}
