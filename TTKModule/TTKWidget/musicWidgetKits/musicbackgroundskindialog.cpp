#include "musicbackgroundskindialog.h"
#include "ui_musicbackgroundskindialog.h"
#include "musicbackgroundmanager.h"
#include "musicbackgroundpalettewidget.h"
#include "musicfunctionuiobject.h"
#include "musicbackgroundremotewidget.h"
#include "musictopareawidget.h"
#include "musicapplicationmodule.h"
#include "musicfileutils.h"
#include "musicsettingmanager.h"
#include "musicextractwrapper.h"

static constexpr int CURRENT_ITEMS_COUNT = 58;

MusicBackgroundSkinDialog::MusicBackgroundSkinDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicBackgroundSkinDialog),
      m_stackThemeIndex(CURRENT_ITEMS_COUNT)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->paletteButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->customSkin->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->stackedWidget->setLength(m_ui->stackedWidget->width(), MusicAnimationStackedWidget::Module::RightToLeft);
#ifdef Q_OS_UNIX
    m_ui->paletteButton->setFocusPolicy(Qt::NoFocus);
    m_ui->customSkin->setFocusPolicy(Qt::NoFocus);
    m_ui->resetWindowButton->setFocusPolicy(Qt::NoFocus);
    m_ui->skinTransparentLabelBox->setFocusPolicy(Qt::NoFocus);
#endif
    connect(m_ui->skinAnimationSiwidget, SIGNAL(buttonClicked(int)), SLOT(backgroundListWidgetChanged(int)));

    m_cacheBackgroundList = new MusicBackgroundListWidget(this);
    m_cacheBackgroundList->setType(MusicBackgroundListWidget::CachedModule);
    TTK::Widget::generateVScrollAreaFormat(m_ui->recommendScrollArea, m_cacheBackgroundList);

    m_stackBackgroundList = new MusicBackgroundListWidget(this);
    m_stackBackgroundList->setType(MusicBackgroundListWidget::StackedModule);
    TTK::Widget::generateVScrollAreaFormat(m_ui->userScrollArea, m_stackBackgroundList);

    m_dailyBackgroundList = new MusicBackgroundDailyWidget(this);
    TTK::Widget::generateVScrollAreaFormat(m_ui->dailyScrollArea, m_dailyBackgroundList);

    m_onlineBackgroundList = new MusicBackgroundOnlineWidget(this);
    TTK::Widget::generateVScrollAreaFormat(m_ui->remoteScrollArea, m_onlineBackgroundList);

    addThemeListWidgetItem();
    backgroundListWidgetChanged(0);

    m_ui->resetWindowButton->setStyleSheet(TTK::UI::BtnResetWindow);
    m_ui->skinTransparentButton->setStyleSheet(TTK::UI::ToolButtonStyle05);
    m_ui->listTransparentButton->setStyleSheet(TTK::UI::ToolButtonStyle05);

    m_ui->skinTransparentButton->setEnabled(false);
    m_ui->skinTransparentLabelBox->setStyleSheet(TTK::UI::CheckBoxStyle04);
    m_ui->listTransparentLabel->setStyleSheet(TTK::UI::ColorStyle03);

    connect(m_ui->skinTransparentLabelBox, SIGNAL(clicked(bool)), SLOT(windowTransparentChanged(bool)));
    connect(m_ui->skinTransparentButton, SIGNAL(valueChanged(int)), MusicTopAreaWidget::instance(), SLOT(backgroundTransparentChanged(int)));
    connect(m_ui->skinTransparentButton, SIGNAL(sliderStateChanged(bool)), MusicTopAreaWidget::instance(), SLOT(backgroundAnimationChanged(bool)));
    connect(m_ui->listTransparentButton, SIGNAL(valueChanged(int)), MusicTopAreaWidget::instance(), SLOT(playlistTransparent(int)));
    connect(m_ui->paletteButton, SIGNAL(clicked()), SLOT(showPaletteDialog()));
    connect(m_ui->customSkin, SIGNAL(clicked()) ,SLOT(showCustomSkinDialog()));
    connect(m_ui->resetWindowButton, SIGNAL(clicked()), MusicApplicationModule::instance(), SLOT(resetWindowGeometry()));

    connect(m_cacheBackgroundList, SIGNAL(itemClicked(int,QString)), SLOT(classicalListWidgetItemClicked(int,QString)));
    connect(m_stackBackgroundList, SIGNAL(itemClicked(int,QString)), SLOT(classicalListWidgetItemClicked(int,QString)));
}

