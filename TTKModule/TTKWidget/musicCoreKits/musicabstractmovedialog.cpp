#include "musicabstractmovedialog.h"
#include "musicbackgroundmanager.h"

MusicAbstractMoveDialog::MusicAbstractMoveDialog(QWidget *parent)
    : MusicAbstractMoveDialog(true, parent)
{

}

MusicAbstractMoveDialog::MusicAbstractMoveDialog(bool transparent, QWidget *parent)
    : TTKAbstractMoveDialog(transparent, parent),
      MusicWidgetRenderer()
{
    G_BACKGROUND_PTR->addObserver(this);
}

MusicAbstractMoveDialog::~MusicAbstractMoveDialog()
{
    G_BACKGROUND_PTR->removeObserver(this);
}

void MusicAbstractMoveDialog::backgroundChanged()
{
    setBackgroundPixmap(size());
}

int MusicAbstractMoveDialog::exec()
{
    backgroundChanged();
    return QDialog::exec();
}

void MusicAbstractMoveDialog::show()
{
    backgroundChanged();
    QDialog::show();
}
