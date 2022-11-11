#include "musictoastlabel.h"
#include "musicapplication.h"

MusicToastLabel::MusicToastLabel(QWidget *parent)
    : TTKToastLabel(parent)
{

}

MusicToastLabel::MusicToastLabel(const QString &text, QWidget *parent)
    : TTKToastLabel(text, parent)
{

}

void MusicToastLabel::popup(const QString &text)
{
    TTKToastLabel *label = new MusicToastLabel(MusicApplication::instance());
    label->setText(text);
    label->popup();
}
