#include "musicbackgroundskindialog.h"
#include "ui_musicbackgroundskindialog.h"
#include "musicbackgroundmanager.h"
#include "musicbackgroundpalettewidget.h"
#include "musicfunctionuiobject.h"
#include "musicbackgroundlistwidget.h"
#include "musicobject.h"
#include "musicapplicationobject.h"

#include <QScrollBar>
#include <QFileDialog>

MusicBackgroundSkinDialog::MusicBackgroundSkinDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicBackgroundSkinDialog)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    ui->mySkin->setStyleSheet(MusicUIObject::MLabelStyle02 + MusicUIObject::MLabelStyle03);
    ui->paletteButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->customSkin->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    m_backgroundList = new MusicBackgroundListWidget(this);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setFrameShape(QFrame::NoFrame);
    ui->scrollArea->setFrameShadow(QFrame::Plain);
    ui->scrollArea->setAlignment(Qt::AlignLeft);
    ui->scrollArea->setWidget(m_backgroundList);
    ui->scrollArea->verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle02);

    addThemeListWidgetItem();

    ui->resetWindowButton->setStyleSheet(MusicUIObject::MKGBtnResetWindow);
    ui->skinTransparentButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    ui->listTransparentButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);

    connect(ui->skinTransparentButton, SIGNAL(valueChanged(int)), parent,
                                       SLOT(musicBgTransparentChanged(int)));
    connect(ui->skinTransparentButton, SIGNAL(sliderStateChanged(bool)), parent,
                                       SLOT(musicBackgroundSliderStateChanged(bool)));
    connect(ui->listTransparentButton, SIGNAL(valueChanged(int)), parent,
                                       SLOT(musicPlayListTransparent(int)));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->paletteButton, SIGNAL(clicked()), SLOT(showPaletteDialog()));
    connect(ui->customSkin, SIGNAL(clicked()) ,SLOT(showCustomSkinDialog()));
    connect(m_backgroundList, SIGNAL(itemClicked(QString)), parent,
                  SLOT(musicBackgroundSkinChanged(QString)));
    connect(this, SIGNAL(currentColorChanged(QString)), parent,
                  SLOT(musicBgTransparentChanged(QString)));
    connect(ui->resetWindowButton, SIGNAL(clicked()), MusicApplicationObject::instance(),
                                   SLOT(musicResetWindow()));
}

MusicBackgroundSkinDialog::~MusicBackgroundSkinDialog()
{
    delete ui;
}

QString MusicBackgroundSkinDialog::getClassName()
{
    return staticMetaObject.className();
}

void MusicBackgroundSkinDialog::addThemeListWidgetItem()
{
    QList<QFileInfo> file(QDir(THEME_DIR_FULL)
                         .entryInfoList(QDir::Files | QDir::NoDotAndDotDot));
    foreach(const QFileInfo &info, file)
    {
        QString fileName = info.fileName();
        fileName.chop(4);
        m_backgroundList->createItem(fileName, info.filePath());
    }
}

void MusicBackgroundSkinDialog::setCurrentBgTheme(const QString &theme, int alpha, int listAlpha)
{
    m_backgroundList->setCurrentItemName(theme);
    //Set the the slider bar value as what the alpha is
    ui->skinTransparentButton->setValue(alpha);
    ui->listTransparentButton->setValue(listAlpha);
    setSkinTransToolText(alpha);
    setListTransToolText(listAlpha);
}

void MusicBackgroundSkinDialog::updateBackground(const QString &text)
{
    QPixmap pix(text);
    ui->background->setPixmap(pix.scaled( size() ));
}

int MusicBackgroundSkinDialog::getListBgSkinAlpha() const
{
    return ui->listTransparentButton->value();
}

void MusicBackgroundSkinDialog::setSkinTransToolText(int value)
{
    ui->skinTransparentButton->setText(QString("%1%").arg(value));
}

void MusicBackgroundSkinDialog::setListTransToolText(int value)
{
    ui->listTransparentButton->setText(QString("%1%").arg(value));
}

void MusicBackgroundSkinDialog::showPaletteDialog()
{
    MusicBackgroundPaletteWidget paletteWidget(this);
    connect(&paletteWidget, SIGNAL(currentColorToFileChanged(QString)),
                            SLOT(showPaletteDialog(QString)));
    connect(&paletteWidget, SIGNAL(currentColorToMemoryChanged(QString)),
                            SIGNAL(currentColorChanged(QString)));
    paletteWidget.exec();
}

void MusicBackgroundSkinDialog::showPaletteDialog(const QString &path)
{
    cpoyFileFromLocal( path );
    m_backgroundList->updateLastedItem();
    emit currentColorChanged( path );
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
    m_backgroundList->updateLastedItem();
}

void MusicBackgroundSkinDialog::cpoyFileFromLocal(const QString &path)
{
    QFile::copy(path, QString("%1theme-%2%3").arg(THEME_DIR_FULL)
                .arg(m_backgroundList->count() + 1).arg(SKN_FILE));
    //add item to listwidget
    m_backgroundList->createItem(QString("theme-%1").arg(m_backgroundList->count() + 1), path);
}

int MusicBackgroundSkinDialog::exec()
{
    updateBackground(M_BACKGROUND_PTR->getMBackground());
    return MusicAbstractMoveDialog::exec();
}
