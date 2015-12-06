#include "musicvideoplaywidget.h"
#include "musicvideoview.h"
#include "musicvideotablewidget.h"
#include "musiclocalsongsearchedit.h"
#include "musicobject.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>

MusicVideoPlayWidget::MusicVideoPlayWidget(bool popup, QWidget *parent)
    : MusicAbstractMoveWidget(parent)
{
    if(popup)
    {
        resize(525, 455);
    }

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    m_topWidget = new QWidget(this);
    m_topWidget->setObjectName("#topWidget");
    m_topWidget->setStyleSheet(MusicUIObject::MCustomStyle19 + \
                               MusicUIObject::MCustomStyle20);
    QHBoxLayout *topLayout = new QHBoxLayout(m_topWidget);
    topLayout->setSpacing(0);
    m_backButton = new QPushButton(this);
    m_backButton->setIcon(QIcon(":/video/back"));
    m_backButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_backButton->setIconSize(QSize(18, 18));
    m_afterButton = new QPushButton(this);
    m_afterButton->setIcon(QIcon(":/video/after"));
    m_afterButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_afterButton->setIconSize(QSize(18, 18));

    m_textLabel = new QLabel(m_topWidget);
    m_textLabel->setStyleSheet(MusicUIObject::MCustomStyle11);
    m_searchEdit = new MusicLocalSongSearchEdit(m_topWidget);
    m_searchEdit->setFixedHeight(18);
    m_searchButton = new QPushButton(m_topWidget);
    m_searchButton->setIcon(QIcon(":/share/searchlineleft"));
    m_searchButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_searchButton->setIconSize(QSize(18, 18));
    topLayout->addWidget(m_backButton);
    topLayout->addWidget(m_afterButton);
    topLayout->addStretch();
    topLayout->addWidget(m_textLabel);
    topLayout->addStretch();
    topLayout->addWidget(m_searchEdit);
    topLayout->addWidget(m_searchButton);
    m_topWidget->setLayout(topLayout);
    m_topWidget->setFixedHeight(40);

    m_stackedWidget = new QStackedWidget(this);
    m_stackedWidget->setStyleSheet(MusicUIObject::MCustomStyle20);
    layout->addWidget(m_topWidget);
    layout->addWidget(m_stackedWidget);
    setLayout(layout);

    m_videoTable = new MusicVideoTableWidget(this);
    m_videoView = new MusicVideoView(popup, this);
    m_stackedWidget->addWidget(m_videoView);
    m_stackedWidget->addWidget(m_videoTable);
    m_stackedWidget->setCurrentIndex(0);

    connect(m_afterButton, SIGNAL(clicked(bool)), SLOT(afterButtonClicked()));
    connect(m_backButton, SIGNAL(clicked(bool)), SLOT(backButtonClicked()));
    connect(m_searchButton,SIGNAL(clicked(bool)), SLOT(searchButtonClicked()));
    connect(m_videoTable, SIGNAL(mvURLNameChanged(QString,QString)),
                          SLOT(mvURLNameChanged(QString,QString)));
    connect(m_videoTable, SIGNAL(restartSearchQuery(QString)),
                          SLOT(musicResearchButtonSearched(QString)));

}

MusicVideoPlayWidget::~MusicVideoPlayWidget()
{
    delete m_backButton;
    delete m_textLabel;
    delete m_searchEdit;
    delete m_searchButton;
    delete m_topWidget;
    delete m_videoView;
    delete m_stackedWidget;
}

void MusicVideoPlayWidget::resizeWindow(bool resize)
{
    if(resize)
    {
        showFullScreen();
    }
    else
    {
        setGeometry(250, 150, 525, 455);
    }
    m_videoView->resizeWindow(resize, size());
    m_videoTable->resizeWindow(size().width() / 525.0);
}

void MusicVideoPlayWidget::backButtonClicked()
{
    m_stackedWidget->setCurrentIndex(0);
}

void MusicVideoPlayWidget::afterButtonClicked()
{
    m_stackedWidget->setCurrentIndex(1);
}

void MusicVideoPlayWidget::searchButtonClicked()
{
    musicResearchButtonSearched(m_searchEdit->text().trimmed());
}

void MusicVideoPlayWidget::musicResearchButtonSearched(const QString &name)
{
    m_searchEdit->setText(name);
    m_stackedWidget->setCurrentIndex(1);
    m_videoTable->startSearchQuery(name);
}

void MusicVideoPlayWidget::mvURLChanged(const QString &data)
{
    m_videoView->setMedia(data);
    m_videoView->play();
    m_stackedWidget->setCurrentIndex(0);
}

void MusicVideoPlayWidget::mvURLNameChanged(const QString &name, const QString &data)
{
    m_textLabel->setText(QFontMetrics(font()).elidedText(name, Qt::ElideRight, 275));
    mvURLChanged(data);
}
