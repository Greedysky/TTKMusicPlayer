#include "musiclrcfloatwidget.h"
#include "musicbgthememanager.h"

#include <QPropertyAnimation>
#include <QPushButton>
#include <QCheckBox>

class MusicLrcFloatPhotoWidget : public QWidget
{
public:
    explicit MusicLrcFloatPhotoWidget(QWidget *parent = 0)
        : QWidget(parent)
    {
        setGeometry(0, parent->height() - 180, parent->width(), 180);
        setObjectName("MusicLrcFloatPhotoWidget");
        setStyleSheet("#MusicLrcFloatPhotoWidget{background:rgba(0, 0, 0, 100);}");
        QWidget *w = new QWidget(this);
        w->setGeometry(0, 0, parent->width(), 125);
        w->setStyleSheet("background-image:url(':/lrc/film');");
        QCheckBox *c = new QCheckBox(this);
        c->setGeometry(0, 125, 20, 20);
//        w->setStyleSheet("background-image:url(':/lrc/film');");
        QPushButton *p = new QPushButton(this);
        p->setGeometry(100, 125, 125, 20);
//        p->setStyleSheet("background-image:url(':/lrc/film');");
    }
    ~MusicLrcFloatPhotoWidget(){}
protected:
//    QWidget

};

MusicLrcFloatWidget::MusicLrcFloatWidget(QWidget *parent)
    : QLabel(parent)
{
    m_posX = parent->width() - 235;
    m_posY = parent->height()/4;
    setStyleSheet("background:rgba(0, 0, 0, 100)");
    setGeometry( m_posX + 110, m_posY + 51, 115, 105);

    new MusicLrcFloatPhotoWidget(parent);

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
}

MusicLrcFloatWidget::~MusicLrcFloatWidget()
{
    delete m_animation;
}

void MusicLrcFloatWidget::enterEvent(QEvent *event)
{
    QLabel::enterEvent(event);
    m_animation->setStartValue(QRect(m_posX + 110, m_posY + 51, 115, 105));
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