MusicBackgroundSkinDialog::~MusicBackgroundSkinDialog()
{
    delete m_ui;
    delete m_cacheBackgroundList;
    delete m_stackBackgroundList;
    delete m_dailyBackgroundList;
    delete m_onlineBackgroundList;
}

QPixmap MusicBackgroundSkinDialog::setBackgroundUrl(QString &name)
{
    QString path = USER_THEME_DIR_FULL + name + TKM_FILE;
    MusicBackgroundSkinDialog::themeIsValid(name, path);
    G_BACKGROUND_PTR->setBackgroundUrl(path);

    MusicBackgroundImage image;
    return MusicExtractWrapper::outputSkin(&image, path) ? image.m_pix : QPixmap();
}

bool MusicBackgroundSkinDialog::themeIsValid(QString &name, QString &path)
{
    if(!QFile::exists(path))
    {
        QString orPath = QString("%1%2%3").arg(THEME_DIR_FULL, name, TKM_FILE);
        if(QFile::exists(orPath))
        {
            QFile::copy(orPath, QString("%1%2%3").arg(USER_THEME_DIR_FULL, name, TKM_FILE));
        }
        else
        {
            name = "theme-1";
            orPath = QString("%1%2%3").arg(THEME_DIR_FULL, name, TKM_FILE);
            QFile::copy(orPath, QString("%1%2%3").arg(USER_THEME_DIR_FULL, name, TKM_FILE));
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
    const QString &des = QString("%1%2%3").arg(USER_THEME_DIR_FULL, theme, TKM_FILE);
    m_stackBackgroundList->addCellItem(theme, des, true);
    m_stackBackgroundList->updateLastItem();
}

void MusicBackgroundSkinDialog::setCurrentBackgroundTheme(const QString &theme, int skin, int list)
{
    m_cacheBackgroundList->setCurrentItemName(theme);
    m_stackBackgroundList->setCurrentItemName(theme);
    //Set the the slider bar value as what the alpha is
    m_ui->listTransparentButton->setValue(list);
    setListTransToolText(list);

    const bool state = G_SETTING_PTR->value(MusicSettingManager::BackgroundTransparentEnable).toBool();
    m_ui->skinTransparentButton->setValue(state ? skin : 0);
    m_ui->skinTransparentButton->setEnabled(state);
    setSkinTransToolText(state ? skin : 0);
    m_ui->skinTransparentLabelBox->setChecked(state);
}

int MusicBackgroundSkinDialog::backgroundListAlpha() const
{
    return m_ui->listTransparentButton->value();
}

bool MusicBackgroundSkinDialog::backgroundTransparentEnable() const
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
    MusicBackgroundPaletteWidget widget(this);
    connect(&widget, SIGNAL(currentColorToFileChanged(QString)), SLOT(showPaletteDialog(QString)));
    connect(&widget, SIGNAL(currentColorToMemoryChanged(QString)), SLOT(currentColorChanged(QString)));
    widget.exec();
}

void MusicBackgroundSkinDialog::showPaletteDialog(const QString &path)
{
    cpoyFileFromLocal(path);
    m_stackBackgroundList->updateLastItem();
}

void MusicBackgroundSkinDialog::showCustomSkinDialog()
{
    const QString &path = TTK::File::getOpenFileName(this, "Image Files (*.png *.bmp *.jpg);;TKM Files (*.tkm)");
    if(path.isEmpty())
    {
        return;
    }

    if(TTK_FILE_SUFFIX(QFileInfo(path)) == TKM_FILE_SUFFIX)
    {
        const int index = cpoyFileToLocalIndex();
        if(index != -1)
        {
            m_stackThemeIndex = index;
            const QString &des = QString("%1theme-%2%3").arg(USER_THEME_DIR_FULL).arg(index + 1).arg(TKM_FILE);
            QFile::copy(path, des);
            m_stackBackgroundList->addCellItem(QString("theme-%1").arg(index + 1), des, true);
        }
    }
    else
    {
        cpoyFileFromLocal(path);
    }

    m_stackBackgroundList->updateLastItem();
}

void MusicBackgroundSkinDialog::backgroundListWidgetChanged(int index)
{
    QWidget *toolWidget = m_onlineBackgroundList->createFunctionsWidget(index != 3, this);
    if(!toolWidget->isVisible())
    {
        toolWidget->show();
        QRect rect = m_ui->stackedWidget->geometry();
        m_ui->stackedWidget->setGeometry(QRect(rect.x(), rect.y() + toolWidget->height(), rect.width(), rect.height() - toolWidget->height()));
    }

    if(m_ui->stackedWidget->currentIndex() == index)
    {
        return;
    }
    //
    m_dailyBackgroundList->abort();
    m_onlineBackgroundList->abort();
    //
    if(index == 2)
    {
        m_dailyBackgroundList->initialize();
    }
    else if(index == 3)
    {
        m_onlineBackgroundList->initialize();
    }
    //
    m_ui->stackedWidget->setIndex(0, 0);
    m_ui->stackedWidget->start(index);
}

void MusicBackgroundSkinDialog::classicalListWidgetItemClicked(int type, const QString &name)
{
    if(type == MusicBackgroundListWidget::CachedModule)
    {
        if(!m_stackBackgroundList->contains(name))
        {
            const QString &path = QString("%1%2%3").arg(USER_THEME_DIR_FULL, name, TKM_FILE);
            QFile::copy(QString("%1%2%3").arg(THEME_DIR_FULL, name, TKM_FILE), path);
            m_stackBackgroundList->addCellItem(name, path, true);
        }

        listWidgetItemClicked(m_stackBackgroundList, name);
    }
    else
    {
        listWidgetItemClicked(m_cacheBackgroundList, name);
    }
}

void MusicBackgroundSkinDialog::remoteListWidgetItemClicked(int type, const QString &name)
{
    switch(type)
    {
        case MusicBackgroundListWidget::DailyModule: listWidgetItemClicked(m_dailyBackgroundList, name); break;
        case MusicBackgroundListWidget::OnlineModule: listWidgetItemClicked(m_onlineBackgroundList, name); break;
        default: break;
    }
}

void MusicBackgroundSkinDialog::currentColorChanged(const QString &path)
{
    if(path.contains(TTK_COLOR_FILE))
    {
        MusicTopAreaWidget::instance()->backgroundTransparentChanged(path);
    }
    else
    {
        MusicTopAreaWidget::instance()->backgroundSkinChanged(QFileInfo(path).baseName());
    }

    G_BACKGROUND_PTR->setBackgroundUrl(path);
    G_BACKGROUND_PTR->backgroundUrlChanged();
}

void MusicBackgroundSkinDialog::windowTransparentChanged(bool state)
{
    m_ui->skinTransparentButton->setEnabled(state);
    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundTransparentEnable, state);
    if(!state)
    {
        m_ui->skinTransparentButton->setValue(0);
        MusicTopAreaWidget::instance()->backgroundTransparentChanged(0);
    }
}

