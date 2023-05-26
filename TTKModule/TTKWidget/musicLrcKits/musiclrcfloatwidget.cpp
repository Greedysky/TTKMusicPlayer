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

    m_update = new QPushButton(tr(" Update"), this);
    m_search = new QPushButton(tr(" Search"), this);
    m_more = new QPushButton(tr(" More"), this);
    m_wallpaper = new QPushButton(tr(" Wallp"), this);
    m_photo = new QPushButton(tr(" Photo"), this);

#ifdef Q_OS_UNIX
    m_update->setFocusPolicy(Qt::NoFocus);
    m_search->setFocusPolicy(Qt::NoFocus);
    m_more->setFocusPolicy(Qt::NoFocus);
    m_wallpaper->setFocusPolicy(Qt::NoFocus);
    m_photo->setFocusPolicy(Qt::NoFocus);
#endif

    m_update->setGeometry(15, 10, 80, 20);
    m_search->setGeometry(15, 50, 80, 20);
    m_more->setGeometry(15, 90, 80, 20);
    m_wallpaper->setGeometry(15, 130, 80, 20);
    m_photo->setGeometry(15, 170, 80, 20);

    m_update->setStyleSheet(TTK::UI::InteriorFloatUpdate + TTK::UI::PushButtonStyle07 + TTK::UI::PushButtonStyle01);
    m_search->setStyleSheet(TTK::UI::InteriorFloatSearch + TTK::UI::PushButtonStyle07 + TTK::UI::PushButtonStyle01);
    m_more->setStyleSheet(TTK::UI::InteriorFloatMore + TTK::UI::PushButtonStyle07 + TTK::UI::PushButtonStyle01);
    m_wallpaper->setStyleSheet(TTK::UI::InteriorFloatWallpaper + TTK::UI::PushButtonStyle07 + TTK::UI::PushButtonStyle01);
    m_photo->setStyleSheet(TTK::UI::InteriorFloatPhoto + TTK::UI::PushButtonStyle07 + TTK::UI::PushButtonStyle01);

    m_update->setCursor(QCursor(Qt::PointingHandCursor));
    m_search->setCursor(QCursor(Qt::PointingHandCursor));
    m_more->setCursor(QCursor(Qt::PointingHandCursor));
    m_wallpaper->setCursor(QCursor(Qt::PointingHandCursor));
    m_photo->setCursor(QCursor(Qt::PointingHandCursor));
#ifdef Q_OS_UNIX
    m_wallpaper->setEnabled(false);
#endif
    connect(m_update, SIGNAL(clicked()), parent, SIGNAL(currentLrcUpdated()));
    connect(m_search, SIGNAL(clicked()), parent, SLOT(searchMusicLrcs()));
    connect(m_wallpaper, SIGNAL(clicked()), SLOT(wallpaperButtonClicked()));
    connect(m_photo, SIGNAL(clicked()), SLOT(showArtistPhotoWidget()));
    connect(m_more, SIGNAL(clicked()), SLOT(showFloatSettingWidget()));
    connect(m_floatSettingWidget, SIGNAL(widgetClose()), SLOT(closeFloatSettingWidget()));
}

MusicLrcFloatWidget::~MusicLrcFloatWidget()
{
    delete m_more;
    delete m_update;
    delete m_search;
    delete m_wallpaper;
    delete m_photo;
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
    if(m_wallpaper->styleSheet().contains(TTK::UI::InteriorFloatWallpaper))
    {
        m_wallpaper->setStyleSheet(TTK::UI::InteriorFloatWallpaperOn + TTK::UI::PushButtonStyle11 + TTK::UI::PushButtonStyle01);
    }
    else
    {
        m_wallpaper->setStyleSheet(TTK::UI::InteriorFloatWallpaper + TTK::UI::PushButtonStyle07 + TTK::UI::PushButtonStyle01);
    }

    if(MusicBottomAreaWidget::instance()->isLrcWidgetShowFullScreen())
    {
        MusicBottomAreaWidget::instance()->lrcWidgetShowFullScreen();
    }
    MusicRightAreaWidget::instance()->containerForWallpaperClicked();
}
