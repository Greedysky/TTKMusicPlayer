#include "musicbackgroundskindialog.h"
#include "ui_musicbackgroundskindialog.h"
#include "musicbackgroundmanager.h"
#include "musicbackgroundpalettewidget.h"
#include "musicfunctionuiobject.h"
#include "musicbackgroundremotewidget.h"
#include "musicobject.h"
#include "musictopareawidget.h"
#include "musicapplicationobject.h"
#include "musicotherdefine.h"
#include "musicwidgetutils.h"
#include "musicsettingmanager.h"

#include <QScrollBar>
#include <QSignalMapper>

MusicBackgroundSkinDialog::MusicBackgroundSkinDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicBackgroundSkinDialog)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    m_ui->recommendSkin->setStyleSheet(MusicUIObject::MColorStyle08);
    m_ui->mySkin->setStyleSheet(MusicUIObject::MColorStyle03);
    m_ui->remoteSkin->setStyleSheet(MusicUIObject::MColorStyle03);
    m_ui->paletteButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->customSkin->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->stackedWidget->setLength(m_ui->stackedWidget->width(), MusicAnimationStackedWidget::RightToLeft);
#ifdef Q_OS_UNIX
    m_ui->paletteButton->setFocusPolicy(Qt::NoFocus);
    m_ui->customSkin->setFocusPolicy(Qt::NoFocus);
    m_ui->resetWindowButton->setFocusPolicy(Qt::NoFocus);
    m_ui->skinTransparentLabelBox->setFocusPolicy(Qt::NoFocus);
#endif

    QSignalMapper *group = new QSignalMapper(this);
    group->setMapping(m_ui->recommendSkin, 0);
    group->setMapping(m_ui->mySkin, 1);
    group->setMapping(m_ui->remoteSkin, 2);
    connect(m_ui->recommendSkin, SIGNAL(clicked()), group, SLOT(map()));
    connect(m_ui->mySkin, SIGNAL(clicked()), group, SLOT(map()));
    connect(m_ui->remoteSkin, SIGNAL(clicked()), group, SLOT(map()));
    connect(group, SIGNAL(mapped(int)), SLOT(backgroundListWidgetChanged(int)));

    //////////////////////////////////////////////////////
    m_backgroundList = new MusicBackgroundListWidget(this);
    m_ui->recommandScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->recommandScrollArea->setWidgetResizable(true);
    m_ui->recommandScrollArea->setFrameShape(QFrame::NoFrame);
    m_ui->recommandScrollArea->setFrameShadow(QFrame::Plain);
    m_ui->recommandScrollArea->setAlignment(Qt::AlignVCenter);
    m_ui->recommandScrollArea->setWidget(m_backgroundList);
    m_ui->recommandScrollArea->verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle03);

    m_myBackgroundList = new MusicBackgroundListWidget(this);
    m_ui->userScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->userScrollArea->setWidgetResizable(true);
    m_ui->userScrollArea->setFrameShape(QFrame::NoFrame);
    m_ui->userScrollArea->setFrameShadow(QFrame::Plain);
    m_ui->userScrollArea->setAlignment(Qt::AlignVCenter);
    m_ui->userScrollArea->setWidget(m_myBackgroundList);
    m_ui->userScrollArea->verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle03);

    m_remoteBackgroundList = new MusicBackgroundRemoteWidget(this);
    m_ui->remoteScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->remoteScrollArea->setWidgetResizable(true);
    m_ui->remoteScrollArea->setFrameShape(QFrame::NoFrame);
    m_ui->remoteScrollArea->setFrameShadow(QFrame::Plain);
    m_ui->remoteScrollArea->setAlignment(Qt::AlignVCenter);
    m_ui->remoteScrollArea->setWidget(m_remoteBackgroundList);
    m_ui->remoteScrollArea->verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle03);

    m_myThemeIndex = CURRENT_ITEMS_COUNT;
    addThemeListWidgetItem();
    backgroundListWidgetChanged(0);
    //////////////////////////////////////////////////////
    m_ui->resetWindowButton->setStyleSheet(MusicUIObject::MKGBtnResetWindow);
    m_ui->skinTransparentButton->setStyleSheet(MusicUIObject::MToolButtonStyle06);
    m_ui->listTransparentButton->setStyleSheet(MusicUIObject::MToolButtonStyle06);

    m_ui->skinTransparentLabelBox->setStyleSheet(MusicUIObject::MCheckBoxStyle04);
    m_ui->listTransparentLabel->setStyleSheet(MusicUIObject::MColorStyle03);

    connect(m_ui->skinTransparentLabelBox, SIGNAL(clicked(bool)), SLOT(windowTransparentChanged(bool)));
    connect(m_ui->skinTransparentButton, SIGNAL(valueChanged(int)), MusicTopAreaWidget::instance(), SLOT(musicBgTransparentChanged(int)));
    connect(m_ui->skinTransparentButton, SIGNAL(sliderStateChanged(bool)), MusicTopAreaWidget::instance(), SLOT(musicBackgroundSliderStateChanged(bool)));
    connect(m_ui->listTransparentButton, SIGNAL(valueChanged(int)), MusicTopAreaWidget::instance(), SLOT(musicPlayListTransparent(int)));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->paletteButton, SIGNAL(clicked()), SLOT(showPaletteDialog()));
    connect(m_ui->customSkin, SIGNAL(clicked()) ,SLOT(showCustomSkinDialog()));
    connect(m_backgroundList, SIGNAL(itemClicked(QString)), SLOT(backgroundListWidgetItemClicked(QString)));
    connect(m_myBackgroundList, SIGNAL(itemClicked(QString)), SLOT(myBackgroundListWidgetItemClicked(QString)));
    connect(m_ui->resetWindowButton, SIGNAL(clicked()), MusicApplicationObject::instance(), SLOT(musicResetWindow()));
}

