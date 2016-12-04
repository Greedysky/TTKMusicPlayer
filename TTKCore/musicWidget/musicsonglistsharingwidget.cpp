#include "musicsonglistsharingwidget.h"
#include "ui_musicsonglistsharingwidget.h"
#include "musicuiobject.h"

MusicSongListSharingWidget::MusicSongListSharingWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicSongListSharingWidget)
{
    ui->setupUi(this);
    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->label->setStyleSheet(MusicUIObject::MBackgroundStyle01 + MusicUIObject::MFontStyle05 + \
                             MusicUIObject::MFontStyle01 + MusicUIObject::MColorStyle03);
    ui->outputButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->inputButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    ui->outputButton->setIcon(QIcon(":/toolSets/btn_kukou_output"));
    ui->inputButton->setIcon(QIcon(":/toolSets/btn_kukou_input"));
}

MusicSongListSharingWidget::~MusicSongListSharingWidget()
{
    delete ui;
}

QString MusicSongListSharingWidget::getClassName()
{
    return staticMetaObject.className();
}

int MusicSongListSharingWidget::exec()
{
    setBackgroundPixmap(ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
