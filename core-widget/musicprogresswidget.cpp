#include "musicprogresswidget.h"
#include "musicbgthememanager.h"
#include "musicuiobject.h"

MusicProgressWidget::MusicProgressWidget(QWidget *parent)
    : QProgressDialog(parent)
{
    setModal(true);
    setWindowFlags( windowFlags() | Qt::WindowStaysOnTopHint);
    setWindowModality(Qt::WindowModal);

//    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
//    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
//    ui->topTitleCloseButton->setEnabled(false);

//    ui->progressBar->setStyleSheet(MusicUIObject::MProgressBar01);
}

MusicProgressWidget::~MusicProgressWidget()
{

}

//void MusicProgressWidget::setTitle(const QString &name)
//{
////    ui->textLabel->setText(name);
//}

//void MusicProgressWidget::setValue(int value)
//{
////    ui->progressBar->setValue(value);
//}

//void MusicProgressWidget::setRange(int min, int max)
//{
////    ui->progressBar->setRange(min, max);
//}

int MusicProgressWidget::exec()
{
//    QPixmap pix(M_BG_MANAGER->getMBackground());
//    ui->background->setPixmap(pix.scaled( size() ));
    return QProgressDialog::exec();
}