void MusicBackgroundSkinDialog::listWidgetItemClicked(MusicBackgroundListWidget *item, const QString &name)
{
    MusicTopAreaWidget::instance()->backgroundSkinChanged(name);

    item->clearState();
    item->setCurrentItemName(name);

    QString s(name);
    QString path = USER_THEME_DIR_FULL + s + TKM_FILE;
    MusicBackgroundSkinDialog::themeIsValid(s, path);

    G_BACKGROUND_PTR->setBackgroundUrl(path);
    G_BACKGROUND_PTR->backgroundUrlChanged();
}

void MusicBackgroundSkinDialog::listWidgetItemClicked(MusicBackgroundRemoteWidget *item, const QString &name)
{
    MusicBackgroundImage image;
    item->outputRemoteSkin(image, name);

    if(!image.isValid())
    {
        return;
    }

    m_cacheBackgroundList->clearState();
    if(!m_stackBackgroundList->contains(image))
    {
        const int index = cpoyFileToLocalIndex();
        const QString &theme = QString("theme-%1").arg(index + 1);
        const QString &des = QString("%1%2%3").arg(USER_THEME_DIR_FULL, theme, TKM_FILE);
        MusicExtractWrapper::inputSkin(&image, des);

        m_stackBackgroundList->addCellItem(theme, des, true);
        listWidgetItemClicked(m_stackBackgroundList, theme);
    }
    else
    {
        const MusicBackgroundListItem *it = m_stackBackgroundList->find(image);
        if(it)
        {
            listWidgetItemClicked(m_stackBackgroundList, it->fileName());
        }
    }
}

