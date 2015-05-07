#include "musicvideoplaylistwidget.h"
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QModelIndex>

MusicVideoPlayListWidget::MusicVideoPlayListWidget(QWidget *parent) : QWidget(parent)
{
    setFixedSize(330,220);
    QVBoxLayout *box = new QVBoxLayout(this);
    box->setMargin(0);
    box->setSpacing(0);

    m_playList = new QListWidget(this);
    m_playList->setSpacing(3);
    m_playList->setStyleSheet("color:rgb(200,111,30)");
    m_barWidget = new QWidget(this);
    m_barWidget->setFixedHeight(20);
    m_barWidget->setStyleSheet("background:rgba(120,20,255,120)");
    m_closeButton = new QPushButton(this);
    m_closeButton->setGeometry(310,0,20,20);
    m_closeButton->setIcon(QIcon(QPixmap(":/share/searchclosed")));
    m_closeButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_closeButton->setToolTip(tr("close"));
    m_closeButton->setStyleSheet("background:transparent");
    box->addWidget(m_barWidget);
    box->addWidget(m_playList);
    setLayout(box);
    connect(m_closeButton,SIGNAL(clicked()),SLOT(close()));
    connect(m_playList,SIGNAL(doubleClicked(QModelIndex)),
                parent,SLOT(listDoubleClicked(QModelIndex)));
}

MusicVideoPlayListWidget::~MusicVideoPlayListWidget()
{
    delete m_closeButton;
    delete m_barWidget;
    delete m_playList;
}

void MusicVideoPlayListWidget::addItems(const QStringList& lis)
{
    m_playList->addItems(lis);
}

void MusicVideoPlayListWidget::setCurrentIndex(int index)
{
    m_playList->setCurrentRow(index);
}
