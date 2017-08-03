#include "musictopareawidget.h"
#include "ui_musicapplication.h"
#include "musicapplication.h"
#include "musicuserwindow.h"
#include "musicbackgroundskindialog.h"
#include "musicbackgroundmanager.h"
#include "musicremotewidgetforsquare.h"
#include "musicremotewidgetforrectangle.h"
#include "musicremotewidgetfordiamond.h"
#include "musicremotewidgetforcircle.h"
#include "musicremotewidgetforsimplestyle.h"
#include "musicremotewidgetforcomplexstyle.h"
#include "musicremotewidgetforstrip.h"
#include "musicremotewidgetforripples.h"
#include "musicuiobject.h"
#include "musictinyuiobject.h"
#include "musicfunctionuiobject.h"
#include "musicdownloaddiscoverlistthread.h"
#include "musicdownloadqueryfactory.h"
#include "musicdownloadcounterpvthread.h"

MusicTopAreaWidget *MusicTopAreaWidget::m_instance = nullptr;

MusicTopAreaWidget::MusicTopAreaWidget(QWidget *parent)
    : QWidget(parent), m_musicBackgroundWidget(nullptr), m_musicRemoteWidget(nullptr)
{
    m_instance = this;
    m_musicUserWindow = new MusicUserWindow(this);

    m_pictureCarouselTimer.setInterval(10*MT_S2MS);
    connect(&m_pictureCarouselTimer, SIGNAL(timeout()), SLOT(musicBackgroundChanged()));
    connect(M_BACKGROUND_PTR, SIGNAL(userSelectIndexChanged()), SLOT(musicBackgroundChanged()));
    ///////////////////////////////////////////////////////
    MusicDownLoadDiscoverListThread *discover = M_DOWNLOAD_QUERY_PTR->getDiscoverListThread(this);
    connect(discover, SIGNAL(downLoadDataChanged(QString)), SLOT(musicSearchTopListInfoFinished(QString)));
    discover->startToSearch();

    m_counterPVThread = new MusicDownloadCounterPVThread(this);
    m_counterPVThread->startToDownload();
    ///////////////////////////////////////////////////////
    m_backgroundAListlpha = 40;
    m_lastRemoteBeforeWallpaper = -1;
}

MusicTopAreaWidget::~MusicTopAreaWidget()
{
    delete m_musicUserWindow;
    delete m_musicBackgroundWidget;
    delete m_musicRemoteWidget;
    delete m_counterPVThread;
}

QString MusicTopAreaWidget::getClassName()
{
    return staticMetaObject.className();
}

MusicTopAreaWidget *MusicTopAreaWidget::instance()
{
    return m_instance;
}

void MusicTopAreaWidget::setupUi(Ui::MusicApplication* ui)
{
    m_ui = ui;
    ui->background->setNoAnimation(true);
    ui->userWindow->addWidget(m_musicUserWindow);

    ui->musicSongSearchLine->initWidget(MusicApplication::instance());
    ui->musicSongSearchLine->setStyleSheet(MusicUIObject::MLineEditStyle03);

    ui->musicSearchButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSearchButton->setStyleSheet(MusicUIObject::MKGTinyBtnMainSearch);

    ui->musicWindowChangeSkin->setToolTip(tr("changeskin"));
    ui->musicWindowChangeSkin->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowChangeSkin->setStyleSheet(MusicUIObject::MKGBtnSkin);
    connect(ui->musicWindowChangeSkin, SIGNAL(clicked()) , SLOT(musicShowSkinChangedWindow()));

    ui->musicWindowSetting->setToolTip(tr("setting"));
    ui->musicWindowSetting->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowSetting->setStyleSheet(MusicUIObject::MKGBtnSetting);
    connect(ui->musicWindowSetting, SIGNAL(clicked()), MusicApplication::instance(), SLOT(musicCreateRightMenu()));

    ui->musicWindowConcise->setToolTip(tr("concisein/out"));
    ui->musicWindowConcise->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowConcise->setStyleSheet(MusicUIObject::MKGBtnConciseIn);
    connect(ui->musicWindowConcise, SIGNAL(clicked()), MusicApplication::instance(), SLOT(musicWindowConciseChanged()));

    ui->musicWindowRemote->setToolTip(tr("remoteWindow"));
    ui->musicWindowRemote->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowRemote->setStyleSheet(MusicUIObject::MKGBtnRemote);
    connect(ui->musicWindowRemote, SIGNAL(clicked()), SLOT(musicSquareRemote()));

    ui->minimization->setStyleSheet(MusicUIObject::MKGBtnMinimum);
    ui->minimization->setCursor(QCursor(Qt::PointingHandCursor));
    ui->minimization->setToolTip(tr("Minimization"));
    connect(ui->minimization, SIGNAL(clicked()), MusicApplication::instance(), SLOT(hide()));

    ui->windowClose->setToolTip(tr("Close"));
    ui->windowClose->setCursor(QCursor(Qt::PointingHandCursor));
    ui->windowClose->setStyleSheet(MusicUIObject::MKGBtnTClose);
    connect(ui->windowClose, SIGNAL(clicked()), MusicApplication::instance(), SLOT(close()));
}

