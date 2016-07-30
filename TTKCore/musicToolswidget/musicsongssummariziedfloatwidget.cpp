#include "musicsongssummariziedfloatwidget.h"
#include "musicconnectionpool.h"
#include "musicnumberdefine.h"
#include "musicbottomareawidget.h"
#include "musicapplication.h"
#include "musicttkuiobject.h"
#include "musicutils.h"

#include <QToolButton>
#include <QPropertyAnimation>

MusicSongsSummariziedFloatWidget::MusicSongsSummariziedFloatWidget(QWidget *parent)
    : QLabel(parent)
{
    setWindowFlags( Qt::Window | Qt::FramelessWindowHint );
    setObjectName("MainWidget");
    setStyleSheet("#MainWidget{background:rgba(255, 255, 255, 180);}");

    resize(55, 26);
    MusicUtils::UWidget::widgetToRound(this, 10, 10);

    QToolButton *locationButton = new QToolButton(this);
    QToolButton *searchButton = new QToolButton(this);
    locationButton->setStyleSheet(MusicTTKUIObject::MKGTinyBtnLocation);
    locationButton->setCursor(QCursor(Qt::PointingHandCursor));
    locationButton->setGeometry(0, 1, 24, 24);
    searchButton->setStyleSheet(MusicTTKUIObject::MKGTinyBtnLocalSearch);
    searchButton->setCursor(QCursor(Qt::PointingHandCursor));
    searchButton->setGeometry(30, 1, 24, 24);
    connect(locationButton, SIGNAL(clicked()), SIGNAL(musicCurrentPlayLocation()));
    connect(searchButton, SIGNAL(clicked()), SIGNAL(musicSearch()));

    m_currentAnimationValue = 1;
    m_timer.setInterval(3*MT_S2MS);
    connect(&m_timer, SIGNAL(timeout()), SLOT(leaveTimeout()));

    m_animation = new QPropertyAnimation(this, "windowOpacity");
    m_animation->setDuration(MT_S2MS);
    connect(m_animation, SIGNAL(finished()), SLOT(animationFinished()));

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicBottomAreaWidget::getClassName());
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicApplication::getClassName());
    m_timer.start();
}

MusicSongsSummariziedFloatWidget::~MusicSongsSummariziedFloatWidget()
{
    M_CONNECTION_PTR->poolDisConnect(getClassName());
    delete m_animation;
}

QString MusicSongsSummariziedFloatWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongsSummariziedFloatWidget::setGeometry(QObject *object)
{
    QWidget *parent = MStatic_cast(QWidget*, object);
    QPoint global(parent->size().width(), parent->size().height());
    global = parent->mapToGlobal( global );
    move(global.x() - width() - height(), global.y() - 2*height());
}

void MusicSongsSummariziedFloatWidget::start(bool play, int end)
{
    if(m_animation->state() == QAbstractAnimation::Running)
    {
        m_currentAnimationValue = m_animation->currentValue().toFloat();
    }
    m_animation->stop();
    play ? m_timer.start() : m_timer.stop();
    m_animation->setStartValue(m_currentAnimationValue);
    m_animation->setEndValue(end);
    m_animation->start();
}

void MusicSongsSummariziedFloatWidget::leaveTimeout()
{
    m_currentAnimationValue = 1;
    start(false, 0);
}

void MusicSongsSummariziedFloatWidget::animationFinished()
{
    m_currentAnimationValue = m_animation->currentValue().toFloat();
    if(m_currentAnimationValue == 0)
    {
        QTimer::singleShot(MT_MS*100, this, SIGNAL(deleteObject()));
    }
}

void MusicSongsSummariziedFloatWidget::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
}

void MusicSongsSummariziedFloatWidget::enterEvent(QEvent *event)
{
    QLabel::enterEvent(event);
    if(m_currentAnimationValue != 0)
    {
        start(false, 1);
    }
}

void MusicSongsSummariziedFloatWidget::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    start(true, 0);
}
