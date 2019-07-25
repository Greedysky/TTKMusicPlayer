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
#include "musicfileutils.h"
#include "musicsettingmanager.h"
#include "musicextractwrap.h"

#include <QScrollBar>

#define NEW_OPERATOR(ui, list)                                                  \
    ui->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);                   \
    ui->setWidgetResizable(true);                                               \
    ui->setFrameShape(QFrame::NoFrame);                                         \
    ui->setFrameShadow(QFrame::Plain);                                          \
    ui->setAlignment(Qt::AlignVCenter);                                         \
    ui->setWidget(list);                                                        \
    ui->verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle01);


MusicBackgroundSkinDialog::MusicBackgroundSkinDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicBackgroundSkinDialog)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));

    m_ui->paletteButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->customSkin->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->stackedWidget->setLength(m_ui->stackedWidget->width(), MusicAnimationStackedWidget::RightToLeft);
#ifdef Q_OS_UNIX
    m_ui->paletteButton->setFocusPolicy(Qt::NoFocus);
    m_ui->customSkin->setFocusPolicy(Qt::NoFocus);
    m_ui->resetWindowButton->setFocusPolicy(Qt::NoFocus);
    m_ui->skinTransparentLabelBox->setFocusPolicy(Qt::NoFocus);
#endif
    connect(m_ui->skinAnimationSiwidget, SIGNAL(buttonClicked(int)), SLOT(backgroundListWidgetChanged(int)));

    //
    m_backgroundList = new MusicBackgroundListWidget(this);
    NEW_OPERATOR(m_ui->recommandScrollArea, m_backgroundList);

    m_myBackgroundList = new MusicBackgroundListWidget(this);
    NEW_OPERATOR(m_ui->userScrollArea, m_myBackgroundList);

    m_thunderBackgroundList = new MusicBackgroundThunderWidget(this);
    NEW_OPERATOR(m_ui->remoteScrollArea, m_thunderBackgroundList);

    m_dailyBackgroundList = new MusicBackgroundDailyWidget(this);
    NEW_OPERATOR(m_ui->dailyScrollArea, m_dailyBackgroundList);

    m_myThemeIndex = CURRENT_ITEMS_COUNT;
    addThemeListWidgetItem();
    backgroundListWidgetChanged(0);
    //
    m_ui->resetWindowButton->setStyleSheet(MusicUIObject::MKGBtnResetWindow);
    m_ui->skinTransparentButton->setStyleSheet(MusicUIObject::MToolButtonStyle06);
    m_ui->listTransparentButton->setStyleSheet(MusicUIObject::MToolButtonStyle06);

    m_ui->skinTransparentButton->setEnabled(false);
    m_ui->skinTransparentLabelBox->setStyleSheet(MusicUIObject::MCheckBoxStyle04);
    m_ui->listTransparentLabel->setStyleSheet(MusicUIObject::MColorStyle03);

    connect(m_ui->skinTransparentLabelBox, SIGNAL(clicked(bool)), SLOT(windowTransparentChanged(bool)));
    connect(m_ui->skinTransparentButton, SIGNAL(valueChanged(int)), MusicTopAreaWidget::instance(), SLOT(musicBackgroundTransparentChanged(int)));
    connect(m_ui->skinTransparentButton, SIGNAL(sliderStateChanged(bool)), MusicTopAreaWidget::instance(), SLOT(musicBackgroundSliderStateChanged(bool)));
    connect(m_ui->listTransparentButton, SIGNAL(valueChanged(int)), MusicTopAreaWidget::instance(), SLOT(musicPlaylistTransparent(int)));
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
    delete m_thunderBackgroundList;
    delete m_dailyBackgroundList;
}