void MusicTopAreaWidget::setParameters(const QString &skin, int alpha, int list)
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

QPixmap MusicTopAreaWidget::getRendererPixmap() const
{
    return m_ui->background->getRendererPixmap();
}

bool MusicTopAreaWidget::getUserLoginState() const
{
    return m_musicUserWindow->isUserLogin();
}

void MusicTopAreaWidget::setTimerStop()
{
    m_pictureCarouselTimer.stop();
}

void MusicTopAreaWidget::showPlayStatus(bool status)
{
    if(m_musicRemoteWidget)
    {
        m_musicRemoteWidget->showPlayStatus(status);
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

void MusicTopAreaWidget::musicSearchTopListInfoFinished(const QString &data)
{
    m_ui->musicSongSearchLine->setPlaceholderText( data );
}

void MusicTopAreaWidget::musicShowSkinChangedWindow()
{
    if(m_musicBackgroundWidget == nullptr)
    {
        m_musicBackgroundWidget = new MusicBackgroundSkinDialog(this);
    }
    m_musicBackgroundWidget->setCurrentBgTheme(m_backgroundImagePath, m_backgroundAlpha, m_backgroundAListlpha);
    m_musicBackgroundWidget->exec();
}

void MusicTopAreaWidget::musicUserContextLogin()
{
    m_musicUserWindow->musicUserContextLogin();
}

void MusicTopAreaWidget::musicBgTransparentChanged(int index)
{
    if(m_ui->surfaceStackedWidget->currentIndex() == 1)
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

void MusicTopAreaWidget::musicBgTransparentChanged(const QString &fileName)
{
    if(m_ui->surfaceStackedWidget->currentIndex() == 1)
    {
        return;
    }

    drawWindowBackgroundRectString(fileName);
}

void MusicTopAreaWidget::musicSetAsArtBackground()
{
    QString path = M_BACKGROUND_PTR->getArtPhotoPathNoIndex();
    if(!path.isEmpty())
    {
        path = MusicBackgroundSkinDialog::cpoyArtFileToLocal(path);
        if(m_musicBackgroundWidget)
        {
            m_musicBackgroundWidget->updateArtFileTheme(path);
        }
        musicBackgroundSkinChanged(path);
    }
}

void MusicTopAreaWidget::musicBgTransparentChanged()
{
    drawWindowBackgroundRect();
}

void MusicTopAreaWidget::musicBackgroundSkinChanged(const QString &fileName)
{
    m_backgroundImagePath = fileName;
    musicBgTransparentChanged();
}

void MusicTopAreaWidget::musicBackgroundChanged()
{
    QString art_path = M_BACKGROUND_PTR->getArtPhotoPath();
    if(!art_path.isEmpty())
    {
        M_BACKGROUND_PTR->indexIncrease();
        drawWindowBackgroundRectString(art_path);
    }
    else
    {
        drawWindowBackgroundRect();
    }
}

void MusicTopAreaWidget::musicBackgroundSliderStateChanged(bool state)
{
    m_ui->background->setNoAnimation(state);
}

void MusicTopAreaWidget::musicBgThemeDownloadFinished()
{
    if(m_ui->surfaceStackedWidget->currentIndex() == 1  &&
       m_ui->musiclrccontainerforinline->artBackgroundIsShow() )
    {
        musicBackgroundChanged();
        m_pictureCarouselTimer.start();
    }
    else
    {
        drawWindowBackgroundRect();
    }
}

void MusicTopAreaWidget::musicBgThemeChangedByResize()
{
    drawWindowBackgroundRectString();
}

void MusicTopAreaWidget::musicPlayListTransparent(int index)
{
    if(m_musicBackgroundWidget)
    {
        m_musicBackgroundWidget->setListTransToolText(index);
    }
    emit setTransparent(m_backgroundAListlpha = index);
}

void MusicTopAreaWidget::musicCircleRemote()
{
    if(m_musicRemoteWidget)
    {
        delete m_musicRemoteWidget;
    }
    m_musicRemoteWidget = new MusicRemoteWidgetForCircle;
    createRemoteWidget();
}

void MusicTopAreaWidget::musicDiamondRemote()
{
    if(m_musicRemoteWidget)
    {
        delete m_musicRemoteWidget;
    }
    m_musicRemoteWidget = new MusicRemoteWidgetForDiamond;
    createRemoteWidget();
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

void MusicTopAreaWidget::musicRipplesRemote()
{
    if(m_musicRemoteWidget)
    {
        delete m_musicRemoteWidget;
    }
    m_musicRemoteWidget = new MusicRemoteWidgetForRipples;
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
        case MusicRemoteWidget::Circle: musicCircleRemote(); break;
        case MusicRemoteWidget::Square: musicSquareRemote(); break;
        case MusicRemoteWidget::Rectangle: musicRectangleRemote(); break;
        case MusicRemoteWidget::SimpleStyle: musicSimpleStyleRemote(); break;
        case MusicRemoteWidget::ComplexStyle: musicComplexStyleRemote(); break;
        case MusicRemoteWidget::Diamond: musicDiamondRemote(); break;
        case MusicRemoteWidget::Ripples: musicRipplesRemote(); break;
    }
}

void MusicTopAreaWidget::createRemoteWidget()
{
    if(!m_musicRemoteWidget)
    {
        return;
    }

    m_musicRemoteWidget->showPlayStatus(!MusicApplication::instance()->isPlaying());
    m_musicRemoteWidget->setVolumeValue(m_ui->musicSound->value());
    connect(m_musicRemoteWidget, SIGNAL(musicWindowSignal()), MusicApplication::instance(), SLOT(showNormal()));
    connect(m_musicRemoteWidget, SIGNAL(musicPlayPreviousSignal()), MusicApplication::instance(), SLOT(musicPlayPrevious()));
    connect(m_musicRemoteWidget, SIGNAL(musicPlayNextSignal()), MusicApplication::instance(), SLOT(musicPlayNext()));
    connect(m_musicRemoteWidget, SIGNAL(musicKeySignal()), MusicApplication::instance(), SLOT(musicStatePlay()));
    connect(m_musicRemoteWidget, SIGNAL(musicSettingSignal()), MusicApplication::instance(), SLOT(musicSetting()));
    connect(m_musicRemoteWidget, SIGNAL(musicVolumeSignal(int)), MusicApplication::instance(), SLOT(musicVolumeChanged(int)));
    connect(m_musicRemoteWidget, SIGNAL(musicRemoteTypeChanged(QAction*)), SLOT(musicRemoteTypeChanged(QAction*)));
    m_musicRemoteWidget->show();
}

void MusicTopAreaWidget::drawWindowBackgroundRect()
{
    QString path = MusicBackgroundSkinDialog::setMBackground(m_backgroundImagePath);
    m_pictureCarouselTimer.stop();
    drawWindowBackgroundRectString(path);
}

void MusicTopAreaWidget::drawWindowBackgroundRectString()
{
    float v = MusicUtils::Widget::reRenderValue<float>(1, 0.35, m_backgroundAlpha);
    MusicApplication::instance()->setWindowOpacity(v);

    QSize size(M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize());
    QPixmap after(size);
    after.fill(Qt::transparent);
    QPainter paint(&after);
    paint.drawPixmap(0, 0, QPixmap::fromImage(m_backgroundImage.scaled(size, Qt::KeepAspectRatioByExpanding)));
    paint.end();

    emit setTransparent(m_backgroundAListlpha);
    m_ui->background->setPixmap(after);
}

void MusicTopAreaWidget::drawWindowBackgroundRectString(const QString &path)
{
    QImage origin(path);
    if(!origin.colorTable().isEmpty())
    {
        origin = origin.convertToFormat(QImage::Format_ARGB32);
    }
    MusicUtils::Widget::reRenderImage(35, &origin, &origin);

    m_backgroundImage = origin;
    drawWindowBackgroundRectString();
}
