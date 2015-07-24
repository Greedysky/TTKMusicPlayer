#include "musiclrcfloatwidget.h"
#include "musicbgthememanager.h"
#include "musicobject.h"

#include <QPropertyAnimation>
#include <QPushButton>
#include <QCheckBox>


MusicLrcFloatPhotoWidget::MusicLrcFloatPhotoWidget(QWidget *parent)
    : QWidget(parent)
{
    setGeometry(0, parent->height() - 180, parent->width(), 180);
    setObjectName("MusicLrcFloatPhotoWidget");
    setStyleSheet("#MusicLrcFloatPhotoWidget{background:rgba(0, 0, 0, 100);}");

    m_animation = new QPropertyAnimation(this, "geometry");
    m_animation->setDuration(500);

    m_filmBGWidget = new QWidget(this);
    m_filmBGWidget->setGeometry(0, 0, parent->width(), 125);
    m_filmBGWidget->setStyleSheet("background-image:url(':/lrc/film');");

    m_checkBox = new QCheckBox("cccc",this);
    m_checkBox->setGeometry(5, 130, 100, 20);
    m_checkBox->setStyleSheet(MusicObject::MusicSettingCheckButton);

    m_confirmButton = new QPushButton(tr("Confirm"),this);
    m_confirmButton->setGeometry(340, 130, 70, 20);
    m_confirmButton->setStyleSheet(MusicObject::MusicSettingMainButton);
    m_confirmButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_cancelButton = new QPushButton(tr("Close"), this);
    m_cancelButton->setGeometry(420, 130, 70, 20);
    m_cancelButton->setStyleSheet(MusicObject::MusicSettingMainButton);
    m_cancelButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_previous = new QPushButton("<", this);
    m_previous->setGeometry(10, 38, 15, 50);
    m_next = new QPushButton(">", this);
    m_next->setGeometry(500, 38, 15, 50);
    m_previous->setCursor(QCursor(Qt::PointingHandCursor));
    m_next->setCursor(QCursor(Qt::PointingHandCursor));
    const QString style = "background:rgba(255, 255, 255, 50);border:none";
    m_previous->setStyleSheet( style );
    m_next->setStyleSheet( style );

    connect(m_cancelButton, SIGNAL(clicked()), SLOT(close()));
//    connect(m_confirmButton, SIGNAL(clicked()), SLOT(searchMusicLrcs()));
}

MusicLrcFloatPhotoWidget::~MusicLrcFloatPhotoWidget()
{
    delete m_animation;
    delete m_filmBGWidget;
    delete m_previous, m_next;
    delete m_confirmButton, m_cancelButton;
    delete m_checkBox;
}

void MusicLrcFloatPhotoWidget::show()
{
    QWidget::show();
    m_animation->setStartValue(QRect(0, 500, 115, 105));
    m_animation->setEndValue( geometry() );
    m_animation->start();
}

MusicLrcFloatWidget::MusicLrcFloatWidget(QWidget *parent)
    : QLabel(parent)
{
    m_posX = parent->width() - 235;
    m_posY = parent->height()/4;
    setStyleSheet("background:rgba(0, 0, 0, 100)");
    setGeometry( m_posX + 110, m_posY + 51, 115, 105);

    m_floatPhotoWidget = new MusicLrcFloatPhotoWidget(parent);
    m_floatPhotoWidget->hide();

    m_animation = new QPropertyAnimation(this, "geometry");
    m_animation->setDuration(500);

    const QString style = "QPushButton{background:transparent;text-align:left; \
                           color:white;}QPushButton::hover{image:url(:/lrc/shadow);}";

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

    m_update->setStyleSheet( style );
    m_search->setStyleSheet( style );
    m_more->setStyleSheet( style );
    m_wallp->setStyleSheet( style );
    m_photo->setStyleSheet( style );

    m_update->setGeometry(10, 10, 80, 30);
    m_search->setGeometry(10, 50, 80, 30);
    m_more->setGeometry(10, 90, 80, 30);
    m_wallp->setGeometry(10, 130, 80, 30);
    m_photo->setGeometry(10, 170, 80, 30);

    m_update->setCursor(QCursor(Qt::PointingHandCursor));
    m_search->setCursor(QCursor(Qt::PointingHandCursor));
    m_more->setCursor(QCursor(Qt::PointingHandCursor));
    m_wallp->setCursor(QCursor(Qt::PointingHandCursor));
    m_photo->setCursor(QCursor(Qt::PointingHandCursor));

    connect(m_update, SIGNAL(clicked()), parent, SIGNAL(theCurrentLrcUpdated()));
    connect(m_search, SIGNAL(clicked()), parent, SLOT(searchMusicLrcs()));
    connect(m_photo, SIGNAL(clicked()), m_floatPhotoWidget, SLOT(show()));
}

MusicLrcFloatWidget::~MusicLrcFloatWidget()
{
    delete m_more, m_update, m_search;
    delete m_wallp, m_photo;
    delete m_floatPhotoWidget;
    delete m_animation;
}

void MusicLrcFloatWidget::enterEvent(QEvent *event)
{
    QLabel::enterEvent(event);
    m_animation->setStartValue( geometry() );
    m_animation->setEndValue(QRect(m_posX, m_posY, 115, 210));
    m_animation->start();
}

void MusicLrcFloatWidget::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    m_animation->setStartValue(QRect(m_posX, m_posY, 115, 210));
    m_animation->setEndValue(QRect(m_posX + 110, m_posY + 51, 115, 105));
    m_animation->start();
}
