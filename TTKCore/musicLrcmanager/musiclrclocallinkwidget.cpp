#include "musiclrclocallinkwidget.h"
#include "ui_musiclrclocallinkwidget.h"
#include "musicbackgroundmanager.h"
#include "musicuiobject.h"

MusicLrcLocalLinkWidget::MusicLrcLocalLinkWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicLrcLocalLinkWidget)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->fuzzyButton->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    ui->localSearchButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->commitButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->previewButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->titleEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
}

MusicLrcLocalLinkWidget::~MusicLrcLocalLinkWidget()
{
    delete ui;
}

void MusicLrcLocalLinkWidget::setCurrentSongName(const QString& name)
{
    ui->titleEdit->setText(name);
}

int MusicLrcLocalLinkWidget::exec()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}
