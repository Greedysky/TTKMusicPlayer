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
#include "musicuiobject.h"
#include "musictinyuiobject.h"
#include "musicfunctionuiobject.h"
#include "musicwydiscoverlistthread.h"

MusicTopAreaWidget *MusicTopAreaWidget::m_instance = nullptr;

MusicTopAreaWidget::MusicTopAreaWidget(QWidget *parent)
    : QWidget(parent), m_musicbgskin(nullptr), m_musicRemoteWidget(nullptr)
{
    m_instance = this;
    m_musicUserWindow = new MusicUserWindow(this);
    m_getDiscoverThread = new MusicWYDiscoverListThread(this);

    m_pictureCarouselTimer.setInterval(10*MT_S2MS);
    connect(&m_pictureCarouselTimer, SIGNAL(timeout()), SLOT(musicBackgroundChanged()));
    connect(M_BACKGROUND_PTR, SIGNAL(userSelectIndexChanged()), SLOT(musicBackgroundChanged()));
    connect(m_getDiscoverThread, SIGNAL(downLoadDataChanged(QString)), SLOT(musicSearchTopListInfoFinished()));
    m_getDiscoverThread->startSearchSong();

    m_currentPlayStatus = true;
    m_listAlpha = 40;
}

MusicTopAreaWidget::~MusicTopAreaWidget()
{
    delete m_musicUserWindow;
    delete m_musicbgskin;
    delete m_musicRemoteWidget;
    delete m_getDiscoverThread;
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

void MusicTopAreaWidget::setParameters(const QString &skin, int alpha, int alphaR)
{
    m_currentBgSkin = skin;
    m_listAlpha = alphaR;
    musicBgTransparentChanged(m_alpha = alpha);
}

QPixmap MusicTopAreaWidget::getBgSkinPixmap() const
{
    return m_ui->background->getRendererPixmap();
}

int MusicTopAreaWidget::getListBgSkinAlpha()
{
    if(m_musicbgskin)
    {
        m_listAlpha = m_musicbgskin->getListBgSkinAlpha();
    }
    return m_listAlpha;
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
    m_currentPlayStatus = status;
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

void MusicTopAreaWidget::musicSearchTopListInfoFinished()
{
    m_ui->musicSongSearchLine->setPlaceholderText( m_getDiscoverThread->getTopListInfo() );
}

void MusicTopAreaWidget::musicShowSkinChangedWindow()
{
    if(m_musicbgskin == nullptr)
    {
        m_musicbgskin = new MusicBackgroundSkinDialog(this);
    }
    m_musicbgskin->setCurrentBgTheme(m_currentBgSkin, m_alpha, m_listAlpha);
    m_musicbgskin->exec();
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
    if(m_musicbgskin)
    {
        m_musicbgskin->setSkinTransToolText(index);
    }
    m_alpha = index;    //save the alpha
    drawWindowBackgroundRect();
}

void MusicTopAreaWidget::musicBgTransparentChanged(const QString &fileName)
{
    if(m_musicbgskin)
    {
        m_musicbgskin->updateBackground(fileName);
    }
    if(m_ui->surfaceStackedWidget->currentIndex() == 1)
    {
        return;
    }
    drawWindowBackgroundRectString(fileName);
}

void MusicTopAreaWidget::musicBgTransparentChanged()
{
    musicBgTransparentChanged(m_alpha);
}

void MusicTopAreaWidget::musicBackgroundSkinChanged(const QString &fileName)
{
    m_currentBgSkin = fileName;
    musicBgTransparentChanged();
}

void MusicTopAreaWidget::musicBackgroundChanged()
{
    QString art_path = M_BACKGROUND_PTR->getArtPhotoPath();
    !art_path.isEmpty() ? drawWindowBackgroundRectString(art_path) : drawWindowBackgroundRect();
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
    if(m_ui->surfaceStackedWidget->currentIndex() == 1  &&
       m_ui->musiclrccontainerforinline->artBackgroundIsShow() )
    {
        m_pictureCarouselTimer.stop();
        QString art_path = M_BACKGROUND_PTR->getArtPhotoPathByIndex();
        !art_path.isEmpty() ? drawWindowBackgroundRectString(art_path) : drawWindowBackgroundRect();
        m_pictureCarouselTimer.start();
    }
    else
    {
        drawWindowBackgroundRect();
    }
}

void MusicTopAreaWidget::musicPlayListTransparent(int index)
{
    if(m_musicbgskin)
    {
        m_musicbgskin->setListTransToolText(index);
    }
    emit setTransparent(m_listAlpha = index);
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

void MusicTopAreaWidget::musicDeleteRemote()
{
    delete m_musicRemoteWidget;
    m_musicRemoteWidget = nullptr;
}

void MusicTopAreaWidget::musicRemoteTypeChanged(QAction *type)
{
    MusicRemoteWidget *tempRemote = m_musicRemoteWidget;
    m_musicRemoteWidget = nullptr;
    if(type->text() == tr("CircleRemote")) musicCircleRemote();
    else if(type->text() == tr("DiamondRemote")) musicDiamondRemote();
    else if(type->text() == tr("SquareRemote")) musicSquareRemote();
    else if(type->text() == tr("RectangleRemote")) musicRectangleRemote();
    else if(type->text() == tr("SimpleStyleRemote")) musicSimpleStyleRemote();
    else if(type->text() == tr("ComplexStyleRemote")) musicComplexStyleRemote();
    else
    {
        m_musicRemoteWidget = tempRemote;
        return;
    }
    tempRemote->deleteLater();
}

void MusicTopAreaWidget::createRemoteWidget()
{
    if(m_musicRemoteWidget == nullptr)
    {
        return;
    }

    m_musicRemoteWidget->showPlayStatus(m_currentPlayStatus);
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
    QString path = THEME_DIR_FULL + m_currentBgSkin + SKN_FILE;
    M_BACKGROUND_PTR->setMBackground(path);
    if(m_musicbgskin)
    {
        m_musicbgskin->updateBackground(path);
    }
    m_pictureCarouselTimer.stop();
    drawWindowBackgroundRectString(path);
}

void MusicTopAreaWidget::drawWindowBackgroundRectString(const QString &path)
{
    QSize size(M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize());

    QImage origin(path);
    if(!origin.colorTable().isEmpty())
    {
        origin = origin.convertToFormat(QImage::Format_ARGB32);
    }
    reRenderImage(35, &origin, &origin);

    QPixmap afterDeal( size );
    afterDeal.fill(Qt::transparent);
    QPainter paint(&afterDeal);
    paint.fillRect(0, 0, afterDeal.width(), afterDeal.height(), QColor(255, 255, 255, 2.55*m_alpha));
    paint.setCompositionMode(QPainter::CompositionMode_SourceIn);
    paint.drawPixmap(0, 0, QPixmap::fromImage(origin.scaled(size, Qt::KeepAspectRatioByExpanding)));
    paint.end();

    emit setTransparent(m_listAlpha);
    m_ui->background->setPixmap(afterDeal);
}

void MusicTopAreaWidget::reRenderImage(int delta, const QImage *input, QImage *output)
{
    for(int w=0; w<input->width(); w++)
    {
        for(int h=0; h<input->height(); h++)
        {
            QRgb rgb = input->pixel(w, h);
            uint resultR = colorBurnTransform(qRed(rgb), delta);
            uint resultG = colorBurnTransform(qGreen(rgb), delta);
            uint resultB = colorBurnTransform(qBlue(rgb), delta);
            uint newRgb = ((resultR & 255)<<16 | (resultG & 255)<<8 | (resultB & 255));
            output->setPixel(w, h, newRgb);
        }
    }
}

uint MusicTopAreaWidget::colorBurnTransform(int c, int delta)
{
    Q_ASSERT(0 <= delta && delta < 255);

    int result = (c - (uint)(c*delta)/(255 - delta));
    if(result > 255)
    {
        result = 255;
    }else if(result < 0)
    {
        result = 0;
    }
    return result;
}
