#include "musictopareawidget.h"
#include "ui_musicapplication.h"
#include "musicapplication.h"
#include "musicbackgroundskindialog.h"
#include "musicbackgroundmanager.h"
#include "musicremotewidgetforsquare.h"
#include "musicremotewidgetforrectangle.h"
#include "musicremotewidgetforsimplestyle.h"
#include "musicremotewidgetforcomplexstyle.h"
#include "musicremotewidgetforstrip.h"
#include "musicremotewidgetforripple.h"
#include "musictinyuiobject.h"
#include "musicfunctionuiobject.h"
#include "musictoolsetswidget.h"
#include "musicimageutils.h"

#include <qmmp/visual.h>

MusicTopAreaWidget *MusicTopAreaWidget::m_instance = nullptr;

MusicTopAreaWidget::MusicTopAreaWidget(QWidget *parent)
    : QWidget(parent),
      m_backgroundWidget(nullptr),
      m_remoteWidget(nullptr),
      m_lastRemoteBeforeWallpaper(-1),
      m_backgroundListAlpha(60)
{
    m_instance = this;

    m_pictureCarouselTimer.setInterval(10 * MT_S2MS);
    connect(&m_pictureCarouselTimer, SIGNAL(timeout()), SLOT(musicBackgroundChanged()));
    connect(G_BACKGROUND_PTR, SIGNAL(selectIndexChanged()), SLOT(musicBackgroundChanged()));

    Visual::initialize(MusicApplication::instance());
}

MusicTopAreaWidget::~MusicTopAreaWidget()
{
    delete m_backgroundWidget;
    delete m_remoteWidget;
}

MusicTopAreaWidget *MusicTopAreaWidget::instance()
{
    return m_instance;
}

void MusicTopAreaWidget::setupUi(Ui::MusicApplication *ui)
{
    m_ui = ui;

    musicBackgroundAnimationChanged(true);
    ui->userIcon->setPixmap(TTK::Image::roundedPixmap(QPixmap(":/image/lb_app_logo"), QPixmap(":/image/lb_mask_30"), QSize(30, 30)));

    ui->musicSongSearchEdit->initialize(MusicApplication::instance());
    ui->musicSongSearchEdit->setStyleSheet(TTK::UI::LineEditStyle02);

    ui->musicSearchButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSearchButton->setStyleSheet(TTK::UI::TinyBtnMainSearch);

    ui->musicWindowIdentify->setToolTip(tr("Identify Songs"));
    ui->musicWindowIdentify->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowIdentify->setStyleSheet(TTK::UI::BtnIdentify);

    ui->musicWindowTools->setToolTip(tr("Tools"));
    ui->musicWindowTools->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowTools->setStyleSheet(TTK::UI::BtnTools);
    connect(ui->musicWindowTools, SIGNAL(clicked()), SLOT(musicStackedToolsWidgetChanged()));

    ui->musicWindowChangeSkin->setToolTip(tr("Skin"));
    ui->musicWindowChangeSkin->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowChangeSkin->setStyleSheet(TTK::UI::BtnSkin);
    connect(ui->musicWindowChangeSkin, SIGNAL(clicked()), SLOT(musicShowSkinChangedWindow()));

    ui->musicWindowSetting->setToolTip(tr("Settings"));
    ui->musicWindowSetting->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowSetting->setStyleSheet(TTK::UI::BtnSetting);
    connect(ui->musicWindowSetting, SIGNAL(clicked()), MusicApplication::instance(), SLOT(musicCreateRightMenu()));

    ui->musicWindowConcise->setToolTip(tr("Concise Mode"));
    ui->musicWindowConcise->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowConcise->setStyleSheet(TTK::UI::BtnConciseIn);
    connect(ui->musicWindowConcise, SIGNAL(clicked()), MusicApplication::instance(), SLOT(musicWindowConciseChanged()));

    ui->musicWindowRemote->setToolTip(tr("Open Remote"));
    ui->musicWindowRemote->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowRemote->setStyleSheet(TTK::UI::BtnRemote);
    connect(ui->musicWindowRemote, SIGNAL(clicked()), SLOT(musicSquareRemote()));

    ui->minimization->setStyleSheet(TTK::UI::BtnMinimum);
    ui->minimization->setCursor(QCursor(Qt::PointingHandCursor));
    ui->minimization->setToolTip(tr("Minimization"));
    connect(ui->minimization, SIGNAL(clicked()), MusicApplication::instance(), SLOT(showMinimized()));

    ui->windowClose->setToolTip(tr("Close"));
    ui->windowClose->setCursor(QCursor(Qt::PointingHandCursor));
    ui->windowClose->setStyleSheet(TTK::UI::BtnTClose);
    connect(ui->windowClose, SIGNAL(clicked()), MusicApplication::instance(), SLOT(close()));
}

