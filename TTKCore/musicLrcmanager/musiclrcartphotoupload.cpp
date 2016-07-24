#include "musiclrcartphotoupload.h"
#include "ui_musiclrcartphotoupload.h"
#include "musicuiobject.h"
#include "musicobject.h"
#include "musicbackgroundmanager.h"
#include "musicmessagebox.h"

#include <QFileDialog>

MusicLrcArtPhotoUpload::MusicLrcArtPhotoUpload(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicLrcArtPhotoUpload)
{
    ui->setupUi(this);
    
    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));

    ui->artSearchEdit->setStyleSheet(MusicUIObject::MLineEditStyle01);
    ui->uploadButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->closeButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->selectButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);

    ui->uploadButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->closeButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->selectButton->setCursor(QCursor(Qt::PointingHandCursor));

    ui->stateLabel->setStyleSheet(MusicUIObject::MCustomStyle08);

    ui->uploadButton->hide();
    ui->closeButton->hide();
    ui->stateLabel->hide();

    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->selectButton, SIGNAL(clicked()), SLOT(selectButtonClicked()));
    connect(ui->closeButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->uploadButton, SIGNAL(clicked()), SLOT(uploadButtonClicked()));
}

MusicLrcArtPhotoUpload::~MusicLrcArtPhotoUpload()
{
    delete ui;
}

QString MusicLrcArtPhotoUpload::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcArtPhotoUpload::selectButtonClicked()
{
    QString picPath = QFileDialog::getOpenFileName(
                      this, QString(), "./", "Images (*.png *.bmp *.jpg)");
    if(picPath.isEmpty())
    {
        return;
    }

    QPixmap pix(picPath);
    if(pix.width() < WINDOW_WIDTH_MIN || pix.height() < WINDOW_HEIGHT_MIN)
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
    QDir bgDir(BACKGROUND_DIR_FULL);
    int count = 0;
    QString name = ui->artSearchEdit->text().trimmed();
    if(name.isEmpty())
    {
        MusicMessageBox message;
        message.setText(tr("the art is empty!"));
        message.exec();
        return;
    }

    foreach(QFileInfo f, bgDir.entryInfoList())
    {
        if(f.fileName().contains( name ))
        {
            ++count;
        }
    }

    QString fileName = QString("%1%2%3").arg(BACKGROUND_DIR_FULL).arg(name).arg(count);
    ui->imageLabel->saveImagePath(fileName + JPG_FILE);
    QFile::rename(fileName + JPG_FILE, fileName + SKN_FILE );
    close();
}

int MusicLrcArtPhotoUpload::exec()
{
    QPixmap pix(M_BACKGROUND_PTR->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}
