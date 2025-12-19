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

MusicTopAreaWidget *MusicTopAreaWidget::m_instance = nullptr;

MusicTopAreaWidget::MusicTopAreaWidget(QWidget *parent)
    : QWidget(parent),
      m_backgroundWidget(nullptr),
      m_remoteWidget(nullptr),
      m_lastRemoteBeforeWallpaper(-1),
      m_backgroundListTransparent(60)
{
    m_instance = this;

    m_pictureCarouselTimer.setInterval(10 * TTK_DN_S2MS);
    connect(&m_pictureCarouselTimer, SIGNAL(timeout()), SLOT(backgroundChanged()));
    connect(G_BACKGROUND_PTR, SIGNAL(selectIndexChanged()), SLOT(backgroundChanged()));
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
    //
    backgroundAnimationChanged(true);

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
    connect(ui->musicWindowTools, SIGNAL(clicked()), SLOT(showToolSetsWidget()));

    ui->musicWindowChangeSkin->setToolTip(tr("Skin"));
    ui->musicWindowChangeSkin->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowChangeSkin->setStyleSheet(TTK::UI::BtnSkin);
    connect(ui->musicWindowChangeSkin, SIGNAL(clicked()), SLOT(showSkinManagerWidget()));

    ui->musicWindowSetting->setToolTip(tr("Settings"));
    ui->musicWindowSetting->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowSetting->setStyleSheet(TTK::UI::BtnSetting);
    connect(ui->musicWindowSetting, SIGNAL(clicked()), MusicApplication::instance(), SLOT(createRightMenu()));

    ui->musicWindowConcise->setToolTip(tr("Concise Mode"));
    ui->musicWindowConcise->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowConcise->setStyleSheet(TTK::UI::BtnConciseIn);
    connect(ui->musicWindowConcise, SIGNAL(clicked()), MusicApplication::instance(), SLOT(windowConciseChanged()));

    ui->musicWindowRemote->setToolTip(tr("Open Remote"));
    ui->musicWindowRemote->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowRemote->setStyleSheet(TTK::UI::BtnRemote);
    connect(ui->musicWindowRemote, SIGNAL(clicked()), SLOT(showSquareRemote()));

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
    m_backgroundTransparent = G_SETTING_PTR->value(MusicSettingManager::BackgroundTransparent).toInt();
    m_backgroundListTransparent = G_SETTING_PTR->value(MusicSettingManager::BackgroundListTransparent).toInt();
}

int MusicTopAreaWidget::backgroundListTransparent()
{
    if(m_backgroundWidget)
    {
        m_backgroundListTransparent = m_backgroundWidget->backgroundListTransparent();
    }
    return m_backgroundListTransparent;
}

bool MusicTopAreaWidget::backgroundTransparentEnabled() const
{
    if(m_backgroundWidget)
    {
        return m_backgroundWidget->backgroundTransparentEnabled();
    }
    return false;
}

const QImage& MusicTopAreaWidget::originImage() const
{
    return m_backgroundImage;
}

const QPixmap& MusicTopAreaWidget::renderPixmap() const
{
    return m_ui->background->renderPixmap();
}

void MusicTopAreaWidget::setBackgroundAnimation(bool state)
{
    state ? m_pictureCarouselTimer.start() : m_pictureCarouselTimer.stop();
}