void MusicTopAreaWidget::setBackgroundParameter()
{
    m_backgroundImagePath = G_SETTING_PTR->value(MusicSettingManager::BackgroundThemeValue).toString();
    m_backgroundListAlpha = G_SETTING_PTR->value(MusicSettingManager::BackgroundListTransparent).toInt();
    m_backgroundAlpha = G_SETTING_PTR->value(MusicSettingManager::BackgroundTransparent).toInt();
}

int MusicTopAreaWidget::backgroundListAlpha()
{
    if(m_backgroundWidget)
    {
        m_backgroundListAlpha = m_backgroundWidget->backgroundListAlpha();
    }
    return m_backgroundListAlpha;
}

bool MusicTopAreaWidget::backgroundTransparentEnable() const
{
    if(m_backgroundWidget)
    {
        return m_backgroundWidget->backgroundTransparentEnable();
    }
    return false;
}

QPixmap MusicTopAreaWidget::rendererPixmap() const
{
    return m_ui->background->rendererPixmap();
}

void MusicTopAreaWidget::setBackgroundAnimation(bool state)
{
    state ? m_pictureCarouselTimer.start() : m_pictureCarouselTimer.stop();
}

void MusicTopAreaWidget::setCurrentPlayStatus(bool status)
{
    if(m_remoteWidget)
    {
        m_remoteWidget->setCurrentPlayStatus(status);
    }
}

void MusicTopAreaWidget::setLabelText(const QString &name) const
{
    if(m_remoteWidget)
    {
        m_remoteWidget->setLabelText(name);
    }
}

void MusicTopAreaWidget::setVolumeValue(int value) const
{
    if(m_remoteWidget)
    {
        m_remoteWidget->setVolumeValue(value);
    }
}

void MusicTopAreaWidget::musicShowSkinChangedWindow()
{
    if(m_backgroundWidget == nullptr)
    {
        m_backgroundWidget = new MusicBackgroundSkinDialog(this);
    }

    m_backgroundWidget->setCurrentBackgroundTheme(m_backgroundImagePath, m_backgroundAlpha, m_backgroundListAlpha);
    m_backgroundWidget->exec();
}

void MusicTopAreaWidget::musicBackgroundTransparentChanged(int value)
{
    if(!isEnableBackground())
    {
        return;
    }

    if(m_backgroundWidget)
    {
        m_backgroundWidget->setSkinTransToolText(value);
    }

    m_backgroundAlpha = value;
    drawWindowBackgroundRectString();
}

void MusicTopAreaWidget::musicBackgroundTransparentChanged(const QString &fileName)
{
    if(!isEnableBackground())
    {
        return;
    }

    drawWindowBackgroundRectString(fileName);
}

void MusicTopAreaWidget::musicSetAsArtistBackground()
{
    QString path = G_BACKGROUND_PTR->artistPhotoDefaultPath();
    if(!path.isEmpty())
    {
        path = MusicBackgroundSkinDialog::cpoyArtistFileToLocal(path);
        if(path.isEmpty())
        {
            return;
        }

        if(m_backgroundWidget)
        {
            m_backgroundWidget->updateArtistFileTheme(path);
        }

        musicBackgroundSkinChanged(path);
    }
}

void MusicTopAreaWidget::musicBackgroundTransparentChanged()
{
    drawWindowBackgroundRect();
}

