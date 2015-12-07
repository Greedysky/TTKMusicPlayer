#include "musicsourceupdatewidget.h"
#include "ui_musicsourceupdatewidget.h"
#include "musicbgthememanager.h"
#include "musicuiobject.h"

MusicSourceUpdateWidget::MusicSourceUpdateWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicSourceUpdateWidget)
{
    ui->setupUi(this);
    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));

    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->upgradeButton, SIGNAL(clicked()), SLOT(upgradeButtonClicked()));
}

MusicSourceUpdateWidget::~MusicSourceUpdateWidget()
{
    delete ui;
}

int MusicSourceUpdateWidget::exec()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}

void MusicSourceUpdateWidget::upgradeButtonClicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
