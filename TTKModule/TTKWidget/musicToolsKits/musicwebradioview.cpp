#include "musicwebradioview.h"
#include "musicwebmusicradiowidget.h"
#include "musicuiobject.h"
#include "musicrightareawidget.h"
#include "musicwidgetheaders.h"

MusicWebRadioView::MusicWebRadioView(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    m_musicRadio = new MusicWebMusicRadioWidget(this);

    QWidget *bottomWidget = new QWidget(this);
    bottomWidget->setFixedHeight(40);
    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomWidget);
    bottomLayout->setSpacing(0);
    bottomLayout->setContentsMargins(5, 0, 0, 0);

    QPushButton *dj = new QPushButton(this);
    dj->setIcon(QIcon(":/tiny/btn_dj_radio"));
    dj->setToolTip(tr("DJRadio"));
    dj->setCursor(QCursor(Qt::PointingHandCursor));
    dj->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    dj->setFixedWidth(40);

    QPushButton *mv = new QPushButton(this);
    mv->setIcon(QIcon(":/tiny/btn_mv_radio"));
    mv->setToolTip(tr("MVRadio"));
    mv->setCursor(QCursor(Qt::PointingHandCursor));
    mv->setStyleSheet(MusicUIObject::MPushButtonStyle01);
    mv->setFixedWidth(40);
#ifdef Q_OS_UNIX
    dj->setFocusPolicy(Qt::NoFocus);
    mv->setFocusPolicy(Qt::NoFocus);
#endif

    bottomLayout->addStretch(1);
    bottomLayout->addWidget(dj);
    bottomLayout->addWidget(mv);
    bottomWidget->setLayout(bottomLayout);

    layout->addWidget(m_musicRadio);
    layout->addWidget(bottomWidget);
    setLayout(layout);

    connect(dj, SIGNAL(clicked()), SLOT(openDJRadioWindow()));
    connect(mv, SIGNAL(clicked()), SLOT(openMVRadioWindow()));
}

MusicWebRadioView::~MusicWebRadioView()
{
    delete m_musicRadio;
}

void MusicWebRadioView::init(int index)
{
    m_musicRadio->initListItems(index);
}

void MusicWebRadioView::openDJRadioWindow()
{
    MusicRightAreaWidget::instance()->musicFunctionClicked(MusicRightAreaWidget::WebDJRadioWidget);
}

void MusicWebRadioView::openMVRadioWindow()
{
    MusicRightAreaWidget::instance()->musicFunctionClicked(MusicRightAreaWidget::WebMVRadioWidget);
}

void MusicWebRadioView::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}
