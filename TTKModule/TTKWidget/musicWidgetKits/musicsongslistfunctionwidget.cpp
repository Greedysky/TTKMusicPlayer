#include "musicsongslistfunctionwidget.h"
#include "musicapplication.h"
#include "musictinyuiobject.h"
#include "musicwidgetheaders.h"

#include <QPropertyAnimation>

MusicSongsListFunctionWidget::MusicSongsListFunctionWidget(QWidget *parent)
    : QLabel(parent),
      m_currentAnimationValue(1)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    QHBoxLayout *layout = new QHBoxLayout(this);
    m_mainWidget = new QWidget(this);
    m_mainWidget->setObjectName("MainWidget");
    m_mainWidget->setStyleSheet("#MainWidget{border-image: url(:/tiny/lb_function_normal); }");
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_mainWidget);
    setLayout(layout);

    resize(55, 26);

    QToolButton *locationButton = new QToolButton(m_mainWidget);
    QToolButton *searchButton = new QToolButton(m_mainWidget);
    locationButton->setStyleSheet(TTK::UI::TinyBtnLocation);
    locationButton->setCursor(QCursor(Qt::PointingHandCursor));
    locationButton->setGeometry(1, 1, 24, 24);
    searchButton->setStyleSheet(TTK::UI::TinyBtnLocalSearch);
    searchButton->setCursor(QCursor(Qt::PointingHandCursor));
    searchButton->setGeometry(29, 1, 24, 24);

    connect(locationButton, SIGNAL(clicked()), MusicApplication::instance(), SLOT(currentPlayLocation()));
    connect(searchButton, SIGNAL(clicked()), parent, SLOT(showSearchWidget()));

    m_timer.setInterval(3 * MT_S2MS);
    connect(&m_timer, SIGNAL(timeout()), SLOT(leaveTimeout()));

    m_animation = new QPropertyAnimation(this, "windowOpacity", this);
    m_animation->setDuration(MT_S2MS);
    connect(m_animation, SIGNAL(finished()), SLOT(animationFinished()));

    m_timer.start();
}

MusicSongsListFunctionWidget::~MusicSongsListFunctionWidget()
{
    delete m_mainWidget;
    delete m_animation;
}

void MusicSongsListFunctionWidget::active()
{
    m_timer.stop();
    m_timer.start();
}

void MusicSongsListFunctionWidget::start(bool play, int end)
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

void MusicSongsListFunctionWidget::leaveTimeout()
{
    m_currentAnimationValue = 1;
    start(false, 0);
}

void MusicSongsListFunctionWidget::animationFinished()
{
    m_currentAnimationValue = m_animation->currentValue().toFloat();
    if(m_currentAnimationValue == 0)
    {
        QTimer::singleShot(MT_ONCE, this, SIGNAL(deleteObject()));
    }
}

void MusicSongsListFunctionWidget::enterEvent(QtEnterEvent *event)
{
    QLabel::enterEvent(event);
    if(m_currentAnimationValue != 0)
    {
        start(false, 1);
    }
    m_mainWidget->setStyleSheet("#MainWidget{border-image: url(:/tiny/lb_function_hover); }");
}

void MusicSongsListFunctionWidget::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    start(true, 0);
    m_mainWidget->setStyleSheet("#MainWidget{border-image: url(:/tiny/lb_function_normal); }");
}
