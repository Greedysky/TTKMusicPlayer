#include "musicbackgroundskindialog.h"
#include "ui_musicbackgroundskindialog.h"
#include "musicbackgroundmanager.h"
#include "musicbackgroundpalettewidget.h"
#include "musicfunctionuiobject.h"
#include "musicbackgroundlistwidget.h"
#include "musicobject.h"
#include "musictopareawidget.h"
#include "musicapplicationobject.h"

#include <QScrollBar>
#include <QFileDialog>
#include <QSignalMapper>
#include <QDebug>

MusicBackgroundSkinDialog::MusicBackgroundSkinDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicBackgroundSkinDialog)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    m_ui->recommendSkin->setStyleSheet(MusicUIObject::MColorStyle08);
    m_ui->mySkin->setStyleSheet(MusicUIObject::MColorStyle03);
    m_ui->paletteButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->customSkin->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->stackedWidget->setLength(m_ui->stackedWidget->width(), MusicAnimationStackedWidget::RightToLeft);

    QSignalMapper *group = new QSignalMapper(this);
    group->setMapping(m_ui->recommendSkin, 0);
    group->setMapping(m_ui->mySkin, 1);
    connect(m_ui->recommendSkin, SIGNAL(clicked()), group, SLOT(map()));
    connect(m_ui->mySkin, SIGNAL(clicked()), group, SLOT(map()));
    connect(group, SIGNAL(mapped(int)), SLOT(backgroundListWidgetChanged(int)));

    //////////////////////////////////////////////////////
    m_backgroundList = new MusicBackgroundListWidget(this);
    m_ui->recommandScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->recommandScrollArea->setWidgetResizable(true);
    m_ui->recommandScrollArea->setFrameShape(QFrame::NoFrame);
    m_ui->recommandScrollArea->setFrameShadow(QFrame::Plain);
    m_ui->recommandScrollArea->setAlignment(Qt::AlignVCenter);
    m_ui->recommandScrollArea->setWidget(m_backgroundList);
    m_ui->recommandScrollArea->verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle02);

    m_myBackgroundList = new MusicBackgroundListWidget(this);
    m_ui->userScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->userScrollArea->setWidgetResizable(true);
    m_ui->userScrollArea->setFrameShape(QFrame::NoFrame);
    m_ui->userScrollArea->setFrameShadow(QFrame::Plain);
    m_ui->userScrollArea->setAlignment(Qt::AlignVCenter);
    m_ui->userScrollArea->setWidget(m_myBackgroundList);
    m_ui->userScrollArea->verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle02);

    m_myThemeIndex = CURRENT_ITEMS_COUNT;
    addThemeListWidgetItem();
    //////////////////////////////////////////////////////
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
    connect(m_backgroundList, SIGNAL(itemClicked(QString)), SLOT(backgroundListWidgetItemClicked(QString)));
    connect(m_myBackgroundList, SIGNAL(itemClicked(QString)), SLOT(myBackgroundListWidgetItemClicked(QString)));
    connect(this, SIGNAL(currentColorChanged(QString)), MusicTopAreaWidget::instance(), SLOT(musicBgTransparentChanged(QString)));
    connect(m_ui->resetWindowButton, SIGNAL(clicked()), MusicApplicationObject::instance(), SLOT(musicResetWindow()));
}

MusicBackgroundSkinDialog::~MusicBackgroundSkinDialog()
{
    delete m_ui;
    delete m_backgroundList;
    delete m_myBackgroundList;
}

QString MusicBackgroundSkinDialog::getClassName()
{
    return staticMetaObject.className();
}

QString MusicBackgroundSkinDialog::setMBackground(QString &name)
{
    QString path = USER_THEME_DIR_FULL + name + SKN_FILE;
    MusicBackgroundSkinDialog::themeValidCheck(name, path);
    M_BACKGROUND_PTR->setMBackground(path);
    return path;
}

bool MusicBackgroundSkinDialog::themeValidCheck(QString &name, QString &path)
{
    if(!QFile::exists(path))
    {
        QString orPath = QString("%1%2%3").arg(THEME_DIR_FULL).arg(name).arg(SKN_FILE);
        if(QFile::exists(orPath))
        {
            QFile::copy(orPath, QString("%1%2%3").arg(USER_THEME_DIR_FULL).arg(name).arg(SKN_FILE));
        }
        else
        {
            name = "theme-1";
            orPath = QString("%1%2%3").arg(THEME_DIR_FULL).arg(name).arg(SKN_FILE);
            QFile::copy(orPath, QString("%1%2%3").arg(USER_THEME_DIR_FULL).arg(name).arg(SKN_FILE));
        }
        return false;
    }
    return true;
}

QString MusicBackgroundSkinDialog::cpoyArtFileToLocal(const QString &path)
{
    int index = cpoyFileToLocal(path);
    return QString("theme-%1").arg(index + 1);
}

