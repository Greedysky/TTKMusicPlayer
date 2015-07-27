#include "musicvideoplayer.h"
#include "musicvideoview.h"
#include "musicvideotablewidget.h"
#include "musiclocalsongsearchedit.h"
#include "musicobject.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>

MusicVideoPlayer::MusicVideoPlayer(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    m_topWidget = new QWidget(this);
    m_topWidget->setObjectName("#topWidget");
    m_topWidget->setStyleSheet("background-color:black;border:none;");

    QHBoxLayout *topLayout = new QHBoxLayout(m_topWidget);
    m_backButton = new QPushButton(this);
    m_backButton->setIcon(QIcon(":/video/back"));
    m_backButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_backButton->setIconSize(QSize(20, 20));
    m_afterButton = new QPushButton(this);
    m_afterButton->setIcon(QIcon(":/video/after"));
    m_afterButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_afterButton->setIconSize(QSize(20, 20));

    m_textLabel = new QLabel("Music Video", m_topWidget);
    m_textLabel->setStyleSheet("color:white;");
    m_searchEdit = new MusicLocalSongSearchEdit(m_topWidget);
    m_searchButton = new QPushButton(m_topWidget);
    m_searchButton->setIcon(QIcon(":/share/searchlineleft"));
    m_searchButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_searchButton->setIconSize(QSize(20, 20));
    topLayout->addWidget(m_backButton);
    topLayout->addWidget(m_afterButton);
    topLayout->addStretch();
    topLayout->addWidget(m_textLabel);
    topLayout->addStretch();
    topLayout->addWidget(m_searchEdit);
    topLayout->addWidget(m_searchButton);
    m_topWidget->setLayout(topLayout);

    m_stackedWidget = new QStackedWidget(this);
    m_stackedWidget->setStyleSheet("border:none;");
    layout->addWidget(m_topWidget);
    layout->addWidget(m_stackedWidget);
    setLayout(layout);

    m_videoView = new MusicVideoView(this);
    m_videoTable = new MusicVideoTableWidget(this);
    m_stackedWidget->addWidget(m_videoView);
    m_stackedWidget->addWidget(m_videoTable);
    m_stackedWidget->setCurrentIndex(0);

    connect(m_afterButton,SIGNAL(clicked(bool)),SLOT(afterButtonClicked()));
    connect(m_backButton,SIGNAL(clicked(bool)),SLOT(backButtonClicked()));
    connect(m_searchButton,SIGNAL(clicked(bool)),SLOT(searchButtonClicked()));
    connect(m_videoTable,SIGNAL(mvURLChanged(QString)),SLOT(mvURLChanged(QString)));
}

MusicVideoPlayer::~MusicVideoPlayer()
{
    delete m_backButton;
    delete m_textLabel;
    delete m_searchEdit;
    delete m_searchButton;
    delete m_topWidget;
    delete m_videoView;
    delete m_stackedWidget;
}

void MusicVideoPlayer::backButtonClicked()
{
    m_stackedWidget->setCurrentIndex(0);
}

void MusicVideoPlayer::afterButtonClicked()
{
    m_stackedWidget->setCurrentIndex(1);
}

void MusicVideoPlayer::searchButtonClicked()
{
    m_stackedWidget->setCurrentIndex(1);
    m_videoTable->startSearchQuery(m_searchEdit->text().trimmed());
}

void MusicVideoPlayer::mvURLChanged(const QString& data)
{
    m_videoView->setMedia(data);
    m_videoView->play();
    m_stackedWidget->setCurrentIndex(0);
}
