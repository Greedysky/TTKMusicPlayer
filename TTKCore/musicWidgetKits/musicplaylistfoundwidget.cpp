#include "musicplaylistfoundwidget.h"
#include "musicsourcedownloadthread.h"
#include "musicdownloadquerywyplaylistthread.h"
#include "musicplaylistfoundinfowidget.h"
#include "musicdownloadqueryfactory.h"
#include "musictinyuiobject.h"
#include "musicplaylistfoundcategorywidget.h"

#include <QPushButton>
#include <QGridLayout>
#include <QScrollArea>
#include <QStackedWidget>

#define MIN_LABEL_SIZE  150
#define MAX_LABEL_SIZE  200

MusicPlaylistFoundItemWidget::MusicPlaylistFoundItemWidget(QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(MIN_LABEL_SIZE, MAX_LABEL_SIZE);

    m_topListenButton = new QPushButton(this);
    m_topListenButton->setGeometry(0, 0, MIN_LABEL_SIZE, 20);
    m_topListenButton->setIcon(QIcon(":/tiny/btn_listen_hover"));
    m_topListenButton->setText(" - ");
    m_topListenButton->setStyleSheet(MusicUIObject::MBackgroundStyle04 + MusicUIObject::MColorStyle01);

    m_playButton = new QPushButton(this);
    m_playButton->setGeometry(110, 110, 30, 30);
    m_playButton->setCursor(Qt::PointingHandCursor);
    m_playButton->setStyleSheet(MusicUIObject::MKGTinyBtnPlaylist);
    connect(m_playButton, SIGNAL(clicked()), SLOT(currentPlayListClicked()));

    m_iconLabel = new QLabel(this);
    m_iconLabel->setGeometry(0, 0, MIN_LABEL_SIZE, MIN_LABEL_SIZE);

    m_nameLabel = new QLabel(this);
    m_nameLabel->setGeometry(0, 150, MIN_LABEL_SIZE, 25);
    m_nameLabel->setText(" - ");

    m_creatorLabel = new QLabel(this);
    m_creatorLabel->setGeometry(0, 175, MIN_LABEL_SIZE, 25);
    m_creatorLabel->setText("by anonymous");
}

MusicPlaylistFoundItemWidget::~MusicPlaylistFoundItemWidget()
{
    delete m_topListenButton;
    delete m_playButton;
    delete m_iconLabel;
    delete m_nameLabel;
    delete m_creatorLabel;
}

QString MusicPlaylistFoundItemWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicPlaylistFoundItemWidget::setMusicPlaylistItem(const MusicPlaylistItem &item)
{
    m_itemData = item;
    m_nameLabel->setToolTip(item.m_name);
    m_nameLabel->setText(MusicUtils::Widget::elidedText(m_nameLabel->font(), m_nameLabel->toolTip(),
                                                        Qt::ElideRight, MIN_LABEL_SIZE));
    m_creatorLabel->setToolTip("by " + item.m_nickname);
    m_creatorLabel->setText(MusicUtils::Widget::elidedText(m_creatorLabel->font(), m_creatorLabel->toolTip(),
                                                           Qt::ElideRight, MIN_LABEL_SIZE));
    bool ok = false;
    int count = item.m_playCount.toInt(&ok);
    if(ok)
    {
        if(count >= 10000)
        {
            m_topListenButton->setText(tr("%1Thous").arg(count/10000));
        }
        else
        {
            m_topListenButton->setText(QString::number(count));
        }
    }
    else
    {
        m_topListenButton->setText(item.m_playCount);
    }

    MusicSourceDownloadThread *download = new MusicSourceDownloadThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    download->startToDownload(item.m_coverUrl);
}

void MusicPlaylistFoundItemWidget::downLoadFinished(const QByteArray &data)
{
    QPixmap pix;
    pix.loadFromData(data);
    m_iconLabel->setPixmap(pix.scaled(m_iconLabel->size()));
    m_topListenButton->raise();
    m_playButton->raise();
}

void MusicPlaylistFoundItemWidget::currentPlayListClicked()
{
    emit currentPlayListClicked(m_itemData);
}



MusicPlaylistFoundWidget::MusicPlaylistFoundWidget(QWidget *parent)
    : MusicFoundAbstractWidget(parent)
{
    m_container = new QStackedWidget(this);
    layout()->removeWidget(m_mainWindow);
    layout()->addWidget(m_container);
    m_container->addWidget(m_mainWindow);

    m_firstInit = false;
    m_infoWidget = nullptr;
    m_gridLayout = nullptr;
    m_categoryButton = nullptr;
    m_downloadThread = M_DOWNLOAD_QUERY_PTR->getPlaylistThread(this);
    connect(m_downloadThread, SIGNAL(createPlaylistItems(MusicPlaylistItem)),
                              SLOT(queryAllFinished(MusicPlaylistItem)));
}

MusicPlaylistFoundWidget::~MusicPlaylistFoundWidget()
{
    delete m_infoWidget;
    delete m_gridLayout;
//    delete m_container;
    delete m_categoryButton;
    delete m_downloadThread;
}

