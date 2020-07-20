#include "musictopareawidget.h"
#include "ui_musicapplication.h"
#include "musicapplication.h"
#include "musicuserwindow.h"
#include "musicbackgroundskindialog.h"
#include "musicbackgroundmanager.h"
#include "musicremotewidgetforsquare.h"
#include "musicremotewidgetforrectangle.h"
#include "musicremotewidgetforsimplestyle.h"
#include "musicremotewidgetforcomplexstyle.h"
#include "musicremotewidgetforstrip.h"
#include "musicremotewidgetforripple.h"
#include "musicuiobject.h"
#include "musictinyuiobject.h"
#include "musicfunctionuiobject.h"
#include "musictoolsetswidget.h"
#include "musicsinglemanager.h"
#include "musicimageutils.h"

///qmmp incldue
#include "visual.h"

MusicTopAreaWidget *MusicTopAreaWidget::m_instance = nullptr;

MusicTopAreaWidget::MusicTopAreaWidget(QWidget *parent)
    : QWidget(parent)
{
    m_instance = this;
    m_musicUserWindow = nullptr;
    m_musicBackgroundWidget = nullptr;
    m_musicRemoteWidget = nullptr;

    m_pictureCarouselTimer.setInterval(10 * MT_S2MS);
    connect(&m_pictureCarouselTimer, SIGNAL(timeout()), SLOT(musicBackgroundChanged()));
    connect(M_BACKGROUND_PTR, SIGNAL(userSelectIndexChanged()), SLOT(musicBackgroundChanged()));

    m_backgroundAListlpha = 40;
    m_lastRemoteBeforeWallpaper = -1;

    Visual::initialize(MusicApplication::instance());
}

MusicTopAreaWidget::~MusicTopAreaWidget()
{
    delete m_musicUserWindow;
    delete m_musicBackgroundWidget;
    delete m_musicRemoteWidget;
}

MusicTopAreaWidget *MusicTopAreaWidget::instance()
{
    return m_instance;
}

void MusicTopAreaWidget::setupUi(Ui::MusicApplication* ui)
{
    m_ui = ui;

    musicBackgroundAnimationChanged(true);
    m_musicUserWindow = new MusicUserWindow(this);
    ui->userWindow->addWidget(m_musicUserWindow);

    ui->musicSongSearchEdit->initWidget(MusicApplication::instance());
    ui->musicSongSearchEdit->setStyleSheet(MusicUIObject::MQSSLineEditStyle03);

    ui->musicSearchButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSearchButton->setStyleSheet(MusicUIObject::MQSSTinyBtnMainSearch);

    ui->musicWindowIdentify->setToolTip(tr("identify"));
    ui->musicWindowIdentify->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowIdentify->setStyleSheet(MusicUIObject::MQSSBtnIdentify);

    ui->musicWindowTools->setToolTip(tr("tools"));
    ui->musicWindowTools->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowTools->setStyleSheet(MusicUIObject::MQSSBtnTools);
    connect(ui->musicWindowTools, SIGNAL(clicked()), SLOT(musicStackedToolsWidgetChanged()));

    ui->musicWindowChangeSkin->setToolTip(tr("changeskin"));
    ui->musicWindowChangeSkin->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowChangeSkin->setStyleSheet(MusicUIObject::MQSSBtnSkin);
    connect(ui->musicWindowChangeSkin, SIGNAL(clicked()), SLOT(musicShowSkinChangedWindow()));

    ui->musicWindowSetting->setToolTip(tr("setting"));
    ui->musicWindowSetting->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowSetting->setStyleSheet(MusicUIObject::MQSSBtnSetting);
    connect(ui->musicWindowSetting, SIGNAL(clicked()), MusicApplication::instance(), SLOT(musicCreateRightMenu()));

    ui->musicWindowConcise->setToolTip(tr("concisein/out"));
    ui->musicWindowConcise->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowConcise->setStyleSheet(MusicUIObject::MQSSBtnConciseIn);
    connect(ui->musicWindowConcise, SIGNAL(clicked()), MusicApplication::instance(), SLOT(musicWindowConciseChanged()));

    ui->musicWindowRemote->setToolTip(tr("remoteWindow"));
    ui->musicWindowRemote->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowRemote->setStyleSheet(MusicUIObject::MQSSBtnRemote);
    connect(ui->musicWindowRemote, SIGNAL(clicked()), SLOT(musicSquareRemote()));

    ui->minimization->setStyleSheet(MusicUIObject::MQSSBtnMinimum);
    ui->minimization->setCursor(QCursor(Qt::PointingHandCursor));
    ui->minimization->setToolTip(tr("Minimization"));
    connect(ui->minimization, SIGNAL(clicked()), MusicApplication::instance(), SLOT(showMinimized()));

    ui->windowClose->setToolTip(tr("Close"));
    ui->windowClose->setCursor(QCursor(Qt::PointingHandCursor));
    ui->windowClose->setStyleSheet(MusicUIObject::MQSSBtnTClose);
    connect(ui->windowClose, SIGNAL(clicked()), MusicApplication::instance(), SLOT(close()));
}

