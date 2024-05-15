#include "musiclrcfloatwidget.h"
#include "musiclrcfloatphotowidget.h"
#include "musiclrcfloatsettingwidget.h"
#include "musicinteriorfloatuiobject.h"
#include "musicrightareawidget.h"
#include "musicbottomareawidget.h"
#include "musicwidgetheaders.h"

MusicLrcFloatWidget::MusicLrcFloatWidget(QWidget *parent)
    : MusicAbstractFloatWidget(parent)
{
    setStyleSheet(TTK::UI::BackgroundStyle05);

    m_floatPhotoWidget = new MusicLrcFloatPhotoWidget(parent);
    m_floatPhotoWidget->hide();

    m_floatSettingWidget = new MusicLrcFloatSettingWidget(parent);
    m_floatSettingWidget->hide();

    resizeGeometry(0, 0);

    m_updateButton = new QPushButton(tr(" Update"), this);
    m_searchButton = new QPushButton(tr(" Search"), this);
    m_moreButton = new QPushButton(tr(" More"), this);
    m_wallpaperButton = new QPushButton(tr(" Wallp"), this);
    m_photoButton = new QPushButton(tr(" Photo"), this);

#ifdef Q_OS_UNIX
    m_updateButton->setFocusPolicy(Qt::NoFocus);
    m_searchButton->setFocusPolicy(Qt::NoFocus);
    m_moreButton->setFocusPolicy(Qt::NoFocus);
    m_wallpaperButton->setFocusPolicy(Qt::NoFocus);
    m_photoButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_updateButton->setGeometry(15, 10, 80, 20);
    m_searchButton->setGeometry(15, 50, 80, 20);
    m_moreButton->setGeometry(15, 90, 80, 20);
    m_wallpaperButton->setGeometry(15, 130, 80, 20);
    m_photoButton->setGeometry(15, 170, 80, 20);

    m_updateButton->setStyleSheet(TTK::UI::InteriorFloatUpdate + TTK::UI::PushButtonStyle07 + TTK::UI::PushButtonStyle01);
    m_searchButton->setStyleSheet(TTK::UI::InteriorFloatSearch + TTK::UI::PushButtonStyle07 + TTK::UI::PushButtonStyle01);
    m_moreButton->setStyleSheet(TTK::UI::InteriorFloatMore + TTK::UI::PushButtonStyle07 + TTK::UI::PushButtonStyle01);
    m_wallpaperButton->setStyleSheet(TTK::UI::InteriorFloatWallpaper + TTK::UI::PushButtonStyle07 + TTK::UI::PushButtonStyle01);
    m_photoButton->setStyleSheet(TTK::UI::InteriorFloatPhoto + TTK::UI::PushButtonStyle07 + TTK::UI::PushButtonStyle01);

    m_updateButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_searchButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_moreButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_wallpaperButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_photoButton->setCursor(QCursor(Qt::PointingHandCursor));
#ifdef Q_OS_UNIX
    m_wallpaperButton->setEnabled(false);
#endif
    connect(m_updateButton, SIGNAL(clicked()), parent, SIGNAL(currentLrcUpdated()));
    connect(m_searchButton, SIGNAL(clicked()), parent, SLOT(searchMusicLrcs()));
    connect(m_wallpaperButton, SIGNAL(clicked()), SLOT(wallpaperButtonClicked()));
    connect(m_photoButton, SIGNAL(clicked()), SLOT(showArtistPhotoWidget()));
    connect(m_moreButton, SIGNAL(clicked()), SLOT(showFloatSettingWidget()));
    connect(m_floatSettingWidget, SIGNAL(widgetClose()), SLOT(closeFloatSettingWidget()));
}

MusicLrcFloatWidget::~MusicLrcFloatWidget()
{
    delete m_moreButton;
    delete m_updateButton;
    delete m_searchButton;
    delete m_wallpaperButton;
    delete m_photoButton;
    delete m_floatPhotoWidget;
    delete m_floatSettingWidget;
}

void MusicLrcFloatWidget::resizeGeometry(int width, int height)
{
    m_rectEnter = QRect(566 + width, 138 + height / 2, 115, 210);
    m_rectLeave = QRect(676 + width, 189 + height / 2, 115, 105);
    setGeometry(m_rectLeave);

    m_floatSettingWidget->resizeGeometry(width, height);
    m_floatPhotoWidget->resizeGeometry(width, height);
}

void MusicLrcFloatWidget::showArtistPhotoWidget()
{
    if(m_floatPhotoWidget)
    {
        m_floatPhotoWidget->show();
    }
}

void MusicLrcFloatWidget::showFloatSettingWidget()
{
    animationLeave();
    m_floatSettingWidget->show();
    setBlockAnimation(true);
}

void MusicLrcFloatWidget::closeFloatSettingWidget()
{
    m_floatSettingWidget->close();
    setBlockAnimation(false);
}

void MusicLrcFloatWidget::wallpaperButtonClicked()
{
    if(m_wallpaperButton->styleSheet().contains(TTK::UI::InteriorFloatWallpaper))
    {
        m_wallpaperButton->setStyleSheet(TTK::UI::InteriorFloatWallpaperOn + TTK::UI::PushButtonStyle11 + TTK::UI::PushButtonStyle01);
    }
    else
    {
        m_wallpaperButton->setStyleSheet(TTK::UI::InteriorFloatWallpaper + TTK::UI::PushButtonStyle07 + TTK::UI::PushButtonStyle01);
    }

    if(MusicBottomAreaWidget::instance()->isLrcWidgetShowFullScreen())
    {
        MusicBottomAreaWidget::instance()->lrcWidgetShowFullScreen();
    }
    MusicRightAreaWidget::instance()->containerForWallpaperClicked();
}