void MusicTopAreaWidget::musicBackgroundSkinChanged(const QString &fileName)
{
    m_backgroundImagePath = fileName;
    if(isEnableBackground())
    {
        musicBackgroundTransparentChanged();
    }
}

void MusicTopAreaWidget::musicBackgroundChanged()
{
    const QString &path = G_BACKGROUND_PTR->artistPhotoPath();
    if(!path.isEmpty())
    {
        G_BACKGROUND_PTR->photoNext();
        drawWindowBackgroundRectString(path);
    }
    else
    {
        drawWindowBackgroundRect();
    }
}

void MusicTopAreaWidget::musicBackgroundAnimationChanged(bool state)
{
    m_ui->background->setAnimation(state);
}

void MusicTopAreaWidget::musicBackgroundThemeDownloadFinished()
{
    if(!isEnableBackground() && m_ui->musiclrccontainerforinterior->isShowArtistBackground())
    {
        musicBackgroundChanged();
        setBackgroundAnimation(true);
    }
    else
    {
        drawWindowBackgroundRect();
    }
}

void MusicTopAreaWidget::musicBackgroundThemeChangedByResize()
{
    musicBackgroundAnimationChanged(true);
    setBackgroundAnimation(false);
    drawWindowBackgroundRectString();
    musicBackgroundAnimationChanged(false);

    if(!isEnableBackground())
    {
        setBackgroundAnimation(true);
    }
}

void MusicTopAreaWidget::musicPlaylistTransparent(int index)
{
    if(m_backgroundWidget)
    {
        m_backgroundWidget->setListTransToolText(index);
    }

    m_backgroundListAlpha = index;
    backgroundTransparentChanged(m_backgroundListAlpha);
}

void MusicTopAreaWidget::musicSquareRemote()
{
    delete m_remoteWidget;
    m_remoteWidget = new MusicRemoteWidgetForSquare;
    createRemoteWidget();
}

void MusicTopAreaWidget::musicRectangleRemote()
{
    delete m_remoteWidget;
    m_remoteWidget = new MusicRemoteWidgetForRectangle;
    m_remoteWidget->setLabelText(m_ui->showCurrentSong->text());
    createRemoteWidget();
}

void MusicTopAreaWidget::musicSimpleStyleRemote()
{
    delete m_remoteWidget;
    m_remoteWidget = new MusicRemoteWidgetForSimpleStyle;
    m_remoteWidget->setLabelText(m_ui->showCurrentSong->text());
    createRemoteWidget();
}

void MusicTopAreaWidget::musicComplexStyleRemote()
{
    delete m_remoteWidget;
    m_remoteWidget = new MusicRemoteWidgetForComplexStyle;
    m_remoteWidget->setLabelText(m_ui->showCurrentSong->text());
    createRemoteWidget();
}

void MusicTopAreaWidget::musicWallpaperRemote(bool create)
{
    if(create)
    {
        if(m_remoteWidget)
        {
            m_lastRemoteBeforeWallpaper = m_remoteWidget->mapRemoteTypeIndex();
            delete m_remoteWidget;
        }

        m_remoteWidget = new MusicRemoteWidgetForStrip;
        m_remoteWidget->setLabelText(m_ui->showCurrentSong->text());
        createRemoteWidget();
    }
    else
    {
        delete m_remoteWidget;
        m_remoteWidget = nullptr;
        musicRemoteTypeChanged(m_lastRemoteBeforeWallpaper);
        m_lastRemoteBeforeWallpaper = -1;
    }
}

void MusicTopAreaWidget::musicRippleRemote()
{
    delete m_remoteWidget;
    m_remoteWidget = new MusicRemoteWidgetForRipple;
    m_remoteWidget->setLabelText(m_ui->showCurrentSong->text());
    createRemoteWidget();
}

void MusicTopAreaWidget::musicDeleteRemote()
{
    delete m_remoteWidget;
    m_remoteWidget = nullptr;
}

void MusicTopAreaWidget::musicRemoteTypeChanged(QAction *type)
{
    if(!m_remoteWidget)
    {
        return;
    }

    MusicRemoteWidget *remote = m_remoteWidget;
    m_remoteWidget = nullptr;

    musicRemoteTypeChanged(type->data().toInt());

    if(m_remoteWidget)
    {
        remote->deleteLater();
    }
}

