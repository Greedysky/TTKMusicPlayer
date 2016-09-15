#include "musiclrccommentswidget.h"
#include "musicfunctionuiobject.h"
#include "musicuiobject.h"

#include <QBoxLayout>
#include <QPushButton>

MusicLrcCommentsWidget::MusicLrcCommentsWidget(QWidget *parent)
    : QLabel(parent)
{
    setObjectName("MainWidnow");
    setStyleSheet(QString("#MainWidnow{%1}").arg(MusicUIObject::MBackgroundStyle19));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QWidget *topWidget = new QWidget(this);
    topWidget->setFixedHeight(40);
    QHBoxLayout *topWidgetLayout = new QHBoxLayout(topWidget);
    topWidgetLayout->setContentsMargins(0, 0, 10, 0);

    m_topLabel = new QLabel(topWidget);
    m_topLabel->setAlignment(Qt::AlignCenter);
    topWidgetLayout->addWidget(m_topLabel);
    QPushButton *closeButton = new QPushButton(this);
    closeButton->setFixedSize(14, 14);
    closeButton->setStyleSheet(MusicUIObject::MKGBtnPClose);
    closeButton->setCursor(QCursor(Qt::PointingHandCursor));

    topWidgetLayout->addWidget(closeButton);
    topWidget->setLayout(topWidgetLayout);

    connect(closeButton, SIGNAL(clicked()), SLOT(close()));

//    QWidget *contentsWidget = new QWidget(this);
//    m_layout = new QVBoxLayout(contentsWidget);
//    m_layout->setContentsMargins(0, 0, 0 ,0);
//    m_layout->setSpacing(0);
//    contentsWidget->setLayout(m_layout);

//    m_scrollArea = new QScrollArea(this);
//    m_scrollArea->setStyleSheet(MusicUIObject::MScrollBarStyle01);
//    m_scrollArea->setWidgetResizable(true);
//    m_scrollArea->setFrameShape(QFrame::NoFrame);
//    m_scrollArea->setAlignment(Qt::AlignLeft);
//    m_scrollArea->setWidget(contentsWidget);

    mainLayout->addWidget(topWidget);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

MusicLrcCommentsWidget::~MusicLrcCommentsWidget()
{
    delete m_topLabel;
}

QString MusicLrcCommentsWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicLrcCommentsWidget::setCurrentSongName(const QString &name)
{
    m_currentSongName = name;
    m_topLabel->setText(tr("<span style='font-size:15pt;color:#AAAAAA;font-style:bold;'>\"%1\" Commit</span>"
                           "<span style='font-size:11pt;color:#666666;font-style:bold;'> (Just %2 Commits)</span>").arg(name).arg(0));
}

void MusicLrcCommentsWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}

void MusicLrcCommentsWidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicLrcCommentsWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicLrcCommentsWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicLrcCommentsWidget::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event);
}