MusicBackgroundSkinDialog::~MusicBackgroundSkinDialog()
{
    delete m_ui;
    delete m_backgroundList;
    delete m_myBackgroundList;
    delete m_remoteBackgroundList;
}

QString MusicBackgroundSkinDialog::getClassName()
{
    return staticMetaObject.className();
}

QPixmap MusicBackgroundSkinDialog::setMBackground(QString &name)
{
    QString path = USER_THEME_DIR_FULL + name + TTS_FILE;
    MusicBackgroundSkinDialog::themeValidCheck(name, path);
    M_BACKGROUND_PTR->setMBackground(path);

    MusicBackgroundImage image;
    return MusicBackgroundImageWrap::outputSkin(image, path) ? image.m_pix : QPixmap();
}

bool MusicBackgroundSkinDialog::themeValidCheck(QString &name, QString &path)
{
    if(!QFile::exists(path))
    {
        QString orPath = QString("%1%2%3").arg(THEME_DIR_FULL).arg(name).arg(TTS_FILE);
        if(QFile::exists(orPath))
        {
            QFile::copy(orPath, QString("%1%2%3").arg(USER_THEME_DIR_FULL).arg(name).arg(TTS_FILE));
        }
        else
        {
            name = "theme-1";
            orPath = QString("%1%2%3").arg(THEME_DIR_FULL).arg(name).arg(TTS_FILE);
            QFile::copy(orPath, QString("%1%2%3").arg(USER_THEME_DIR_FULL).arg(name).arg(TTS_FILE));
        }
        return false;
    }
    return true;
}

QString MusicBackgroundSkinDialog::cpoyArtFileToLocal(const QString &path)
{
    int index = cpoyFileToLocal(path);
    return (index != -1) ? QString("theme-%1").arg(index + 1) : QString();
}

void MusicBackgroundSkinDialog::updateArtFileTheme(const QString &theme)
{
    QString des = QString("%1%2%3").arg(USER_THEME_DIR_FULL).arg(theme).arg(TTS_FILE);
    m_myBackgroundList->createItem(theme, des, true);
    m_myBackgroundList->updateLastedItem();
}

