#include "musicbackgroundskindialog.h"
#include "musicobject.h"
#include "musicuiobject.h"
#include "ui_musicbackgroundskindialog.h"
#include "musicbgthememanager.h"

#include <QFileDialog>
#include <QColorDialog>

MusicBackgroundSkinDialog::MusicBackgroundSkinDialog(QWidget *parent) :
    MusicAbstractMoveDialog(parent),
    ui(new Ui::MusicBackgroundSkinDialog)
{
    ui->setupUi(this);
    drawWindowRoundedRect(this);
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

    addThemeListWidgetItem();

    connect(ui->bgTransparentSlider, SIGNAL(valueChanged(int)), parent,
                                     SLOT(musicBgTransparentChanged(int)));
    connect(ui->bgTransparentSliderR, SIGNAL(valueChanged(int)), parent,
                                      SIGNAL(setTransparent(int)));
    connect(ui->themeListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
                                 SLOT(itemUserClicked(QListWidgetItem*)));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->mySkin, SIGNAL(clicked()), SLOT(changeToMySkin()));
    connect(ui->netSkin, SIGNAL(clicked()), SLOT(changeToNetSkin()));
    connect(ui->paletteButton, SIGNAL(clicked()), SLOT(showPaletteDialog()));
    connect(ui->customSkin, SIGNAL(clicked()) ,SLOT(showCustomSkinDialog()));
    connect(this, SIGNAL(currentTextChanged(QString)), parent,
                  SLOT(musicBackgroundSkinChanged(QString)));
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
        fileName.chop(4);
        createItem(fileName, QIcon(QPixmap(file[i].filePath()).scaled(90,70)));
    }
}

void MusicBackgroundSkinDialog::createItem(const QString &name, const QIcon &icon)
{
    QListWidgetItem *item = new QListWidgetItem(ui->themeListWidget);
    item->setIcon(icon);
    item->setData(MUSIC_BG_ROLE, name);
    item->setToolTip(name);
    ui->themeListWidget->addItem(item);
}

void MusicBackgroundSkinDialog::setCurrentBgTheme(const QString &theme, int alpha, int alphaR)
{
    //Set the current theme index
    for(int i=0; i<ui->themeListWidget->count(); ++i)
    {
        QListWidgetItem *item = ui->themeListWidget->item(i);
        if(item && item->data(MUSIC_BG_ROLE) == theme )
        {
            ui->themeListWidget->setCurrentItem(item);
            break;
        }
    }
    //Set the the slider bar value as what the alpha is
    ui->bgTransparentSlider->setValue(alpha);
    ui->bgTransparentSliderR->setValue(alphaR);
}

void MusicBackgroundSkinDialog::updateBackground()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
}

int MusicBackgroundSkinDialog::getListBgSkinAlpha() const
{
    return ui->bgTransparentSliderR->value();
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
    QImage image(16, 16, QImage::Format_ARGB32);
    QString palettePath = QString("%1theme-%2%3").arg(THEME_DOWNLOAD)
                          .arg(ui->themeListWidget->count() + 1).arg(JPG_FILE);
    image.fill(paletteColor);
    if(image.save(palettePath))
    {
        //add item to listwidget
        createItem(QString("theme-%1").arg(ui->themeListWidget->count() + 1),
                   QIcon(QPixmap::fromImage(image).scaled(90, 70)));

        QFile file(palettePath);
        file.open(QIODevice::ReadOnly);
        file.rename(QString("%1theme-%2%3").arg(THEME_DOWNLOAD)
                    .arg(ui->themeListWidget->count()).arg(SKN_FILE));
        file.close();
    }

}

void MusicBackgroundSkinDialog::showCustomSkinDialog()
{
    QString customSkinPath =  QFileDialog::getOpenFileName(
                              this, "", "./", "Images (*.png *.bmp *.jpg)");
    if(customSkinPath.isEmpty())
    {
        return;
    }
    QFile::copy(customSkinPath, QString("%1theme-%2%3").arg(THEME_DOWNLOAD)
                  .arg(ui->themeListWidget->count()+1).arg(SKN_FILE));
    //add item to listwidget
    createItem(QString("theme-%1").arg(ui->themeListWidget->count() + 1),
               QIcon(QPixmap(customSkinPath).scaled(90,70)));
}

void MusicBackgroundSkinDialog::itemUserClicked(QListWidgetItem *item)
{
    if( item )
    {
        emit currentTextChanged( item->data(MUSIC_BG_ROLE).toString() );
    }
}

int MusicBackgroundSkinDialog::exec()
{
    updateBackground();
    return MusicAbstractMoveDialog::exec();
}
