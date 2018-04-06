#include "musicsongstoolitemrenamedwidget.h"
#include "musicstringutils.h"
#include "musicuiobject.h"
#include "musictoastlabel.h"

MusicSongsToolItemRenamedWidget::MusicSongsToolItemRenamedWidget(const QString &text, QWidget *parent)
    : QLineEdit(parent)
{
    setGeometry(1, 0, 330, 20);

    m_focusBlock = false;

    setText(text);
    setStyleSheet(MusicUIObject::MLineEditStyle01);
    setFocus(Qt::MouseFocusReason);
    setFocusPolicy(Qt::ClickFocus);

    connect(this, SIGNAL(textChanged(QString)), SLOT(textChanged(QString)));
    connect(this, SIGNAL(editingFinished()), SLOT(renameFinished()));

}

QString MusicSongsToolItemRenamedWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongsToolItemRenamedWidget::textChanged(const QString &text)
{
    if(MusicUtils::String::illegalCharactersCheck(text))
    {
        backspace();
        m_focusBlock = true;

        MusicToastLabel *toast = new MusicToastLabel(this);
        toast->setFontSize(15);
        toast->setFontMargin(20, 20);
        toast->setText(tr("Illegal Chars %1").arg(MusicUtils::String::illegalCharacters().join("")));
        toast->popup(this);
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
