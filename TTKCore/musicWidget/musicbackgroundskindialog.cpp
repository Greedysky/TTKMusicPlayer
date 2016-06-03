#include "musicbackgroundskindialog.h"
#include "ui_musicbackgroundskindialog.h"
#include "musicobject.h"
#include "musicbackgroundmanager.h"
#include "musicutils.h"
#include "musicslidermenuwidget.h"

#include <QFileDialog>
#include <QColorDialog>

MusicBackgroundSkinDialog::MusicBackgroundSkinDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicBackgroundSkinDialog)
{
    ui->setupUi(this);

    m_listTransMenu = new MusicSliderMenuWidget(this);
    m_skinTransMenu = new MusicSliderMenuWidget(this);
    ui->listTransparentButton->setPopupMode(QToolButton::InstantPopup);
    ui->listTransparentButton->setMenu(m_listTransMenu);
    ui->listTransparentButton->setStyleSheet(MusicUIObject::MToolButtonStyle10);
    ui->skinTransparentButton->setPopupMode(QToolButton::InstantPopup);
    ui->skinTransparentButton->setMenu(m_skinTransMenu);
    ui->skinTransparentButton->setStyleSheet(MusicUIObject::MToolButtonStyle10);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    ui->mySkin->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->netSkin->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->paletteButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->customSkin->setStyleSheet(MusicUIObject::MPushButtonStyle08);
//#ifdef Q_OS_UNIX
//    ui->showPerArea->setText("100%\n\n\n\n\n\n\n\n\n\n 50%\n\n\n\n\n\n\n\n\n\n 0%");
//    MusicUtils::setLabelFont(ui->showPerArea, 7);
//#endif
    addThemeListWidgetItem();

    connect(m_skinTransMenu, SIGNAL(valueChanged(int)), parent,
                             SLOT(musicBgTransparentChanged(int)));
    connect(m_listTransMenu, SIGNAL(valueChanged(int)), parent,
                             SLOT(musicPlayListTransparent(int)));
    connect(ui->themeListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
                                 SLOT(itemUserClicked(QListWidgetItem*)));
    connect(ui->remoteWidget, SIGNAL(showCustomSkin(QString)), SLOT(showCustomSkin(QString)));
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
    delete m_listTransMenu;
    delete m_skinTransMenu;
    delete ui;
}

void MusicBackgroundSkinDialog::addThemeListWidgetItem()
{
    QList<QFileInfo> file(QDir(THEME_DIR_FULL)
                         .entryInfoList(QDir::Files | QDir::NoDotAndDotDot));
    foreach(QFileInfo info, file)
    {
        QString fileName = info.fileName();
        fileName.chop(4);
        ui->themeListWidget->createItem(fileName, info.filePath() );
    }
}

void MusicBackgroundSkinDialog::setCurrentBgTheme(const QString &theme, int alpha, int listAlpha)
{
    ui->themeListWidget->setCurrentItemName(theme);
    //Set the the slider bar value as what the alpha is
    m_skinTransMenu->setValue(alpha);
    m_listTransMenu->setValue(listAlpha);
    setSkinTransToolText(alpha);
    setListTransToolText(listAlpha);
}

void MusicBackgroundSkinDialog::updateBackground()
{
    QPixmap pix(M_BACKGROUND_PTR->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
}

int MusicBackgroundSkinDialog::getListBgSkinAlpha() const
{
    return m_listTransMenu->getValue();
}

void MusicBackgroundSkinDialog::setSkinTransToolText(int value)
{
    ui->skinTransparentButton->setText(QString("%1%").arg(value));
}

void MusicBackgroundSkinDialog::setListTransToolText(int value)
{
    ui->listTransparentButton->setText(QString("%1%").arg(value));
}

void MusicBackgroundSkinDialog::changeToMySkin()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MusicBackgroundSkinDialog::changeToNetSkin()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MusicBackgroundSkinDialog::showPaletteDialog()
{
    QColor paletteColor = QColorDialog::getColor(Qt::white, this);
    if(!paletteColor.isValid())
    {
        return;
    }
    QImage image(16, 16, QImage::Format_ARGB32);
    QString palettePath = QString("%1theme-%2%3").arg(THEME_DIR_FULL)
                          .arg(ui->themeListWidget->count() + 1).arg(JPG_FILE);
    image.fill(paletteColor);
    if(image.save(palettePath))
    {
        ui->themeListWidget->createItem(QString("theme-%1")
                   .arg(ui->themeListWidget->count() + 1),
                   QIcon(QPixmap::fromImage(image).scaled(90, 70)));

        QFile file(palettePath);
        file.open(QIODevice::ReadOnly);
        file.rename(QString("%1theme-%2%3").arg(THEME_DIR_FULL)
                    .arg(ui->themeListWidget->count()).arg(SKN_FILE));
        file.close();
    }

}

void MusicBackgroundSkinDialog::showCustomSkinDialog()
{
    QString customSkinPath =  QFileDialog::getOpenFileName(
                              this, QString(), "./", "Images (*.png *.bmp *.jpg)");
    if(customSkinPath.isEmpty())
    {
        return;
    }
    cpoyFileFromLocal( customSkinPath );
}

void MusicBackgroundSkinDialog::cpoyFileFromLocal(const QString &path)
{
    QFile::copy(path, QString("%1theme-%2%3").arg(THEME_DIR_FULL)
                .arg(ui->themeListWidget->count()+1).arg(SKN_FILE));
    //add item to listwidget
    ui->themeListWidget->createItem(QString("theme-%1")
                        .arg(ui->themeListWidget->count() + 1), path);
}

void MusicBackgroundSkinDialog::showCustomSkin(const QString &path)
{
    if( !ui->themeListWidget->contains(path) )
    {
        cpoyFileFromLocal(path);
    }
    itemUserClicked( ui->themeListWidget->item(ui->themeListWidget->indexOf(path)) );
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