QString MusicPlaylistFoundWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicPlaylistFoundWidget::setSongName(const QString &name)
{
    MusicFoundAbstractWidget::setSongName(name);
    m_downloadThread->startSearchSong(MusicDownLoadQueryThreadAbstract::MovieQuery, QString());
}

void MusicPlaylistFoundWidget::resizeWindow()
{
    if(!m_resizeWidget.isEmpty())
    {
        if(m_gridLayout)
        {
            for(int i=0; i<m_resizeWidget.count(); ++i)
            {
                m_gridLayout->removeWidget(m_resizeWidget[i]);
            }

            int lineNumber = width()/MAX_LABEL_SIZE;
            for(int i=0; i<m_resizeWidget.count(); ++i)
            {
                m_gridLayout->addWidget(m_resizeWidget[i], i/lineNumber, i%lineNumber, Qt::AlignCenter);
            }
        }
    }
}

void MusicPlaylistFoundWidget::queryAllFinished(const MusicPlaylistItem &item)
{
    if(!m_firstInit)
    {
        delete m_statusLabel;
        m_statusLabel = nullptr;

        m_container->removeWidget(m_mainWindow);
        QScrollArea *scrollArea = new QScrollArea(this);
        scrollArea->setStyleSheet(MusicUIObject::MScrollBarStyle01);
        scrollArea->setWidgetResizable(true);
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setAlignment(Qt::AlignLeft);
        scrollArea->setWidget(m_mainWindow);
        m_container->addWidget(scrollArea);

        m_firstInit = true;
        QHBoxLayout *mainlayout = MStatic_cast(QHBoxLayout*, m_mainWindow->layout());
        QWidget *containTopWidget = new QWidget(m_mainWindow);
        QHBoxLayout *containTopLayout  = new QHBoxLayout(containTopWidget);
        containTopLayout->setContentsMargins(30, 0, 30, 0);
        m_categoryButton = new MusicPlaylistFoundCategoryWidget(m_mainWindow);
        m_categoryButton->setCategory(m_downloadThread->getQueryServer(), this);
        containTopLayout->addWidget(m_categoryButton);
        containTopLayout->addStretch(1);
        foreach(const QString &data, QStringList() << tr("Recommend") << tr("Top") << tr("Hot") << tr("New"))
        {
            QLabel *l = new QLabel(data, containTopWidget);
            l->setStyleSheet(QString("QLabel::hover{%1}").arg(MusicUIObject::MColorStyle08));
            QFrame *hline = new QFrame(containTopWidget);
            hline->setFrameShape(QFrame::VLine);
            hline->setStyleSheet(MusicUIObject::MColorStyle06);
            containTopLayout->addWidget(l);
            containTopLayout->addWidget(hline);
        }

        QFrame *line = new QFrame(m_mainWindow);
        line->setFrameShape(QFrame::HLine);
        line->setStyleSheet(MusicUIObject::MColorStyle06);

        QWidget *containWidget = new QWidget(m_mainWindow);
        m_gridLayout = new QGridLayout(containWidget);
        m_gridLayout->setVerticalSpacing(35);
        containWidget->setLayout(m_gridLayout);

        mainlayout->addWidget(containTopWidget);
        mainlayout->addWidget(line);
        mainlayout->addWidget(containWidget);
        mainlayout->addStretch(1);
    }

    MusicPlaylistFoundItemWidget *label = new MusicPlaylistFoundItemWidget(this);
    connect(label, SIGNAL(currentPlayListClicked(MusicPlaylistItem)), SLOT(currentPlayListClicked(MusicPlaylistItem)));
    label->setMusicPlaylistItem(item);

    int lineNumber = width()/MAX_LABEL_SIZE;
    m_gridLayout->addWidget(label, m_resizeWidget.count()/lineNumber,
                                   m_resizeWidget.count()%lineNumber, Qt::AlignCenter);
    m_resizeWidget << label;
}

void MusicPlaylistFoundWidget::currentPlayListClicked(const MusicPlaylistItem &item)
{
    delete m_infoWidget;
    m_infoWidget = new MusicPlaylistFoundInfoWidget(this);
    m_infoWidget->setQueryInput(M_DOWNLOAD_QUERY_PTR->getPlaylistThread(this));
    m_infoWidget->setMusicPlaylistItem(item, this);
    m_container->addWidget(m_infoWidget);
    m_container->setCurrentIndex(1);
}

void MusicPlaylistFoundWidget::backToPlayListMenu()
{
    m_container->setCurrentIndex(0);
}

void MusicPlaylistFoundWidget::categoryChanged(const PlaylistCategoryItem &category)
{
    if(m_categoryButton)
    {
        m_categoryButton->setText(category.m_name);
        m_categoryButton->closeMenu();

        while(!m_resizeWidget.isEmpty())
        {
            QWidget *w = m_resizeWidget.takeLast();
            m_gridLayout->removeWidget(w);
            delete w;
        }
        m_downloadThread->startSearchSong(MusicDownLoadQueryThreadAbstract::MovieQuery, category.m_id);
    }
}

void MusicPlaylistFoundWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeWindow();
}