void MusicTopAreaWidget::setCurrentPlayState(bool state) const
{
    if(m_remoteWidget)
    {
        m_remoteWidget->setCurrentPlayState(state);
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

void MusicTopAreaWidget::showSkinManagerWidget()
{
    if(m_backgroundWidget == nullptr)
    {
        m_backgroundWidget = new MusicBackgroundSkinDialog(this);
    }

    m_backgroundWidget->setCurrentBackgroundTheme(m_backgroundImagePath, m_backgroundTransparent, m_backgroundListTransparent);
    m_backgroundWidget->exec();
}

void MusicTopAreaWidget::backgroundTransparentChanged(int value)
{
    if(isArtistBackground())
    {
        return;
    }

    if(m_backgroundWidget)
    {
        m_backgroundWidget->setSkinTransToolText(value);
    }

    m_backgroundTransparent = value;
    drawWindowBackgroundByImage();
}

void MusicTopAreaWidget::backgroundTransparentChanged(const QString &fileName)
{
    if(isArtistBackground())
    {
        return;
    }

    drawWindowBackgroundByPath(fileName);
}

void MusicTopAreaWidget::setArtistBackground()
{
    QString path = G_BACKGROUND_PTR->artistImageDefaultPath();
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

        backgroundSkinChanged(path);
    }
}

void MusicTopAreaWidget::backgroundTransparentChanged()
{
    drawWindowBackground();
}

void MusicTopAreaWidget::backgroundSkinChanged(const QString &fileName)
{
    m_backgroundImagePath = fileName;
    if(!isArtistBackground())
    {
        backgroundTransparentChanged();
    }
}

void MusicTopAreaWidget::backgroundChanged()
{
    const QString &path = G_BACKGROUND_PTR->artistImagePath();
    if(!path.isEmpty())
    {
        G_BACKGROUND_PTR->imageNext();
        drawWindowBackgroundByPath(path);
    }
    else
    {
        drawWindowBackground();
    }
}

void MusicTopAreaWidget::backgroundAnimationChanged(bool state)
{
    m_ui->background->setAnimation(!state);
}

void MusicTopAreaWidget::backgroundThemeDownloadFinished()
{
    if(isArtistBackground() && m_ui->musiclrccontainerforinterior->isShowArtistBackground())
    {
        backgroundChanged();
        setBackgroundAnimation(true);
    }
    else
    {
        drawWindowBackground();
    }
}

void MusicTopAreaWidget::backgroundThemeChangedByResize()
{
    backgroundAnimationChanged(true);
    setBackgroundAnimation(false);
    drawWindowBackgroundByImage();
    backgroundAnimationChanged(false);

    if(isArtistBackground())
    {
        setBackgroundAnimation(true);
    }
}

void MusicTopAreaWidget::playlistTransparentChanged(int index)
{
    if(m_backgroundWidget)
    {
        m_backgroundWidget->setListTransToolText(index);
    }

    m_backgroundListTransparent = index;
    backgroundTransparent(m_backgroundListTransparent);
}

void MusicTopAreaWidget::showSquareRemote()
{
    delete m_remoteWidget;
    m_remoteWidget = new MusicRemoteWidgetForSquare;
    createRemoteWidget();
}

void MusicTopAreaWidget::showRectangleRemote()
{
    delete m_remoteWidget;
    m_remoteWidget = new MusicRemoteWidgetForRectangle;
    m_remoteWidget->setLabelText(m_ui->musicSongTitle->text());
    createRemoteWidget();
}

void MusicTopAreaWidget::showSimpleStyleRemote()
{
    delete m_remoteWidget;
    m_remoteWidget = new MusicRemoteWidgetForSimpleStyle;
    m_remoteWidget->setLabelText(m_ui->musicSongTitle->text());
    createRemoteWidget();
}

void MusicTopAreaWidget::showComplexStyleRemote()
{
    delete m_remoteWidget;
    m_remoteWidget = new MusicRemoteWidgetForComplexStyle;
    m_remoteWidget->setLabelText(m_ui->musicSongTitle->text());
    createRemoteWidget();
}

void MusicTopAreaWidget::showRippleRemote()
{
    delete m_remoteWidget;
    m_remoteWidget = new MusicRemoteWidgetForRipple;
    m_remoteWidget->setLabelText(m_ui->musicSongTitle->text());
    createRemoteWidget();
}

void MusicTopAreaWidget::showWallpaperRemote(bool create)
{
    if(create)
    {
        if(m_remoteWidget)
        {
            m_lastRemoteBeforeWallpaper = m_remoteWidget->mapRemoteTypeIndex();
            delete m_remoteWidget;
        }

        m_remoteWidget = new MusicRemoteWidgetForStrip;
        m_remoteWidget->setLabelText(m_ui->musicSongTitle->text());
        createRemoteWidget();
    }
    else
    {
        delete m_remoteWidget;
        m_remoteWidget = nullptr;
        remoteTypeChanged(m_lastRemoteBeforeWallpaper);
        m_lastRemoteBeforeWallpaper = -1;
    }
}

void MusicTopAreaWidget::deleteCurrentRemote()
{
    delete m_remoteWidget;
    m_remoteWidget = nullptr;
}

void MusicTopAreaWidget::remoteTypeChanged(QAction *type)
{
    if(!m_remoteWidget)
    {
        return;
    }

    MusicRemoteWidget *remote = m_remoteWidget;
    m_remoteWidget = nullptr;

    remoteTypeChanged(type->data().toInt());

    if(m_remoteWidget)
    {
        remote->deleteLater();
    }
}

void MusicTopAreaWidget::remoteTypeChanged(int type)
{
    switch(type)
    {
        case MusicRemoteWidget::Square: showSquareRemote(); break;
        case MusicRemoteWidget::Rectangle: showRectangleRemote(); break;
        case MusicRemoteWidget::SimpleStyle: showSimpleStyleRemote(); break;
        case MusicRemoteWidget::ComplexStyle: showComplexStyleRemote(); break;
        case MusicRemoteWidget::Ripple: showRippleRemote(); break;
        default: break;
    }
}

void MusicTopAreaWidget::showToolSetsWidget()
{
    TTK_GENERATE_SINGLE_WIDGET(MusicToolSetsWidget);
}

void MusicTopAreaWidget::backgroundTransparent(int value)
{
    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundListTransparent, value);
    m_ui->centerLeftWidget->backgroundTransparent(value);
}