void MusicTopAreaWidget::setBackgroundParams(const QString &skin, int alpha, int list)
{
    m_backgroundImagePath = skin;
    m_backgroundAListlpha = list;
    m_backgroundAlpha = alpha;

    drawWindowBackgroundRect();
}

int MusicTopAreaWidget::getBackgroundListAlpha()
{
    if(m_musicBackgroundWidget)
    {
        m_backgroundAListlpha = m_musicBackgroundWidget->getBackgroundListAlpha();
    }
    return m_backgroundAListlpha;
}

bool MusicTopAreaWidget::getBackgroundTransparentEnable() const
{
    if(m_musicBackgroundWidget)
    {
        return m_musicBackgroundWidget->getBackgroundTransparentEnable();
    }
    return false;
}

QPixmap MusicTopAreaWidget::getRendererPixmap() const
{
    return m_ui->background->getRendererPixmap();
}

bool MusicTopAreaWidget::getUserLoginState() const
{
    return m_musicUserWindow->isUserLogin();
}

void MusicTopAreaWidget::setBackgroundAnimation(bool state)
{
    state ? m_pictureCarouselTimer.start() : m_pictureCarouselTimer.stop();
}

void MusicTopAreaWidget::setCurrentPlayStatus(bool status)
{
    if(m_musicRemoteWidget)
    {
        m_musicRemoteWidget->setCurrentPlayStatus(status);
    }
}

void MusicTopAreaWidget::setLabelText(const QString &name) const
{
    if(m_musicRemoteWidget)
    {
        m_musicRemoteWidget->setLabelText(name);
    }
}

void MusicTopAreaWidget::setVolumeValue(int value) const
{
    if(m_musicRemoteWidget)
    {
        m_musicRemoteWidget->setVolumeValue(value);
    }
}

void MusicTopAreaWidget::musicShowSkinChangedWindow()
{
    if(m_musicBackgroundWidget == nullptr)
    {
        m_musicBackgroundWidget = new MusicBackgroundSkinDialog(this);
    }
    m_musicBackgroundWidget->setCurrentBackgroundTheme(m_backgroundImagePath, m_backgroundAlpha, m_backgroundAListlpha);
    m_musicBackgroundWidget->exec();
}

void MusicTopAreaWidget::musicUserContextLogin()
{
    m_musicUserWindow->musicUserContextLogin();
}

void MusicTopAreaWidget::musicBackgroundTransparentChanged(int index)
{
    if(m_ui->functionsContainer->currentIndex() == APP_WINDOW_INDEX_1)
    {
        return;
    }

    if(m_musicBackgroundWidget)
    {
        m_musicBackgroundWidget->setSkinTransToolText(index);
    }
    m_backgroundAlpha = index;
    drawWindowBackgroundRectString();
}

