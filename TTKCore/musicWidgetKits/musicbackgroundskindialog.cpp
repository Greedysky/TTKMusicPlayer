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
      m_ui(new Ui::MusicBackgroundSkinDialog)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    m_ui->mySkin->setStyleSheet(MusicUIObject::MLabelStyle02 + MusicUIObject::MLabelStyle03);
    m_ui->paletteButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->customSkin->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    m_backgroundList = new MusicBackgroundListWidget(this);
    m_ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->scrollArea->setWidgetResizable(true);
    m_ui->scrollArea->setFrameShape(QFrame::NoFrame);
    m_ui->scrollArea->setFrameShadow(QFrame::Plain);
    m_ui->scrollArea->setAlignment(Qt::AlignVCenter);
    m_ui->scrollArea->setWidget(m_backgroundList);
    m_ui->scrollArea->verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle02);

    addThemeListWidgetItem();

    m_ui->resetWindowButton->setStyleSheet(MusicUIObject::MKGBtnResetWindow);
    m_ui->skinTransparentButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);
    m_ui->listTransparentButton->setStyleSheet(MusicUIObject::MToolButtonStyle05);

    connect(m_ui->skinTransparentButton, SIGNAL(valueChanged(int)), parent,
                                       SLOT(musicBgTransparentChanged(int)));
    connect(m_ui->skinTransparentButton, SIGNAL(sliderStateChanged(bool)), parent,
                                       SLOT(musicBackgroundSliderStateChanged(bool)));
    connect(m_ui->listTransparentButton, SIGNAL(valueChanged(int)), parent,
                                       SLOT(musicPlayListTransparent(int)));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->paletteButton, SIGNAL(clicked()), SLOT(showPaletteDialog()));
    connect(m_ui->customSkin, SIGNAL(clicked()) ,SLOT(showCustomSkinDialog()));
    connect(m_backgroundList, SIGNAL(itemClicked(QString)), parent,
                  SLOT(musicBackgroundSkinChanged(QString)));
    connect(this, SIGNAL(currentColorChanged(QString)), parent,
                  SLOT(musicBgTransparentChanged(QString)));
    connect(m_ui->resetWindowButton, SIGNAL(clicked()), MusicApplicationObject::instance(),
                                   SLOT(musicResetWindow()));
}

MusicBackgroundSkinDialog::~MusicBackgroundSkinDialog()
{
    delete m_ui;
}

QString MusicBackgroundSkinDialog::getClassName()
{
    return staticMetaObject.className();
}

void MusicBackgroundSkinDialog::addThemeListWidgetItem()
{
    QList<QFileInfo> file(QDir(THEME_DIR_FULL)
                         .entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name));
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
    m_ui->skinTransparentButton->setValue(alpha);
    m_ui->listTransparentButton->setValue(listAlpha);
    setSkinTransToolText(alpha);
    setListTransToolText(listAlpha);
}

void MusicBackgroundSkinDialog::updateBackground(const QString &text)
{
    QPixmap pix(text);
    m_ui->background->setPixmap(pix.scaled( size() ));
}

int MusicBackgroundSkinDialog::getListBgSkinAlpha() const
{
    return m_ui->listTransparentButton->value();
}

void MusicBackgroundSkinDialog::setSkinTransToolText(int value)
{
    m_ui->skinTransparentButton->setText(QString("%1%").arg(value));
}

void MusicBackgroundSkinDialog::setListTransToolText(int value)
{
    m_ui->listTransparentButton->setText(QString("%1%").arg(value));
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
