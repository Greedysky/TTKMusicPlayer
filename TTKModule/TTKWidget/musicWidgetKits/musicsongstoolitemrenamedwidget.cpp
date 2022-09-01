#include "musicsongstoolitemrenamedwidget.h"
#include "musicstringutils.h"
#include "musicuiobject.h"
#include "musictoastlabel.h"

MusicSongsToolItemRenamedWidget::MusicSongsToolItemRenamedWidget(QWidget *parent)
    : QLineEdit(parent),
      m_focusBlock(false)
{
    setGeometry(1, 0, 330, 20);
    setStyleSheet(MusicUIObject::MQSSLineEditStyle01);
    setFocus(Qt::MouseFocusReason);
    setFocusPolicy(Qt::ClickFocus);

    connect(this, SIGNAL(textChanged(QString)), SLOT(textChanged(QString)));
    connect(this, SIGNAL(editingFinished()), SLOT(renameFinished()));

}

MusicSongsToolItemRenamedWidget::MusicSongsToolItemRenamedWidget(const QString &text, QWidget *parent)
    : MusicSongsToolItemRenamedWidget(parent)
{
    setText(text);
}

void MusicSongsToolItemRenamedWidget::textChanged(const QString &text)
{
    if(MusicUtils::String::isCharacterValid(text))
    {
        backspace();
        m_focusBlock = true;

        MusicToastLabel *toast = new MusicToastLabel(this);
        toast->setText(tr("Illegal chars %1").arg(MusicUtils::String::illegalCharacters().join("")));
        toast->popup();
        connect(toast, SIGNAL(animationCloseChanged()), SLOT(animationCloseChanged()));
    }
}

void MusicSongsToolItemRenamedWidget::renameFinished()
{
    Q_EMIT renameFinished(text());
}

void MusicSongsToolItemRenamedWidget::animationCloseChanged()
{
    m_focusBlock = false;
    setFocus(Qt::MouseFocusReason);
}

void MusicSongsToolItemRenamedWidget::focusOutEvent(QFocusEvent *event)
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