bool MusicTopAreaWidget::isArtistBackground() const
{
    return m_ui->functionsContainer->currentIndex() == MUSIC_LRC_PAGE;
}

void MusicTopAreaWidget::createRemoteWidget()
{
    if(!m_remoteWidget)
    {
        return;
    }

    m_remoteWidget->setCurrentPlayState(MusicApplication::instance()->isPlaying());
    m_remoteWidget->setVolumeValue(m_ui->musicSound->value());

    connect(m_remoteWidget, SIGNAL(showMainWindow()), MusicApplication::instance(), SLOT(showNormal()));
    connect(m_remoteWidget, SIGNAL(playPreviousChanged()), MusicApplication::instance(), SLOT(playPrevious()));
    connect(m_remoteWidget, SIGNAL(playNextChanged()), MusicApplication::instance(), SLOT(playNext()));
    connect(m_remoteWidget, SIGNAL(playStateChanged()), MusicApplication::instance(), SLOT(switchToPlayState()));
    connect(m_remoteWidget, SIGNAL(settingChanged()), MusicApplication::instance(), SLOT(showSettingWidget()));
    connect(m_remoteWidget, SIGNAL(volumeChanged(int)), MusicApplication::instance(), SLOT(volumeChanged(int)));
    connect(m_remoteWidget, SIGNAL(remoteTypeChanged(QAction*)), SLOT(remoteTypeChanged(QAction*)));
    m_remoteWidget->show();
}

void MusicTopAreaWidget::drawWindowBackground()
{
    setBackgroundAnimation(false);
    drawWindowBackground(MusicBackgroundSkinDialog::setBackgroundUrl(m_backgroundImagePath).toImage());
}

void MusicTopAreaWidget::drawWindowBackground(const QImage &image)
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
    drawWindowBackgroundByImage();
}

void MusicTopAreaWidget::drawWindowBackgroundByImage()
{
    if(m_backgroundImage.isNull())
    {
        return;
    }

    const float v = TTK::Image::boundValue<float>(1.0f, 0.35f, TTK_RN_MAX - m_backgroundTransparent);
    MusicApplication::instance()->setWindowOpacity(v);

    const QSize size(G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize());

    QPixmap pix(size);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    painter.drawImage(0, 0, m_backgroundImage.scaled(size, Qt::KeepAspectRatioByExpanding));

    backgroundTransparent(m_backgroundListTransparent);
    Q_EMIT originImageChanged(m_backgroundImage);

    m_ui->background->setPixmap(pix);
}

void MusicTopAreaWidget::drawWindowBackgroundByPath(const QString &path)
{
    drawWindowBackground(QImage(path));
}