QPixmap MusicBackgroundSkinDialog::setBackgroundUrl(QString &name)
{
    QString path = USER_THEME_DIR_FULL + name + TTS_FILE;
    MusicBackgroundSkinDialog::themeValidCheck(name, path);
    M_BACKGROUND_PTR->setBackgroundUrl(path);

    MusicBackgroundImage image;
    return MusicExtractWrap::outputSkin(&image, path) ? image.m_pix : QPixmap();
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

QString MusicBackgroundSkinDialog::cpoyArtistFileToLocal(const QString &path)
{
    const int index = cpoyFileToLocal(path);
    return (index != -1) ? QString("theme-%1").arg(index + 1) : QString();
}

void MusicBackgroundSkinDialog::updateArtistFileTheme(const QString &theme)
{
    const QString &des = QString("%1%2%3").arg(USER_THEME_DIR_FULL).arg(theme).arg(TTS_FILE);
    m_myBackgroundList->createItem(theme, des, true);
    m_myBackgroundList->updateLastedItem();
}

void MusicBackgroundSkinDialog::setCurrentBackgroundTheme(const QString &theme, int alpha, int listAlpha)
{
    m_backgroundList->setCurrentItemName(theme);
    m_myBackgroundList->setCurrentItemName(theme);
    //Set the the slider bar value as what the alpha is
    m_ui->listTransparentButton->setValue(listAlpha);
    setListTransToolText(listAlpha);

    const bool state = M_SETTING_PTR->value(MusicSettingManager::BackgroundTransparentEnableChoiced).toBool();
    m_ui->skinTransparentButton->setValue(state ? alpha : 100);
    m_ui->skinTransparentButton->setEnabled(state);
    setSkinTransToolText(state ? alpha : 100);
    m_ui->skinTransparentLabelBox->setChecked(state);
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
    connect(&paletteWidget, SIGNAL(currentColorToFileChanged(QString)), SLOT(showPaletteDialog(QString)));
    connect(&paletteWidget, SIGNAL(currentColorToMemoryChanged(QString)), SLOT(currentColorChanged(QString)));
    paletteWidget.exec();
}

void MusicBackgroundSkinDialog::showPaletteDialog(const QString &path)
{
    cpoyFileFromLocal( path );
    m_myBackgroundList->updateLastedItem();
}

void MusicBackgroundSkinDialog::showCustomSkinDialog()
{
    QString customSkinPath = MusicUtils::File::getOpenFileDialog(this, "Images (*.png *.bmp *.jpg);;TTKS Files(*.ttks)");
    if(customSkinPath.isEmpty())
    {
        return;
    }

    if(QFileInfo(customSkinPath).suffix().toLower() == TTS_FILE_PREFIX)
    {
        const int index = cpoyFileToLocalIndex();
        if(index != -1)
        {
            m_myThemeIndex = index;
            QString des = QString("%1theme-%2%3").arg(USER_THEME_DIR_FULL).arg(index + 1).arg(TTS_FILE);
            QFile::copy(customSkinPath, des);
            m_myBackgroundList->createItem(QString("theme-%1").arg(index + 1), des, true);
        }
    }
    else
    {
        cpoyFileFromLocal( customSkinPath );
    }
    m_myBackgroundList->updateLastedItem();
}

void MusicBackgroundSkinDialog::backgroundListWidgetChanged(int index)
{
    QWidget *toolWidget = m_thunderBackgroundList->createFunctionsWidget(index != 3, this);
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
    //
    m_dailyBackgroundList->abort();
    m_thunderBackgroundList->abort();
    if(index == 2)
    {
        m_dailyBackgroundList->init();
    }
    else if(index == 3)
    {
        m_thunderBackgroundList->init();
    }
    //
    m_ui->stackedWidget->setIndex(0, 0);
    m_ui->stackedWidget->start(index);
}

void MusicBackgroundSkinDialog::backgroundListWidgetItemClicked(const QString &name)
{
    if(!m_myBackgroundList->contains(name))
    {
        const QString &path = QString("%1%2%3").arg(USER_THEME_DIR_FULL).arg(name).arg(TTS_FILE);
        QFile::copy(QString("%1%2%3").arg(THEME_DIR_FULL).arg(name).arg(TTS_FILE), path);
        m_myBackgroundList->createItem(name, path, true);
    }
    listWidgetItemClicked(m_myBackgroundList, name);
}

void MusicBackgroundSkinDialog::myBackgroundListWidgetItemClicked(const QString &name)
{
    listWidgetItemClicked(m_backgroundList, name);
}

void MusicBackgroundSkinDialog::dailyBackgroundListWidgetItemClicked(const QString &name)
{
    listWidgetItemClicked(m_dailyBackgroundList, name);
}

void MusicBackgroundSkinDialog::remoteBackgroundListWidgetItemClicked(const QString &name)
{
    listWidgetItemClicked(m_thunderBackgroundList, name);
}

void MusicBackgroundSkinDialog::currentColorChanged(const QString &path)
{
    if(path.contains(MUSIC_COLOR_FILE))
    {
        MusicTopAreaWidget::instance()->musicBackgroundTransparentChanged(path);
    }
    else
    {
        MusicTopAreaWidget::instance()->musicBackgroundSkinCustumChanged(QFileInfo(path).baseName());
    }
    M_BACKGROUND_PTR->setBackgroundUrl(path);
    emit M_BACKGROUND_PTR->backgroundHasChanged();
}

void MusicBackgroundSkinDialog::windowTransparentChanged(bool state)
{
    m_ui->skinTransparentButton->setEnabled(state);
    M_SETTING_PTR->setValue(MusicSettingManager::BackgroundTransparentEnableChoiced, state);
    if(!state)
    {
        m_ui->skinTransparentButton->setValue(100);
        MusicTopAreaWidget::instance()->musicBackgroundTransparentChanged(100);
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
    M_BACKGROUND_PTR->setBackgroundUrl(path);
    emit M_BACKGROUND_PTR->backgroundHasChanged();
}

void MusicBackgroundSkinDialog::listWidgetItemClicked(MusicBackgroundRemoteWidget *item, const QString &name)
{
    MusicBackgroundImage image;
    item->outputRemoteSkin(image, name);
    if(!image.isValid())
    {
        return;
    }

    if(!m_myBackgroundList->contains(image))
    {
        const int index = cpoyFileToLocalIndex();
        const QString &theme = QString("theme-%1").arg(index + 1);
        const QString &des = QString("%1%2%3").arg(USER_THEME_DIR_FULL).arg(theme).arg(TTS_FILE);
        MusicExtractWrap::inputSkin(&image, des);

        m_myBackgroundList->createItem(theme, des, true);
        listWidgetItemClicked(m_myBackgroundList, theme);
    }
    else
    {
        const MusicBackgroundListItem *it = m_myBackgroundList->find(image);
        if(it)
        {
            listWidgetItemClicked(m_myBackgroundList, it->getFileName());
        }
    }
}

void MusicBackgroundSkinDialog::addThemeListWidgetItem()
{
    addThemeListWidgetItem(m_backgroundList, THEME_DIR_FULL, false);
    addThemeListWidgetItem(m_myBackgroundList, USER_THEME_DIR_FULL, true);
}

void MusicBackgroundSkinDialog::addThemeListWidgetItem(MusicBackgroundListWidget *item, const QString &dir, bool state)
{
   const  QStringList files(QDir(dir).entryList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name));
    MIntList data;
    foreach(const QString &path, files)
    {
        QString fileName = QFileInfo(path).baseName();
                fileName = fileName.split("-").last();
        data << fileName.trimmed().toInt();
    }
    qSort(data.begin(), data.end(), qLess<int>());

    foreach(const int index, data)
    {
        const QFileInfo info(QString("%1theme-%2%3").arg(dir).arg(index).arg(TTS_FILE));
        item->createItem(info.baseName(), info.filePath(), state);
    }
}

void MusicBackgroundSkinDialog::cpoyFileFromLocal(const QString &path)
{
    const int index = cpoyFileToLocal(path);
    if(index != -1)
    {
        m_myThemeIndex = index;
        const QString &des = QString("%1theme-%2%3").arg(USER_THEME_DIR_FULL).arg(m_myThemeIndex + 1).arg(TTS_FILE);
        m_myBackgroundList->createItem(QString("theme-%1").arg(m_myThemeIndex + 1), des, true);
    }
}

int MusicBackgroundSkinDialog::cpoyFileToLocalIndex()
{
    const QList<QFileInfo> files(QDir(USER_THEME_DIR_FULL).entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name));
    MIntList data;
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
    const int index = cpoyFileToLocalIndex();

    const QString &des = QString("%1theme-%2%3").arg(USER_THEME_DIR_FULL).arg(index + 1).arg(TTS_FILE);
    MusicBackgroundImage image;
    image.m_pix = QPixmap(path);
    return MusicExtractWrap::inputSkin(&image, des) ? index : -1;
}