void MusicBackgroundSkinDialog::setCurrentBgTheme(const QString &theme, int alpha, int listAlpha)
{
    m_backgroundList->setCurrentItemName(theme);
    m_myBackgroundList->setCurrentItemName(theme);
    //Set the the slider bar value as what the alpha is
    m_ui->listTransparentButton->setValue(listAlpha);
    setListTransToolText(listAlpha);

    bool s = M_SETTING_PTR->value(MusicSettingManager::BgTransparentEnableChoiced).toBool();
    m_ui->skinTransparentButton->setValue(s ? alpha : 100);
    setSkinTransToolText(s ? alpha : 100);
    if(s)
    {
        m_ui->skinTransparentLabelBox->click();
    }
}

int MusicBackgroundSkinDialog::getBackgroundListAlpha() const
{
    return m_ui->listTransparentButton->value();
}

bool MusicBackgroundSkinDialog::getBackgroundTransparentEnable() const
{
    return m_ui->skinTransparentLabelBox->isChecked();
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
                            SLOT(currentColorChanged(QString)));
    paletteWidget.exec();
}

void MusicBackgroundSkinDialog::showPaletteDialog(const QString &path)
{
    cpoyFileFromLocal( path );
    m_myBackgroundList->updateLastedItem();
}

void MusicBackgroundSkinDialog::showCustomSkinDialog()
{
    QString customSkinPath = MusicUtils::Widget::getOpenFileDialog(this);
    if(customSkinPath.isEmpty())
    {
        return;
    }
    cpoyFileFromLocal( customSkinPath );
    m_myBackgroundList->updateLastedItem();
}

void MusicBackgroundSkinDialog::backgroundListWidgetChanged(int index)
{
    QWidget *toolWidget = m_remoteBackgroundList->createFunctionsWidget(index != 2, this);
    if(!toolWidget->isVisible())
    {
        toolWidget->show();
        QRect rect = m_ui->stackedWidget->geometry();
        m_ui->stackedWidget->setGeometry(QRect(rect.x(), rect.y() + toolWidget->height(), rect.width(), rect.height() - toolWidget->height()));
    }

    if(m_ui->stackedWidget->getCurrentIndex() == index)
    {
        return;
    }

    m_remoteBackgroundList->abort();
    if(index == 0)
    {
        m_ui->recommendSkin->setStyleSheet(MusicUIObject::MColorStyle08);
        m_ui->mySkin->setStyleSheet(MusicUIObject::MColorStyle03);
        m_ui->remoteSkin->setStyleSheet(MusicUIObject::MColorStyle03);
    }
    else if(index == 1)
    {
        m_ui->recommendSkin->setStyleSheet(MusicUIObject::MColorStyle03);
        m_ui->mySkin->setStyleSheet(MusicUIObject::MColorStyle08);
        m_ui->remoteSkin->setStyleSheet(MusicUIObject::MColorStyle03);
    }
    else if(index == 2)
    {
        m_remoteBackgroundList->init();
        m_ui->recommendSkin->setStyleSheet(MusicUIObject::MColorStyle03);
        m_ui->mySkin->setStyleSheet(MusicUIObject::MColorStyle03);
        m_ui->remoteSkin->setStyleSheet(MusicUIObject::MColorStyle08);
    }

    m_ui->stackedWidget->setIndex(0, 0);
    m_ui->stackedWidget->start(index);
}

void MusicBackgroundSkinDialog::backgroundListWidgetItemClicked(const QString &name)
{
    if(!m_myBackgroundList->contains(name))
    {
        QString path = QString("%1%2%3").arg(USER_THEME_DIR_FULL).arg(name).arg(TTS_FILE);
        QFile::copy(QString("%1%2%3").arg(THEME_DIR_FULL).arg(name).arg(TTS_FILE), path);
        m_myBackgroundList->createItem(name, path, true);
    }
    listWidgetItemClicked(m_myBackgroundList, name);
}

void MusicBackgroundSkinDialog::myBackgroundListWidgetItemClicked(const QString &name)
{
    listWidgetItemClicked(m_backgroundList, name);
}