void MusicTopAreaWidget::musicRemoteTypeChanged(int type)
{
    switch(type)
    {
        case MusicRemoteWidget::Square: musicSquareRemote(); break;
        case MusicRemoteWidget::Rectangle: musicRectangleRemote(); break;
        case MusicRemoteWidget::SimpleStyle: musicSimpleStyleRemote(); break;
        case MusicRemoteWidget::ComplexStyle: musicComplexStyleRemote(); break;
        case MusicRemoteWidget::Ripple: musicRippleRemote(); break;
        default: break;
    }
}

void MusicTopAreaWidget::musicStackedToolsWidgetChanged()
{
    GENERATE_SINGLE_WIDGET_CLASS(MusicToolSetsWidget);
}

void MusicTopAreaWidget::backgroundTransparentChanged(int value)
{
    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundListTransparent, value);
    m_ui->centerLeftWidget->backgroundTransparent(value);
}

bool MusicTopAreaWidget::isEnableBackground() const
{
    return m_ui->functionsContainer->currentIndex() != MUSIC_LRC_PAGE;
}

void MusicTopAreaWidget::createRemoteWidget()
{
    if(!m_remoteWidget)
    {
        return;
    }

    m_remoteWidget->setCurrentPlayStatus(MusicApplication::instance()->isPlaying());
    m_remoteWidget->setVolumeValue(m_ui->musicSound->value());

    connect(m_remoteWidget, SIGNAL(musicWindowChanged()), MusicApplication::instance(), SLOT(showNormal()));
    connect(m_remoteWidget, SIGNAL(musicPlayPreviousChanged()), MusicApplication::instance(), SLOT(playPrevious()));
    connect(m_remoteWidget, SIGNAL(musicPlayNextChanged()), MusicApplication::instance(), SLOT(playNext()));
    connect(m_remoteWidget, SIGNAL(musicKeyChanged()), MusicApplication::instance(), SLOT(playState()));
    connect(m_remoteWidget, SIGNAL(musicSettingChanged()), MusicApplication::instance(), SLOT(musicSetting()));
    connect(m_remoteWidget, SIGNAL(volumeChanged(int)), MusicApplication::instance(), SLOT(volumeChanged(int)));
    connect(m_remoteWidget, SIGNAL(musicRemoteTypeChanged(QAction*)), SLOT(musicRemoteTypeChanged(QAction*)));
    m_remoteWidget->show();
}

void MusicTopAreaWidget::drawWindowBackgroundRect()
{
    setBackgroundAnimation(false);
    drawWindowBackgroundRect(MusicBackgroundSkinDialog::setBackgroundUrl(m_backgroundImagePath).toImage());
}

void MusicTopAreaWidget::drawWindowBackgroundRect(const QImage &image)
{
    if(image.isNull())
    {
        return;
    }

    QImage origin(image);
    if(!origin.colorTable().isEmpty())
    {
        origin = origin.convertToFormat(QImage::Format_ARGB32);
    }
    TTK::Image::reRenderImage(35, &origin, &origin);

    m_backgroundImage = origin;
    drawWindowBackgroundRectString();
}

void MusicTopAreaWidget::drawWindowBackgroundRectString()
{
    if(m_backgroundImage.isNull())
    {
        return;
    }

    const float v = TTK::Image::reRenderValue<float>(1, 0.35, MV_MAX - m_backgroundAlpha);
    MusicApplication::instance()->setWindowOpacity(v);

    const QSize size(G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize());

    QPixmap pix(size);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    painter.drawPixmap(0, 0, QPixmap::fromImage(m_backgroundImage.scaled(size, Qt::KeepAspectRatioByExpanding)));

    backgroundTransparentChanged(m_backgroundListAlpha);
    Q_EMIT backgroundPixmapChanged(pix);

    m_ui->background->setPixmap(pix);
}

void MusicTopAreaWidget::drawWindowBackgroundRectString(const QString &path)
{
    drawWindowBackgroundRect(QImage(path));
}
