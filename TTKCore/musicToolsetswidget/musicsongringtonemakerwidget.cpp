#include "musicsongringtonemakerwidget.h"
#include "ui_musicsongringtonemakerwidget.h"
#include "musicbackgroundmanager.h"
#include "musicuiobject.h"

MusicSongRingtoneMaker::MusicSongRingtoneMaker(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicSongRingtoneMaker)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
}

MusicSongRingtoneMaker::~MusicSongRingtoneMaker()
{
    delete ui;
}

int MusicSongRingtoneMaker::exec()
{
    QPixmap pix(M_BACKGROUND_PTR->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}

