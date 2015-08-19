#include "musiclrcartphotoupload.h"
#include "ui_musiclrcartphotoupload.h"
#include "musicuiobject.h"
#include "musicobject.h"
#include <QFileDialog>
#include <QMessageBox>

MusicLrcArtPhotoUpload::MusicLrcArtPhotoUpload(QWidget *parent)
    : MusicMoveDialogAbstract(parent),
      ui(new Ui::MusicLrcArtPhotoUpload)
{
    ui->setupUi(this);
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),4,4);
    setMask(bmp);
    //set window radius
    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle01);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));

    ui->artSearchEdit->setStyleSheet(MusicUIObject::MLineEditStyle02);
    ui->uploadButton->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    ui->closeButton->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    ui->selectButton->setStyleSheet(MusicUIObject::MPushButtonStyle01);

    ui->uploadButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->closeButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->selectButton->setCursor(QCursor(Qt::PointingHandCursor));

    ui->stateLabel->setStyleSheet(MusicUIObject::MCustomStyle07);

    ui->uploadButton->hide();
    ui->closeButton->hide();
    ui->stateLabel->hide();

    connect(ui->topTitleCloseButton,SIGNAL(clicked()),SLOT(close()));
    connect(ui->selectButton,SIGNAL(clicked()),SLOT(selectButtonClicked()));
    connect(ui->closeButton,SIGNAL(clicked()),SLOT(close()));
    connect(ui->uploadButton,SIGNAL(clicked()),SLOT(uploadButtonClicked()));
}

void MusicLrcArtPhotoUpload::selectButtonClicked()
{
    QString picPath = QFileDialog::getOpenFileName(
                      this, "", "./", "Images (*.png *.bmp *.jpg)");
    if(picPath.isEmpty())
        return;

    QPixmap pix(picPath);
    if(pix.width() < 1001 || pix.height() < 669)
    {
        ui->stateLabel->show();
        ui->uploadButton->hide();
        ui->closeButton->hide();
    }
    else
    {
        ui->stateLabel->hide();
        ui->uploadButton->show();
        ui->closeButton->show();
        ui->introduceLabel->hide();
        ui->selectButton->hide();
        ui->imageLabel->setImagePath(picPath);
    }
}

void  MusicLrcArtPhotoUpload::uploadButtonClicked()
{
    QDir bgDir(ART_BG);
    int count = 0;
    QString name = ui->artSearchEdit->text().trimmed();
    if(name.isEmpty())
    {
        QMessageBox::warning(this, tr("QMusicPlayer"), tr("the art is empty!"));
        return;
    }

    foreach(QFileInfo f, bgDir.entryInfoList())
    {
        if(f.fileName().contains( name ))
        {
            ++count;
            qDebug()<<f.fileName();
        }
    }

    QString fileName = QString("%1%2%3").arg(ART_BG).arg(name).arg(count);
    ui->imageLabel->saveImagePath(fileName + JPG_FILE);
    QFile::rename(fileName + JPG_FILE, fileName + SKN_FILE );
    close();
}
