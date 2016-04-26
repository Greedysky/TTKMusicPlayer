#include "musiclrclocallinkwidget.h"
#include "ui_musiclrclocallinkwidget.h"

MusicLrcLocalLinkWidget::MusicLrcLocalLinkWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicLrcLocalLinkWidget)
{
    ui->setupUi(this);
}

MusicLrcLocalLinkWidget::~MusicLrcLocalLinkWidget()
{
    delete ui;
}
