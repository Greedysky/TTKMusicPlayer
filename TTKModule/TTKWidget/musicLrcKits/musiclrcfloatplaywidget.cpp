#include "musiclrcfloatplaywidget.h"
#include "musicfunctionuiobject.h"
#include "musicapplication.h"
#include "musicwidgetheaders.h"

MusicLrcFloatPlayWidget::MusicLrcFloatPlayWidget(QWidget *parent)
    : MusicAbstractFloatWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(2, 2, 2, 2);
    layout->setSpacing(0);

    m_buttonPrevious = new QToolButton(this);
    m_buttonNext = new QToolButton(this);
    m_buttonKey = new QToolButton(this);

    layout->addWidget(m_buttonPrevious);
    layout->addWidget(m_buttonKey);
    layout->addWidget(m_buttonNext);
    setLayout(layout);

    m_buttonPrevious->setStyleSheet(TTK::UI::BtnPrevious);
    m_buttonNext->setStyleSheet(TTK::UI::BtnNext);

    m_buttonPrevious->setCursor(QCursor(Qt::PointingHandCursor));
    m_buttonKey->setCursor(QCursor(Qt::PointingHandCursor));
    m_buttonNext->setCursor(QCursor(Qt::PointingHandCursor));

    m_buttonPrevious->setFixedSize(44, 44);
    m_buttonNext->setFixedSize(44, 44);
    m_buttonKey->setFixedSize(44, 44);

    connect(m_buttonKey, SIGNAL(clicked()), SLOT(switchPlayState()));
    connect(m_buttonPrevious, SIGNAL(clicked()), MusicApplication::instance(), SLOT(playPrevious()));
    connect(m_buttonNext, SIGNAL(clicked()), MusicApplication::instance(), SLOT(playNext()));

    m_buttonKey->setToolTip(tr("Play"));
    m_buttonPrevious->setToolTip(tr("Previous"));
    m_buttonNext->setToolTip(tr("Next"));

    resizeGeometry(0, 0);
    setCurrentPlayState();

    show();
}

MusicLrcFloatPlayWidget::~MusicLrcFloatPlayWidget()
{
    delete m_buttonPrevious;
    delete m_buttonNext;
    delete m_buttonKey;
}

void MusicLrcFloatPlayWidget::resizeGeometry(int width, int height)
{
    m_rectEnter = QRect((width - this->width()) / 2, height - this->height(), 145, 60);
    m_rectLeave = QRect((width - this->width()) / 2, height - 5, 145, 60);

    setGeometry(m_rectLeave);

    if((width != 0 && height != 0) && !m_blockAnimation)
    {
        animationEnter();
    }
}

void MusicLrcFloatPlayWidget::switchPlayState()
{
    MusicApplication::instance()->switchPlayState();
    setCurrentPlayState();
}

void MusicLrcFloatPlayWidget::enterEvent(QtEnterEvent *event)
{
    MusicAbstractFloatWidget::enterEvent(event);
    setCurrentPlayState();
}

void MusicLrcFloatPlayWidget::setCurrentPlayState()
{
    m_buttonKey->setStyleSheet(MusicApplication::instance()->isPlaying() ? TTK::UI::BtnPause : TTK::UI::BtnPlay);
}