void MusicBackgroundSkinDialog::updateArtFileTheme(const QString &theme)
{
    QString des = QString("%1%2%3").arg(USER_THEME_DIR_FULL).arg(theme).arg(SKN_FILE);
    m_myBackgroundList->createItem(theme, des, true);
    m_myBackgroundList->updateLastedItem();
}

void MusicBackgroundSkinDialog::setCurrentBgTheme(const QString &theme, int alpha, int listAlpha)
{
    m_backgroundList->setCurrentItemName(theme);
    m_myBackgroundList->setCurrentItemName(theme);
    //Set the the slider bar value as what the alpha is
    m_ui->skinTransparentButton->setValue(alpha);
    m_ui->listTransparentButton->setValue(listAlpha);
    setSkinTransToolText(alpha);
    setListTransToolText(listAlpha);
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
    m_myBackgroundList->updateLastedItem();
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
    m_myBackgroundList->updateLastedItem();
}

void MusicBackgroundSkinDialog::backgroundListWidgetChanged(int index)
{
    if(m_ui->stackedWidget->getCurrentIndex() == index)
    {
        return;
    }

    if(index == 0)
    {
        m_ui->recommendSkin->setStyleSheet(MusicUIObject::MColorStyle08);
        m_ui->mySkin->setStyleSheet(MusicUIObject::MColorStyle03);
    }
    else
    {
        m_ui->recommendSkin->setStyleSheet(MusicUIObject::MColorStyle03);
        m_ui->mySkin->setStyleSheet(MusicUIObject::MColorStyle08);
    }
    m_ui->stackedWidget->setIndex(0, 0);
    m_ui->stackedWidget->start(index);
}

void MusicBackgroundSkinDialog::backgroundListWidgetItemClicked(const QString &name)
{
    MusicTopAreaWidget::instance()->musicBackgroundSkinChanged(name);

    if(!m_myBackgroundList->contains(name))
    {
        QString path = QString("%1%2%3").arg(USER_THEME_DIR_FULL).arg(name).arg(SKN_FILE);
        QFile::copy(QString("%1%2%3").arg(THEME_DIR_FULL).arg(name).arg(SKN_FILE), path);
        m_myBackgroundList->createItem(name, path, true);
    }
    m_myBackgroundList->clearSelectState();
    m_myBackgroundList->setCurrentItemName(name);

    QString s = name;
    setMBackground(s);
    emit M_BACKGROUND_PTR->backgroundHasChanged();
}

void MusicBackgroundSkinDialog::myBackgroundListWidgetItemClicked(const QString &name)
{
    MusicTopAreaWidget::instance()->musicBackgroundSkinChanged(name);

    m_backgroundList->clearSelectState();
    m_backgroundList->setCurrentItemName(name);

    QString s = name;
    setMBackground(s);
    emit M_BACKGROUND_PTR->backgroundHasChanged();
}

int MusicBackgroundSkinDialog::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}

void MusicBackgroundSkinDialog::addThemeListWidgetItem()
{
    QList<QFileInfo> files(QDir(THEME_DIR_FULL)
                         .entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name));
    foreach(const QFileInfo &info, files)
    {
        QString fileName = info.fileName();
        fileName.chop(4);
        m_backgroundList->createItem(fileName, info.filePath(), false);
    }

    files = QDir(USER_THEME_DIR_FULL)
                         .entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
    foreach(const QFileInfo &info, files)
    {
        QString fileName = info.fileName();
        fileName.chop(4);
        m_myBackgroundList->createItem(fileName, info.filePath(), true);
    }
}

void MusicBackgroundSkinDialog::cpoyFileFromLocal(const QString &path)
{
    m_myThemeIndex = cpoyFileToLocal(path);
    QString des = QString("%1theme-%2%3").arg(USER_THEME_DIR_FULL).arg(m_myThemeIndex + 1).arg(SKN_FILE);
    m_myBackgroundList->createItem(QString("theme-%1").arg(m_myThemeIndex + 1), des, true);
}

int MusicBackgroundSkinDialog::cpoyFileToLocal(const QString &path)
{
    QList<QFileInfo> files(QDir(USER_THEME_DIR_FULL)
                         .entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name));
    MusicObject::MIntList data;
    foreach(const QFileInfo &info, files)
    {
        QString fileName = info.fileName();
        fileName.chop(4);
        fileName = fileName.split("-").last();
        data << fileName.trimmed().toInt();
    }
    qSort(data.begin(), data.end(), qGreater<int>());

    int index = CURRENT_ITEMS_COUNT;
    if(!data.isEmpty())
    {
        index = data.first();
        if(index < CURRENT_ITEMS_COUNT)
        {
            index = CURRENT_ITEMS_COUNT;
        }
    }

    QString des = QString("%1theme-%2%3").arg(USER_THEME_DIR_FULL).arg(index + 1).arg(SKN_FILE);
    QFile::copy(path, des);

    return index;
}
