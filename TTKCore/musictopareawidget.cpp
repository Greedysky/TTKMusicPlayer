#include "musictopareawidget.h"
#include "ui_musicapplication.h"
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
#include "musicconnectionpool.h"
#include "musicttkuiobject.h"

MusicTopAreaWidget::MusicTopAreaWidget(QWidget *parent)
    : QWidget(parent), m_musicbgskin(nullptr), m_musicRemoteWidget(nullptr)
{
    m_supperClass = parent;
    m_musicUserWindow = new MusicUserWindow(this);
    connect(&m_pictureCarouselTimer, SIGNAL(timeout()), SLOT(musicBackgroundChanged()));
    connect(M_BACKGROUND_PTR, SIGNAL(userSelectIndexChanged()), SLOT(musicBackgroundChanged()));

    m_currentPlayStatus = true;
    m_listAlpha = 40;

    M_CONNECTION_PTR->setValue(getClassName(), this);
}

MusicTopAreaWidget::~MusicTopAreaWidget()
{
    delete m_musicUserWindow;
    delete m_musicbgskin;
    delete m_musicRemoteWidget;
}

QString MusicTopAreaWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicTopAreaWidget::setupUi(Ui::MusicApplication* ui)
{
    m_ui = ui;
    ui->userWindow->addWidget(m_musicUserWindow);
    ui->musicSongSearchLine->initWidget(m_supperClass);
    ui->musicSongSearchLine->setStyleSheet(MusicUIObject::MLineEditStyle03);
    ui->musicSongSearchLine->setText(tr("please input search text"));

    ui->musicSearchButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicSearchButton->setStyleSheet(MusicTTKUIObject::MKGTinyBtnMainSearch);

    ui->musicWindowChangeSkin->setToolTip(tr("changeskin"));
    ui->musicWindowChangeSkin->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowChangeSkin->setStyleSheet(MusicTTKUIObject::MKGBtnSkin);
    connect(ui->musicWindowChangeSkin, SIGNAL(clicked()) , SLOT(musicShowSkinChangedWindow()));

    ui->musicWindowSetting->setToolTip(tr("setting"));
    ui->musicWindowSetting->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowSetting->setStyleSheet(MusicTTKUIObject::MKGBtnSetting);
    connect(ui->musicWindowSetting, SIGNAL(clicked()), m_supperClass, SLOT(musicCreateRightMenu()));

    ui->musicWindowConcise->setToolTip(tr("concisein/out"));
    ui->musicWindowConcise->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowConcise->setStyleSheet(MusicTTKUIObject::MKGBtnConciseIn);
    connect(ui->musicWindowConcise, SIGNAL(clicked()), m_supperClass, SLOT(musicWindowConciseChanged()));

    ui->musicWindowRemote->setToolTip(tr("remoteWindow"));
    ui->musicWindowRemote->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowRemote->setStyleSheet(MusicTTKUIObject::MKGBtnRemote);
    connect(ui->musicWindowRemote, SIGNAL(clicked()), SLOT(musicSquareRemote()));

    ui->minimization->setStyleSheet(MusicTTKUIObject::MKGBtnMinimum);
    ui->minimization->setCursor(QCursor(Qt::PointingHandCursor));
    ui->minimization->setToolTip(tr("Minimization"));
    connect(ui->minimization, SIGNAL(clicked()), m_supperClass, SLOT(hide()));

    ui->windowClose->setToolTip(tr("Close"));
    ui->windowClose->setCursor(QCursor(Qt::PointingHandCursor));
    ui->windowClose->setStyleSheet(MusicTTKUIObject::MKGBtnTClose);
    connect(ui->windowClose, SIGNAL(clicked()), m_supperClass, SLOT(close()));
}

void MusicTopAreaWidget::setParameters(const QString &skin, int alpha, int alphaR)
{
    m_currentBgSkin = skin;
    m_listAlpha = alphaR;
    musicBgTransparentChanged(m_alpha = alpha);
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
    QPixmap origin(path);
    QPixmap afterDeal( size );
    afterDeal.fill(Qt::transparent);
    QPainter paint(&afterDeal);
    paint.fillRect(0, 0, afterDeal.width(), afterDeal.height(), QColor(255, 255, 255, 2.55*m_alpha));
    paint.setCompositionMode(QPainter::CompositionMode_SourceIn);
    paint.drawPixmap(0, 0, QPixmap::fromImage(origin.scaled(size, Qt::KeepAspectRatioByExpanding).toImage()));
    paint.end();

    emit setTransparent(m_listAlpha);
    m_ui->background->setPixmap(afterDeal);
}

void MusicTopAreaWidget::musicBgThemeDownloadFinished()
{
    if(m_ui->surfaceStackedWidget->currentIndex() == 1  &&
       m_ui->musiclrccontainerforinline->artBackgroundIsShow() )
    {
        musicBackgroundChanged();
        m_pictureCarouselTimer.start(5*MT_S2MS);
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
        m_pictureCarouselTimer.start(5*MT_S2MS);
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

void MusicTopAreaWidget::createRemoteWidget()
{
    if(m_musicRemoteWidget == nullptr)
    {
        return;
    }
    m_musicRemoteWidget->showPlayStatus(m_currentPlayStatus);
    m_musicRemoteWidget->setVolumeValue(m_ui->musicSound->value());
    connect(m_musicRemoteWidget, SIGNAL(musicWindowSignal()), m_supperClass, SLOT(showNormal()));
    connect(m_musicRemoteWidget, SIGNAL(musicPlayPreviousSignal()), m_supperClass, SLOT(musicPlayPrevious()));
    connect(m_musicRemoteWidget, SIGNAL(musicPlayNextSignal()), m_supperClass, SLOT(musicPlayNext()));
    connect(m_musicRemoteWidget, SIGNAL(musicKeySignal()), m_supperClass, SLOT(musicStatePlay()));
    connect(m_musicRemoteWidget, SIGNAL(musicSettingSignal()), m_supperClass, SLOT(musicSetting()));
    connect(m_musicRemoteWidget, SIGNAL(musicVolumeSignal(int)), m_supperClass, SLOT(musicVolumeChanged(int)));
    connect(m_musicRemoteWidget, SIGNAL(musicRemoteTypeChanged(QAction*)), SLOT(musicRemoteTypeChanged(QAction*)));
    m_musicRemoteWidget->show();
}

void MusicTopAreaWidget::musicDeleteRemote()
{
    delete m_musicRemoteWidget;
    m_musicRemoteWidget = nullptr;
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
