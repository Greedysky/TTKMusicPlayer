#include "musictopareawidget.h"
#include "ui_musicapplication.h"
#include "musicuserwindow.h"
#include "musicbackgroundskindialog.h"
#include "musicbgthememanager.h"
#include "musicremotewidgetforsquare.h"
#include "musicremotewidgetforrectangle.h"
#include "musicremotewidgetfordiamond.h"
#include "musicremotewidgetforcircle.h"
#include "musicuiobject.h"

MusicTopAreaWidget::MusicTopAreaWidget(QWidget *parent)
    : QWidget(parent), m_musicbgskin(NULL), m_musicRemoteWidget(NULL)
{
    m_supperClass = parent;
    m_msuicUserWindow = new MusicUserWindow(this);
    connect(&m_pictureCarouselTimer,SIGNAL(timeout()),SLOT(musicBackgroundChanged()));
    m_currentPlayStatus = false;
}

MusicTopAreaWidget::~MusicTopAreaWidget()
{
    delete m_msuicUserWindow;
    delete m_musicbgskin;
    delete m_musicRemoteWidget;
}

void MusicTopAreaWidget::setupUi(Ui::MusicApplication* ui)
{
    m_ui = ui;
    ui->userWindow->addWidget(m_msuicUserWindow);
    ui->musicSongSearchLine->setStyleSheet(MusicUIObject::MLineEditStyle03);
    ui->musicSongSearchLine->setText(tr("please input search text"));

    ui->musicSearchButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->musicSearchButton,SIGNAL(clicked()), m_supperClass, SLOT(musicSearchButtonSearched()));

    ui->musicWindowChangeSkin->setToolTip(tr("changeskin"));
    ui->musicWindowChangeSkin->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowChangeSkin->setStyleSheet(MusicUIObject::MToolButtonStyle01);
    ui->musicWindowChangeSkin->setIconSize(QSize(22,22));
    ui->musicWindowChangeSkin->setIcon(QIcon(QString::fromUtf8(":/image/windowskin")));
    connect(ui->musicWindowChangeSkin,SIGNAL(clicked()) ,SLOT(musicShowSkinChangedWindow()));

    ui->musicWindowConcise->setToolTip(tr("concisein/out"));
    ui->musicWindowConcise->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowConcise->setStyleSheet(MusicUIObject::MToolButtonStyle01);
    ui->musicWindowConcise->setIcon(QIcon(QString::fromUtf8(":/image/concisein")));
    connect(ui->musicWindowConcise,SIGNAL(clicked()), m_supperClass, SLOT(musicWindowConciseChanged()));

    ui->musicWindowRemote->setToolTip(tr("remoteWindow"));
    ui->musicWindowRemote->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowRemote->setStyleSheet(MusicUIObject::MToolButtonStyle01);
    ui->musicWindowRemote->setIcon(QIcon(QString::fromUtf8(":/image/windowremote")));
    connect(ui->musicWindowRemote,SIGNAL(clicked()), SLOT(musicSquareRemote()));

    QPixmap minPix  = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
    ui->minimization->setIcon(QIcon(minPix.scaled(25,25)));
    ui->minimization->setStyleSheet(MusicUIObject::MToolButtonStyle01);
    ui->minimization->setCursor(QCursor(Qt::PointingHandCursor));
    ui->minimization->setToolTip(tr("Minimization"));
    connect(ui->minimization,SIGNAL(clicked()), m_supperClass, SLOT(hide()));


    ui->windowClose->setToolTip(tr("Close"));
    ui->windowClose->setCursor(QCursor(Qt::PointingHandCursor));
    ui->windowClose->setStyleSheet(MusicUIObject::MToolButtonStyle01);
    ui->windowClose->setIcon(QIcon(QPixmap(QString::fromUtf8(":/image/close")).scaled(25,25)));
    connect(ui->windowClose,SIGNAL(clicked()), m_supperClass, SLOT(close()));
}

void MusicTopAreaWidget::setParameters(const QString& b, int a)
{
    m_currentBgSkin = b;
    m_alpha = a;
    musicBgTransparentChanged(a);
}

QString MusicTopAreaWidget::getBgSkin()
{
    return m_currentBgSkin;
}

int MusicTopAreaWidget::getBgSkinAlpha()
{
    return m_alpha;
}

void MusicTopAreaWidget::setTimerStop()
{
    m_pictureCarouselTimer.stop();
}

void MusicTopAreaWidget::musicShowSkinChangedWindow()
{
    if(m_musicbgskin == NULL)
    {
        m_musicbgskin = new MusicBackgroundSkinDialog(this);
    }
    m_musicbgskin->setCurrentBgTheme(m_currentBgSkin, m_alpha);
    m_musicbgskin->exec();
}

void MusicTopAreaWidget::musicUserContextLogin()
{
    m_msuicUserWindow->musicUserContextLogin();
}

void MusicTopAreaWidget::musicBgTransparentChanged(int index)
{
    if(m_ui->SurfaceStackedWidget->currentIndex() == 2) return;
    m_alpha = index;//save the alpha
    //Here set the picture transparency
    drawWindowBackgroundRect();
}

