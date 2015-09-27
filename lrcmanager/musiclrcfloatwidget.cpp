#include "musiclrcfloatwidget.h"
#include "musiclrcfloatphotowidget.h"
#include "musiclrcfloatsettingwidget.h"

#include <QPushButton>

MusicLrcFloatWidget::MusicLrcFloatWidget(QWidget *parent)
    : MusicLrcFloatAbstractWidget(parent)
{
    setStyleSheet(MusicUIObject::MCustomStyle03);
    m_rectIn = QRect(405, 120, 115, 210);
    m_rectOut = QRect(515, 171, 115, 105);
    setGeometry( m_rectOut );

    m_floatPhotoWidget = new MusicLrcFloatPhotoWidget(parent);
    m_floatPhotoWidget->hide();
    m_floatSettingWidget = new MusicLrcFloatSettingWidget(parent);
    m_floatSettingWidget->hide();

    m_update = new QPushButton(tr(" Update"), this);
    m_search = new QPushButton(tr(" Search"), this);
    m_more = new QPushButton(tr(" More"), this);
    m_wallp = new QPushButton(tr(" Wallp"), this);
    m_photo = new QPushButton(tr(" Photo"), this);

    m_update->setIcon(QIcon(":/lrc/update"));
    m_search->setIcon(QIcon(":/lrc/search"));
    m_more->setIcon(QIcon(":/lrc/more"));
    m_wallp->setIcon(QIcon(":/lrc/wallpaper"));
    m_photo->setIcon(QIcon(":/lrc/photo"));

    m_update->setStyleSheet( MusicUIObject::MPushButtonStyle15 );
    m_search->setStyleSheet( MusicUIObject::MPushButtonStyle15 );
    m_more->setStyleSheet( MusicUIObject::MPushButtonStyle15 );
    m_wallp->setStyleSheet( MusicUIObject::MPushButtonStyle15 );
    m_photo->setStyleSheet( MusicUIObject::MPushButtonStyle15 );

    m_update->setGeometry(15, 10, m_update->width(), m_update->height());
    m_search->setGeometry(15, 50, m_search->width(), m_search->height());
    m_more->setGeometry(15, 90, m_more->width(), m_more->height());
    m_wallp->setGeometry(15, 130, m_wallp->width(), m_wallp->height());
    m_photo->setGeometry(15, 170, m_photo->width(), m_photo->height());

    m_update->setCursor(QCursor(Qt::PointingHandCursor));
    m_search->setCursor(QCursor(Qt::PointingHandCursor));
    m_more->setCursor(QCursor(Qt::PointingHandCursor));
    m_wallp->setCursor(QCursor(Qt::PointingHandCursor));
    m_photo->setCursor(QCursor(Qt::PointingHandCursor));

    connect(m_update, SIGNAL(clicked()), parent, SIGNAL(theCurrentLrcUpdated()));
    connect(m_search, SIGNAL(clicked()), parent, SLOT(searchMusicLrcs()));
    connect(m_photo, SIGNAL(clicked()), m_floatPhotoWidget, SLOT(show()));
    connect(m_floatSettingWidget, SIGNAL(widgetClose()), SLOT(closeFloatSettingWidget()));
    connect(m_more, SIGNAL(clicked()), this, SLOT(showFloatSettingWidget()));
}

MusicLrcFloatWidget::~MusicLrcFloatWidget()
{
    delete m_more, m_update, m_search;
    delete m_wallp, m_photo;
    delete m_floatPhotoWidget;
    delete m_floatSettingWidget;
}

void MusicLrcFloatWidget::showFloatSettingWidget()
{
    animationOut();
    m_floatSettingWidget->show();
    setblockAnimation(true);
}

void MusicLrcFloatWidget::closeFloatSettingWidget()
{
    m_floatSettingWidget->close();
    setblockAnimation(false);
}
