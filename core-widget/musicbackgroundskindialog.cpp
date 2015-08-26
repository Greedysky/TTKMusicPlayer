#include "musicbackgroundskindialog.h"
#include "musicobject.h"
#include "musicuiobject.h"
#include "ui_musicbackgroundskindialog.h"
#include <QFileDialog>
#include <QColorDialog>

MusicBackgroundSkinDialog::MusicBackgroundSkinDialog(QWidget *parent) :
    MusicMoveDialogAbstract(parent),
    ui(new Ui::MusicBackgroundSkinDialog)
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

    ui->themeListWidget->setFrameShape(QFrame::NoFrame);//Set No Border
    ui->themeListWidget->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->themeListWidget->setIconSize(QSize(100,80));
    ui->themeListWidget->setViewMode(QListView::IconMode);
    ui->themeListWidget->setMovement(QListView::Static);

    ui->showPerArea->setWordWrap(true);
    ui->bgTransparentSlider->setStyleSheet(MusicUIObject::MSliderStyle02);
    ui->bgTransparentSliderR->setStyleSheet(MusicUIObject::MSliderStyle02);
    ui->bgTransparentSliderR->setValue(100);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle01);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    ui->mySkin->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    ui->netSkin->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    ui->paletteButton->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    ui->customSkin->setStyleSheet(MusicUIObject::MPushButtonStyle01);

    this->addThemeListWidgetItem();

    connect(ui->bgTransparentSlider,SIGNAL(valueChanged(int)),parent,
                                SLOT(musicBgTransparentChanged(int)));
    connect(ui->themeListWidget,SIGNAL(currentTextChanged(QString)),parent,
                                SLOT(musicBackgroundSkinChanged(QString)));
    connect(ui->topTitleCloseButton,SIGNAL(clicked()),SLOT(close()));
    connect(ui->mySkin,SIGNAL(clicked()),SLOT(changeToMySkin()));
    connect(ui->netSkin,SIGNAL(clicked()),SLOT(changeToNetSkin()));
    connect(ui->paletteButton,SIGNAL(clicked()),SLOT(showPaletteDialog()));
    connect(ui->customSkin,SIGNAL(clicked()),SLOT(showCustomSkinDialog()));

}

MusicBackgroundSkinDialog::~MusicBackgroundSkinDialog()
{
    clearAllItems();
    delete ui;
}

void MusicBackgroundSkinDialog::clearAllItems()
{
    ui->themeListWidget->clear();
}

void MusicBackgroundSkinDialog::addThemeListWidgetItem()
{
    QList<QFileInfo> file(QDir(THEME_DOWNLOAD).entryInfoList());
    for(int i=2; i<file.count(); ++i)
    {
        QString fileName = file[i].fileName();
        QListWidgetItem *item = new QListWidgetItem(QIcon(QPixmap(file[i].filePath()).scaled(90,70))
             ,fileName.left(fileName.lastIndexOf('.')),ui->themeListWidget);
        item->setSizeHint(QSize(112,90));
        ui->themeListWidget->addItem(item);
    }
}

void MusicBackgroundSkinDialog::setCurrentBgTheme(const QString &theme, int alpha)
{
    //Set the current theme index
    for(int i=0; i<ui->themeListWidget->count(); ++i)
    {
        QListWidgetItem *item = ui->themeListWidget->item(i);
        if( item->text() == theme )
        {
            ui->themeListWidget->setCurrentItem(item);
            break;
        }
    }
    //Set the the slider bar value as what the alpha is
    ui->bgTransparentSlider->setValue(alpha);
}

void MusicBackgroundSkinDialog::changeToMySkin()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MusicBackgroundSkinDialog::changeToNetSkin()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MusicBackgroundSkinDialog::showPaletteDialog()
{
    QColor paletteColor = QColorDialog::getColor(Qt::white, this);
    if(!paletteColor.isValid())
    {
        return;
    }
    QImage image(16,16,QImage::Format_ARGB32);
    QString palettePath = QString("./MTheme/theme%1%2")
                          .arg(ui->themeListWidget->count() + 1).arg(".jpg");
    image.fill(paletteColor);
    if(image.save(palettePath))
    {
        //add item to listwidget
        QListWidgetItem *item = new QListWidgetItem(QIcon(QPixmap::fromImage(image).scaled(90,70)),
                    "theme" + QString::number(ui->themeListWidget->count()+1),ui->themeListWidget);
        item->setSizeHint(QSize(112,90));
        ui->themeListWidget->addItem(item);

        QFile file(palettePath);
        file.open(QIODevice::ReadOnly);
        file.rename(QString("./MTheme/theme%1%2")
                    .arg(ui->themeListWidget->count()).arg(SKN_FILE));
        file.close();
    }

}

void MusicBackgroundSkinDialog::showCustomSkinDialog()
{
    QString customSkinPath =  QFileDialog::getOpenFileName(
                      this,"","./",
                      "Images (*.png *.bmp *.jpg)");
    if(customSkinPath.isEmpty())
    {
        return;
    }
    QFile::copy(customSkinPath, QString("./MTheme/theme%1%2")
              .arg(ui->themeListWidget->count()+1).arg(SKN_FILE));
    //add item to listwidget
    QListWidgetItem *item = new QListWidgetItem(QIcon(QPixmap(customSkinPath).scaled(90,70)),
                "theme" + QString::number(ui->themeListWidget->count()+1),ui->themeListWidget);
    item->setSizeHint(QSize(112,90));
    ui->themeListWidget->addItem(item);
}
