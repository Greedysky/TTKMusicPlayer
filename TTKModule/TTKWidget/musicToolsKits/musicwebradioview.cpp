#include "musicwebradioview.h"
#include "musicwidgetheaders.h"
#include "musicrightareawidget.h"
#include "musicwebfmradioplaywidget.h"
#include "musicttkfmradioplaywidget.h"

MusicWebRadioView::MusicWebRadioView(QWidget *parent)
    : QWidget(parent),
      m_radio(nullptr)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QPushButton *fm = new QPushButton(this);
    fm->setIcon(QIcon(":/tiny/btn_fm_radio"));
    fm->setToolTip(tr("FMRadio"));
    fm->setCursor(QCursor(Qt::PointingHandCursor));
    fm->setStyleSheet(TTK::UI::PushButtonStyle01);
    fm->setFixedWidth(40);

    QPushButton *tk = new QPushButton(this);
    tk->setIcon(QIcon(":/tiny/btn_fm_radio"));
    tk->setToolTip(tr("FMRadio"));
    tk->setCursor(QCursor(Qt::PointingHandCursor));
    tk->setStyleSheet(TTK::UI::PushButtonStyle01);
    tk->setFixedWidth(40);

    QPushButton *dj = new QPushButton(this);
    dj->setIcon(QIcon(":/tiny/btn_dj_radio"));
    dj->setToolTip(tr("DJRadio"));
    dj->setCursor(QCursor(Qt::PointingHandCursor));
    dj->setStyleSheet(TTK::UI::PushButtonStyle01);
    dj->setFixedWidth(40);

    QPushButton *mv = new QPushButton(this);
    mv->setIcon(QIcon(":/tiny/btn_mv_radio"));
    mv->setToolTip(tr("MVRadio"));
    mv->setCursor(QCursor(Qt::PointingHandCursor));
    mv->setStyleSheet(TTK::UI::PushButtonStyle01);
    mv->setFixedWidth(40);
#ifdef Q_OS_UNIX
    fm->setFocusPolicy(Qt::NoFocus);
    tk->setFocusPolicy(Qt::NoFocus);
    dj->setFocusPolicy(Qt::NoFocus);
    mv->setFocusPolicy(Qt::NoFocus);
#endif

    layout->addWidget(fm);
    layout->addWidget(tk);
    layout->addWidget(dj);
    layout->addWidget(mv);
    setLayout(layout);

    connect(fm, SIGNAL(clicked()), SLOT(openFMRadioWindow()));
    connect(tk, SIGNAL(clicked()), SLOT(openTKRadioWindow()));
    connect(dj, SIGNAL(clicked()), SLOT(openDJRadioWindow()));
    connect(mv, SIGNAL(clicked()), SLOT(openMVRadioWindow()));
}

MusicWebRadioView::~MusicWebRadioView()
{
    delete m_radio;
}

void MusicWebRadioView::openFMRadioWindow()
{
    delete m_radio;
    MusicWebFMRadioPlayWidget *w = new MusicWebFMRadioPlayWidget(this);
    m_radio = w;
    w->show();
}

void MusicWebRadioView::openTKRadioWindow()
{
    delete m_radio;
    MusicTTKFMRadioPlayWidget *w = new MusicTTKFMRadioPlayWidget(this);
    m_radio = w;
    w->show();
}

void MusicWebRadioView::openDJRadioWindow()
{
    MusicRightAreaWidget::instance()->functionClicked(MusicRightAreaWidget::WebDJRadioWidget);
}

void MusicWebRadioView::openMVRadioWindow()
{
    MusicRightAreaWidget::instance()->functionClicked(MusicRightAreaWidget::WebMVRadioWidget);
}