void MusicBackgroundSkinDialog::remoteBackgroundListWidgetItemClicked(const QString &name)
{
    MusicBackgroundImage image;
    m_remoteBackgroundList->outputRemoteSkin(image, name);
    if(!image.isValid())
    {
        return;
    }

    if(!m_myBackgroundList->contains(image))
    {
        int index = cpoyFileToLocalIndex();
        QString theme = QString("theme-%1").arg(index + 1);
        QString des = QString("%1%2%3").arg(USER_THEME_DIR_FULL).arg(theme).arg(TTS_FILE);
        MusicBackgroundImageWrap::inputSkin(image, des);

        m_myBackgroundList->createItem(theme, des, true);
        listWidgetItemClicked(m_myBackgroundList, theme);
    }
    else
    {
        MusicBackgroundListItem *it = m_myBackgroundList->find(image);
        if(it)
        {
            listWidgetItemClicked(m_myBackgroundList, it->getFileName());
        }
    }
}

void MusicBackgroundSkinDialog::currentColorChanged(const QString &path)
{
    if(path.contains(MUSIC_COLOR_FILE))
    {
        MusicTopAreaWidget::instance()->musicBgTransparentChanged(path);
    }
    else
    {
        MusicTopAreaWidget::instance()->musicBackgroundSkinCustumChanged(QFileInfo(path).baseName());
    }
    M_BACKGROUND_PTR->setMBackground(path);
    emit M_BACKGROUND_PTR->backgroundHasChanged();
}

void MusicBackgroundSkinDialog::windowTransparentChanged(bool state)
{
    m_ui->skinTransparentButton->setEnabled(state);
    if(!state)
    {
        m_ui->skinTransparentButton->setValue(100);
        MusicTopAreaWidget::instance()->musicBgTransparentChanged(100);
    }
}

int MusicBackgroundSkinDialog::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}

void MusicBackgroundSkinDialog::listWidgetItemClicked(MusicBackgroundListWidget *item, const QString &name)
{
    MusicTopAreaWidget::instance()->musicBackgroundSkinChanged(name);

    item->clearSelectState();
    item->setCurrentItemName(name);

    QString s(name);
    QString path = USER_THEME_DIR_FULL + s + TTS_FILE;
    MusicBackgroundSkinDialog::themeValidCheck(s, path);
    M_BACKGROUND_PTR->setMBackground(path);
    emit M_BACKGROUND_PTR->backgroundHasChanged();
}

void MusicBackgroundSkinDialog::addThemeListWidgetItem()
{
    QList<QFileInfo> files(QDir(THEME_DIR_FULL)
                         .entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name));
    foreach(const QFileInfo &info, files)
    {
        m_backgroundList->createItem(info.baseName(), info.filePath(), false);
    }

    files = QDir(USER_THEME_DIR_FULL)
                         .entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
    foreach(const QFileInfo &info, files)
    {
        m_myBackgroundList->createItem(info.baseName(), info.filePath(), true);
    }
}

void MusicBackgroundSkinDialog::cpoyFileFromLocal(const QString &path)
{
    int index = cpoyFileToLocal(path);
    if(index != -1)
    {
        m_myThemeIndex = index;
        QString des = QString("%1theme-%2%3").arg(USER_THEME_DIR_FULL).arg(m_myThemeIndex + 1).arg(TTS_FILE);
        m_myBackgroundList->createItem(QString("theme-%1").arg(m_myThemeIndex + 1), des, true);
    }
}

int MusicBackgroundSkinDialog::cpoyFileToLocalIndex()
{
    QList<QFileInfo> files(QDir(USER_THEME_DIR_FULL)
                         .entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name));
    MusicObject::MIntList data;
    foreach(const QFileInfo &info, files)
    {
        QString fileName = info.baseName();
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

    return index;
}

int MusicBackgroundSkinDialog::cpoyFileToLocal(const QString &path)
{
    int index = cpoyFileToLocalIndex();

    QString des = QString("%1theme-%2%3").arg(USER_THEME_DIR_FULL).arg(index + 1).arg(TTS_FILE);
    MusicBackgroundImage image;
    image.m_pix = QPixmap(path);
    return MusicBackgroundImageWrap::inputSkin(image, des) ? index : -1;
}
