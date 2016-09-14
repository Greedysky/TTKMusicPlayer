#include "musicsongssummariziedfloatwidget.h"
#include "musicnumberdefine.h"
#include "musicbottomareawidget.h"
#include "musicapplication.h"
#include "musictinyuiobject.h"
#include "musicuiobject.h"
#include "musicutils.h"

#include <QToolButton>
#include <QPropertyAnimation>

MusicSongsSummariziedFloatWidget::MusicSongsSummariziedFloatWidget(QWidget *parent)
    : QLabel(parent)
{
    setWindowFlags( Qt::Window | Qt::FramelessWindowHint );
    setObjectName("MainWidget");
    setStyleSheet(QString("#MainWidget{%1}").arg(MusicUIObject::MBackgroundStyle12));

    resize(55, 26);
    MusicUtils::Widget::widgetToRound(this, 10, 10);

    QToolButton *locationButton = new QToolButton(this);
    QToolButton *searchButton = new QToolButton(this);
    locationButton->setStyleSheet(MusicUIObject::MKGTinyBtnLocation);
    locationButton->setCursor(QCursor(Qt::PointingHandCursor));
    locationButton->setGeometry(0, 1, 24, 24);
    searchButton->setStyleSheet(MusicUIObject::MKGTinyBtnLocalSearch);
    searchButton->setCursor(QCursor(Qt::PointingHandCursor));
    searchButton->setGeometry(30, 1, 24, 24);
    connect(locationButton, SIGNAL(clicked()), MusicApplication::instance(), SLOT(musicCurrentPlayLocation()));
    connect(searchButton, SIGNAL(clicked()), MusicBottomAreaWidget::instance(), SLOT(musicSearch()));

    m_currentAnimationValue = 1;
    m_timer.setInterval(3*MT_S2MS);
    connect(&m_timer, SIGNAL(timeout()), SLOT(leaveTimeout()));

    m_animation = new QPropertyAnimation(this, "windowOpacity");
    m_animation->setDuration(MT_S2MS);
    connect(m_animation, SIGNAL(finished()), SLOT(animationFinished()));

    m_timer.start();
}

MusicSongsSummariziedFloatWidget::~MusicSongsSummariziedFloatWidget()
{
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
