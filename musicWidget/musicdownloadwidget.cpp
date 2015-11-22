#include "musicdownloadwidget.h"
#include "ui_musicdownloadwidget.h"
#include "musicbgthememanager.h"
#include "musicuiobject.h"

MusicDownloadWidget::MusicDownloadWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicDownloadWidget)
{
    ui->setupUi(this);
    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

}

MusicDownloadWidget::~MusicDownloadWidget()
{
    delete ui;
}

int MusicDownloadWidget::exec()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}
