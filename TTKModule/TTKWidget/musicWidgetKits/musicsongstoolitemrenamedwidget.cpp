#include "musicsongstoolitemrenamedwidget.h"
#include "musicstringutils.h"
#include "musicuiobject.h"
#include "musictoastlabel.h"
#include "musicapplication.h"

MusicSongsToolItemRenamedWidget::MusicSongsToolItemRenamedWidget(QWidget *parent)
    : QLineEdit(parent)
{
    setGeometry(1, 0, 330, 20);

    m_focusBlock = false;

    setStyleSheet(MusicUIObject::MLineEditStyle01);
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
    if(MusicUtils::String::illegalCharactersCheck(text))
    {
        backspace();
        m_focusBlock = true;

        MusicToastLabel *toast = new MusicToastLabel(this);
        toast->defaultLabel(MusicApplication::instance(), tr("Illegal Chars %1").arg(MusicUtils::String::illegalCharacters().join("")));
        connect(toast, SIGNAL(animationCloseChanged()), SLOT(animationCloseChanged()));
    }
}

void MusicSongsToolItemRenamedWidget::renameFinished()
{
    emit renameFinished(text());
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
        emit renameFinished(text());
        close();
    }
    else
    {
        Q_UNUSED(event);
    }
}

void MusicSongsToolItemRenamedWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    //do nothing just cover the origin
}
