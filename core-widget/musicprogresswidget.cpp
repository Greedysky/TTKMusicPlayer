#include "musicprogresswidget.h"
#include "ui_musicprogresswidget.h"
#include "musicbgthememanager.h"
#include "musicuiobject.h"

MusicProgressWidget::MusicProgressWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicProgressWidget)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setEnabled(false);

    ui->progressBar->setStyleSheet(MusicUIObject::MProgressBar01);
}

MusicProgressWidget::~MusicProgressWidget()
{
    delete ui;
}

void MusicProgressWidget::setTitle(const QString &name)
{
    ui->textLabel->setText(name);
}

void MusicProgressWidget::setValue(int value)
{
    ui->progressBar->setValue(value);
}

void MusicProgressWidget::setRange(int min, int max)
{
    ui->progressBar->setRange(min, max);
}

int MusicProgressWidget::exec()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}