void MusicBackgroundSkinDialog::addThemeListWidgetItem()
{
    addThemeListWidgetItem(m_cacheBackgroundList, THEME_DIR_FULL, false);
    addThemeListWidgetItem(m_stackBackgroundList, USER_THEME_DIR_FULL, true);
}

void MusicBackgroundSkinDialog::addThemeListWidgetItem(MusicBackgroundListWidget *item, const QString &dir, bool state)
{
    TTKIntList data;
    findThemeListByPath(dir, data);
    std::sort(data.begin(), data.end());

    for(const int index : qAsConst(data))
    {
        const QFileInfo fin(QString("%1theme-%2%3").arg(dir).arg(index).arg(TKM_FILE));
        item->addCellItem(fin.baseName(), fin.filePath(), state);
    }
}

void MusicBackgroundSkinDialog::cpoyFileFromLocal(const QString &path)
{
    const int index = cpoyFileToLocal(path);
    if(index != -1)
    {
        m_stackThemeIndex = index;
        const QString &des = QString("%1theme-%2%3").arg(USER_THEME_DIR_FULL).arg(m_stackThemeIndex + 1).arg(TKM_FILE);
        m_stackBackgroundList->addCellItem(QString("theme-%1").arg(m_stackThemeIndex + 1), des, true);
    }
}

void MusicBackgroundSkinDialog::findThemeListByPath(const QString &dir, TTKIntList &data)
{
    data.clear();
    const QStringList files(QDir(dir).entryList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name));

    for(const QString &path : qAsConst(files))
    {
        const QFileInfo fin(path);
        if(TTK_FILE_SUFFIX(fin) != TKM_FILE_SUFFIX)
        {
            continue;
        }

        const QStringList &list = fin.baseName().split(TTK_DEFAULT_STR);
        if(list.count() < 2)
        {
            continue;
        }

        const QString &fileName = list.back();
        data << fileName.trimmed().toInt();
    }
}

int MusicBackgroundSkinDialog::cpoyFileToLocalIndex()
{
    TTKIntList data;
    findThemeListByPath(USER_THEME_DIR_FULL, data);
    std::sort(data.begin(), data.end(), std::greater<int>());

    int index = CURRENT_ITEMS_COUNT;
    if(!data.isEmpty())
    {
        index = data.front();
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

    const QString &des = QString("%1theme-%2%3").arg(USER_THEME_DIR_FULL).arg(index + 1).arg(TKM_FILE);
    MusicBackgroundImage image;
    image.m_pix = QPixmap(path);
    return MusicExtractWrapper::inputSkin(&image, des) ? index : -1;
}