void MusicTopAreaWidget::musicBgTransparentChanged()
{
    musicBgTransparentChanged(m_alpha);
}

void MusicTopAreaWidget::musicBackgroundSkinChanged(const QString &filename)
{
    m_currentBgSkin = filename;
    musicBgTransparentChanged();
}

void MusicTopAreaWidget::musicBackgroundChanged()
{
    QString art_path = M_ARTBG.getArtPhotoPath();
    !art_path.isEmpty() ? drawWindowBackgroundRectString(art_path) : drawWindowBackgroundRect();
}

void MusicTopAreaWidget::drawWindowBackgroundRect()
{
    m_pictureCarouselTimer.stop();
    drawWindowBackgroundRectString(THEME_DOWNLOAD + m_currentBgSkin + SKN_FILE);
}

void MusicTopAreaWidget::drawWindowBackgroundRectString(const QString& path)
{
    QSize size(950, 620);
    QPixmap origin(path);
    QPixmap afterDeal( size );
    afterDeal.fill(Qt::transparent);
    QPainter paint(&afterDeal);
    paint.fillRect(0,0,afterDeal.width(),afterDeal.height(),QColor(255,255,255,2.55*m_alpha));
    paint.setCompositionMode(QPainter::CompositionMode_SourceIn);
    paint.drawPixmap(0,0,QPixmap::fromImage(origin.scaled(size, Qt::KeepAspectRatioByExpanding).toImage()));
    paint.end();

    emit updateToolStyle();
    m_ui->background->setPixmap(afterDeal);
}

void MusicTopAreaWidget::musicBgThemeDownloadFinished()
{
    if(m_ui->SurfaceStackedWidget->currentIndex() == 2  &&
       m_ui->musiclrccontainerforinline->artBackgroundIsShow() )
    {
        musicBackgroundChanged();
        emit updateArtPicture();
        m_pictureCarouselTimer.start(5000);
    }
    else
        drawWindowBackgroundRect();
}

void MusicTopAreaWidget::musicVolumeChangedFromRemote(int value)
{
    m_ui->musicSoundSlider->setValue(value);
}

void MusicTopAreaWidget::showPlayStatus(bool status)
{
    m_currentPlayStatus = status;
    if(m_musicRemoteWidget)
    {
        m_musicRemoteWidget->showPlayStatus(status);
    }
}

void MusicTopAreaWidget::setLabelText(const QString &name)
{
    if(m_musicRemoteWidget)
    {
        m_musicRemoteWidget->setLabelText(name);
    }
}

void MusicTopAreaWidget::setVolumeValue(int value)
{
    if(m_musicRemoteWidget)
    {
        m_musicRemoteWidget->setVolumeValue(value);
    }
}

void MusicTopAreaWidget::createRemoteWidget()
{
    if(m_musicRemoteWidget == NULL)
    {
        return;
    }
    m_musicRemoteWidget->showPlayStatus(m_currentPlayStatus);
    m_musicRemoteWidget->setVolumeValue(m_ui->musicSoundSlider->value());
    connect(m_musicRemoteWidget,SIGNAL(musicWindowSignal()), m_supperClass, SLOT(showNormal()));
    connect(m_musicRemoteWidget,SIGNAL(musicPlayPriviousSignal()), m_supperClass, SLOT(musicPlayPrivious()));
    connect(m_musicRemoteWidget,SIGNAL(musicPlayNextSignal()), m_supperClass, SLOT(musicPlayNext()));
    connect(m_musicRemoteWidget,SIGNAL(musicKeySignal()), m_supperClass, SLOT(musicKey()));
    connect(m_musicRemoteWidget,SIGNAL(musicVolumeSignal(int)), this, SLOT(musicVolumeChangedFromRemote(int)));
    connect(m_musicRemoteWidget,SIGNAL(musicSettingSignal()), m_supperClass, SLOT(musicSetting()));
    m_musicRemoteWidget->show();
}

void MusicTopAreaWidget::musicDeleteRemote()
{
    delete m_musicRemoteWidget;
    m_musicRemoteWidget = NULL;
}

void MusicTopAreaWidget::musicSquareRemote()
{
    if(m_musicRemoteWidget)
        delete m_musicRemoteWidget;
    m_musicRemoteWidget = new MusicRemoteWidgetForSquare;
    createRemoteWidget();
}

void MusicTopAreaWidget::musicCircleRemote()
{
    if(m_musicRemoteWidget)
        delete m_musicRemoteWidget;
    m_musicRemoteWidget = new MusicRemoteWidgetForCircle;
    createRemoteWidget();
}

void MusicTopAreaWidget::musicDiamondRemote()
{
    if(m_musicRemoteWidget)
        delete m_musicRemoteWidget;
    m_musicRemoteWidget = new MusicRemoteWidgetForDiamond;
    createRemoteWidget();
}

void MusicTopAreaWidget::musicRectangleRemote()
{
    if(m_musicRemoteWidget)
        delete m_musicRemoteWidget;
    m_musicRemoteWidget = new MusicRemoteWidgetForRectangle;
    m_musicRemoteWidget->setLabelText(m_ui->showCurrentSong->text().split('=').back().trimmed());
    createRemoteWidget();
}