void MusicTopAreaWidget::musicBackgroundTransparentChanged(const QString &fileName)
{
    if(m_ui->functionsContainer->currentIndex() == APP_WINDOW_INDEX_1)
    {
        return;
    }

    drawWindowBackgroundRectString(fileName);
}

void MusicTopAreaWidget::musicSetAsArtistBackground()
{
    QString path = M_BACKGROUND_PTR->getArtistPhotoPathNoIndex();
    if(!path.isEmpty())
    {
        path = MusicBackgroundSkinDialog::cpoyArtistFileToLocal(path);
        if(path.isEmpty())
        {
            return;
        }

        if(m_musicBackgroundWidget)
        {
            m_musicBackgroundWidget->updateArtistFileTheme(path);
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
    musicBackgroundTransparentChanged();
}

void MusicTopAreaWidget::musicBackgroundSkinCustumChanged(const QString &fileName)
{
    if(m_ui->functionsContainer->currentIndex() == APP_WINDOW_INDEX_1)
    {
        return;
    }

    musicBackgroundSkinChanged(fileName);
}

void MusicTopAreaWidget::musicBackgroundChanged()
{
    const QString &artistPath = M_BACKGROUND_PTR->getArtistPhotoPath();
    if(!artistPath.isEmpty())
    {
        M_BACKGROUND_PTR->indexIncrease();
        drawWindowBackgroundRectString(artistPath);
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
    if(m_ui->functionsContainer->currentIndex() == APP_WINDOW_INDEX_1 && m_ui->musiclrccontainerforinterior->artistBackgroundIsShow())
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
    ///
    drawWindowBackgroundRectString();
    ///
    musicBackgroundAnimationChanged(false);
    setBackgroundAnimation(true);
}

void MusicTopAreaWidget::musicPlaylistTransparent(int index)
{
    if(m_musicBackgroundWidget)
    {
        m_musicBackgroundWidget->setListTransToolText(index);
    }
    Q_EMIT setTransparent(m_backgroundAListlpha = index);
}

void MusicTopAreaWidget::musicSquareRemote()
{
    if(m_musicRemoteWidget)
    {
        delete m_musicRemoteWidget;
    }
    m_musicRemoteWidget = new MusicRemoteWidgetForSquare;
    createRemoteWidget();
}

void MusicTopAreaWidget::musicRectangleRemote()
{
    if(m_musicRemoteWidget)
    {
        delete m_musicRemoteWidget;
    }
    m_musicRemoteWidget = new MusicRemoteWidgetForRectangle;
    m_musicRemoteWidget->setLabelText(m_ui->showCurrentSong->text());
    createRemoteWidget();
}

void MusicTopAreaWidget::musicSimpleStyleRemote()
{
    if(m_musicRemoteWidget)
    {
        delete m_musicRemoteWidget;
    }
    m_musicRemoteWidget = new MusicRemoteWidgetForSimpleStyle;
    m_musicRemoteWidget->setLabelText(m_ui->showCurrentSong->text());
    createRemoteWidget();
}

void MusicTopAreaWidget::musicComplexStyleRemote()
{
    if(m_musicRemoteWidget)
    {
        delete m_musicRemoteWidget;
    }
    m_musicRemoteWidget = new MusicRemoteWidgetForComplexStyle;
    m_musicRemoteWidget->setLabelText(m_ui->showCurrentSong->text());
    createRemoteWidget();
}

void MusicTopAreaWidget::musicWallpaperRemote(bool create)
{
    if(create)
    {
        if(m_musicRemoteWidget)
        {
            m_lastRemoteBeforeWallpaper = m_musicRemoteWidget->mapRemoteTypeIndex();
            delete m_musicRemoteWidget;
        }
        m_musicRemoteWidget = new MusicRemoteWidgetForStrip;
        m_musicRemoteWidget->setLabelText(m_ui->showCurrentSong->text());
        createRemoteWidget();
    }
    else
    {
        delete m_musicRemoteWidget;
        m_musicRemoteWidget = nullptr;
        musicRemoteTypeChanged(m_lastRemoteBeforeWallpaper);
        m_lastRemoteBeforeWallpaper = -1;
    }
}

void MusicTopAreaWidget::musicRippleRemote()
{
    if(m_musicRemoteWidget)
    {
        delete m_musicRemoteWidget;
    }
    m_musicRemoteWidget = new MusicRemoteWidgetForRipple;
    m_musicRemoteWidget->setLabelText(m_ui->showCurrentSong->text());
    createRemoteWidget();
}

void MusicTopAreaWidget::musicDeleteRemote()
{
    delete m_musicRemoteWidget;
    m_musicRemoteWidget = nullptr;
}

void MusicTopAreaWidget::musicRemoteTypeChanged(QAction *type)
{
    if(!m_musicRemoteWidget)
    {
        return;
    }

    MusicRemoteWidget *tempRemote = m_musicRemoteWidget;
    m_musicRemoteWidget = nullptr;

    musicRemoteTypeChanged(type->data().toInt());

    if(m_musicRemoteWidget)
    {
        tempRemote->deleteLater();
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
    M_SINGLE_MANAGER_WIDGET_CLASS(MusicToolSetsWidget);
}

void MusicTopAreaWidget::createRemoteWidget()
{
    if(!m_musicRemoteWidget)
    {
        return;
    }

    m_musicRemoteWidget->setCurrentPlayStatus(!MusicApplication::instance()->isPlaying());
    m_musicRemoteWidget->setVolumeValue(m_ui->musicSound->value());
    connect(m_musicRemoteWidget, SIGNAL(musicWindowChanged()), MusicApplication::instance(), SLOT(showNormal()));
    connect(m_musicRemoteWidget, SIGNAL(musicPlayPreviousChanged()), MusicApplication::instance(), SLOT(musicPlayPrevious()));
    connect(m_musicRemoteWidget, SIGNAL(musicPlayNextChanged()), MusicApplication::instance(), SLOT(musicPlayNext()));
    connect(m_musicRemoteWidget, SIGNAL(musicKeyChanged()), MusicApplication::instance(), SLOT(musicStatePlay()));
    connect(m_musicRemoteWidget, SIGNAL(musicSettingChanged()), MusicApplication::instance(), SLOT(musicSetting()));
    connect(m_musicRemoteWidget, SIGNAL(musicVolumeChanged(int)), MusicApplication::instance(), SLOT(musicVolumeChanged(int)));
    connect(m_musicRemoteWidget, SIGNAL(musicRemoteTypeChanged(QAction*)), SLOT(musicRemoteTypeChanged(QAction*)));
    m_musicRemoteWidget->show();
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
    MusicUtils::Image::reRenderImage(35, &origin, &origin);

    m_backgroundImage = origin;
    drawWindowBackgroundRectString();
}

void MusicTopAreaWidget::drawWindowBackgroundRectString()
{
    const float v = MusicUtils::Image::reRenderValue<float>(1, 0.35, m_backgroundAlpha);
    MusicApplication::instance()->setWindowOpacity(v);

    const QSize size(M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize());
    QPixmap after(size);
    after.fill(Qt::transparent);
    QPainter paint(&after);
    paint.drawPixmap(0, 0, QPixmap::fromImage(m_backgroundImage.scaled(size, Qt::KeepAspectRatioByExpanding)));
    paint.end();

    Q_EMIT setTransparent(m_backgroundAListlpha);
    Q_EMIT backgroundPixmapChanged(after);

    m_ui->background->setPixmap(after);
}

void MusicTopAreaWidget::drawWindowBackgroundRectString(const QString &path)
{
    drawWindowBackgroundRect(QImage(path));
}
