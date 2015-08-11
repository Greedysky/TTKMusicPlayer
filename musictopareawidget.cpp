#include "musictopareawidget.h"
#include "ui_musicapplication.h"
#include "musicuserwindow.h"
#include "musicbackgroundskindialog.h"
#include "musicbgthememanager.h"

MusicTopAreaWidget::MusicTopAreaWidget(QWidget *parent)
    : QWidget(parent), m_musicbgskin(NULL)
{
    m_msuicUserWindow = new MusicUserWindow(this);
    connect(&m_pictureCarouselTimer,SIGNAL(timeout()),SLOT(musicBackgroundChanged()));

}

MusicTopAreaWidget::~MusicTopAreaWidget()
{
    delete m_msuicUserWindow;
    delete m_musicbgskin;
}

void MusicTopAreaWidget::setupUi(Ui::MusicApplication* ui)
{
    m_ui = ui;
    ui->userWindow->addWidget(m_msuicUserWindow);
    ui->musicSongSearchLine->setStyleSheet("QLineEdit{border-image: url(:/image/searchline);"
                                           "font-size:15px; color:#6495ED;}");
    ui->musicSongSearchLine->setText(tr("please input search text"));

    ui->musicSearchButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->musicSearchButton,SIGNAL(clicked()), parent(),SLOT(musicSearchButtonSearched()));

    ui->musicWindowChangeSkin->setToolTip(tr("changeskin"));
    ui->musicWindowChangeSkin->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowChangeSkin->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->musicWindowChangeSkin->setIconSize(QSize(22,22));
    ui->musicWindowChangeSkin->setIcon(QIcon(QString::fromUtf8(":/image/windowskin")));
    connect(ui->musicWindowChangeSkin,SIGNAL(clicked()) ,SLOT(musicShowSkinChangedWindow()));

    ui->musicWindowConcise->setToolTip(tr("concisein/out"));
    ui->musicWindowConcise->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowConcise->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->musicWindowConcise->setIcon(QIcon(QString::fromUtf8(":/image/concisein")));
    connect(ui->musicWindowConcise,SIGNAL(clicked()), parent(), SLOT(musicWindowConciseChanged()));

    ui->musicWindowRemote->setToolTip(tr("remoteWindow"));
    ui->musicWindowRemote->setCursor(QCursor(Qt::PointingHandCursor));
    ui->musicWindowRemote->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->musicWindowRemote->setIcon(QIcon(QString::fromUtf8(":/image/windowremote")));
    connect(ui->musicWindowRemote,SIGNAL(clicked()), parent(), SLOT(musicSquareRemote()));

    QPixmap minPix  = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
    ui->minimization->setIcon(QIcon(minPix.scaled(25,25)));
    ui->minimization->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->minimization->setCursor(QCursor(Qt::PointingHandCursor));
    ui->minimization->setToolTip(tr("Minimization"));
    connect(ui->minimization,SIGNAL(clicked()), parent(), SLOT(hide()));


    ui->windowClose->setToolTip(tr("Close"));
    ui->windowClose->setCursor(QCursor(Qt::PointingHandCursor));
    ui->windowClose->setStyleSheet(MusicObject::QToolButtonStyle);
    ui->windowClose->setIcon(QIcon(QPixmap(QString::fromUtf8(":/image/close")).scaled(25,25)));
    connect(ui->windowClose,SIGNAL(clicked()), parent(), SLOT(close()));
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

void MusicTopAreaWidget::musicShowSkinChangedWindow()
{
    if(m_musicbgskin == NULL)
        m_musicbgskin = new MusicBackgroundSkinDialog(this);
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

//    m_musicSongTree->setStyleSheet(MusicObject::QToolBoxItemStyle);
    m_ui->background->setPixmap(afterDeal);
}

void MusicTopAreaWidget::musicBgThemeDownloadFinished()
{
    if(m_ui->SurfaceStackedWidget->currentIndex() == 2  &&
       m_ui->musiclrccontainerforinline->artBackgroundIsShow() )
    {
        musicBackgroundChanged();
//        m_musicSongTree->updateArtPicture();
        m_pictureCarouselTimer.start(5000);
    }
    else
        drawWindowBackgroundRect();
}
