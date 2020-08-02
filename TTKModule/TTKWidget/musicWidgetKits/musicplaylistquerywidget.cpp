#include "musicplaylistquerywidget.h"
#include "musicdownloadsourcerequest.h"
#include "musicqueryplaylistrequest.h"
#include "musicplaylistqueryinfowidget.h"
#include "musicdownloadqueryfactory.h"
#include "musictinyuiobject.h"
#include "musicplaylistquerycategorypopwidget.h"
#include "musicpagingwidgetobject.h"
#include "musicimageutils.h"

#include <qmath.h>
#include <QGridLayout>

#define WIDTH_LABEL_SIZE   150
#define HEIGHT_LABEL_SIZE  200
#define LINE_SPACING_SIZE  200

MusicPlaylistQueryItemWidget::MusicPlaylistQueryItemWidget(QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(WIDTH_LABEL_SIZE, HEIGHT_LABEL_SIZE);

    m_topListenButton = new QPushButton(this);
    m_topListenButton->setGeometry(0, 0, WIDTH_LABEL_SIZE, 20);
    m_topListenButton->setIcon(QIcon(":/tiny/btn_listen_hover"));
    m_topListenButton->setText(" - ");
    m_topListenButton->setStyleSheet(MusicUIObject::MQSSBackgroundStyle04 + MusicUIObject::MQSSColorStyle01);

    m_playButton = new QPushButton(this);
    m_playButton->setGeometry(110, 110, 30, 30);
    m_playButton->setCursor(Qt::PointingHandCursor);
    m_playButton->setStyleSheet(MusicUIObject::MQSSTinyBtnPlaylist);
    connect(m_playButton, SIGNAL(clicked()), SLOT(currentItemClicked()));

#ifdef Q_OS_UNIX
    m_topListenButton->setFocusPolicy(Qt::NoFocus);
    m_playButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_iconLabel = new QLabel(this);
    m_iconLabel->setGeometry(0, 0, WIDTH_LABEL_SIZE, WIDTH_LABEL_SIZE);

    m_nameLabel = new QLabel(this);
    m_nameLabel->setGeometry(0, 150, WIDTH_LABEL_SIZE, 25);
    m_nameLabel->setText(" - ");

    m_creatorLabel = new QLabel(this);
    m_creatorLabel->setGeometry(0, 175, WIDTH_LABEL_SIZE, 25);
    m_creatorLabel->setText("by anonymous");
}

MusicPlaylistQueryItemWidget::~MusicPlaylistQueryItemWidget()
{
    delete m_topListenButton;
    delete m_playButton;
    delete m_iconLabel;
    delete m_nameLabel;
    delete m_creatorLabel;
}

void MusicPlaylistQueryItemWidget::setMusicResultsItem(const MusicResultsItem &item)
{
    m_itemData = item;
    m_nameLabel->setToolTip(item.m_name);
    m_nameLabel->setText(MusicUtils::Widget::elidedText(m_nameLabel->font(), m_nameLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));
    m_creatorLabel->setToolTip("by " + item.m_nickName);
    m_creatorLabel->setText(MusicUtils::Widget::elidedText(m_creatorLabel->font(), m_creatorLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));

    bool ok = false;
    const int count = item.m_playCount.toInt(&ok);
    if(ok)
    {
        if(count >= 10000)
        {
            m_topListenButton->setText(tr("%1Thous").arg(count / 10000));
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

    MusicDownloadSourceRequest *download = new MusicDownloadSourceRequest(this);
    connect(download, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != COVER_URL_NULL)
    {
        download->startToDownload(item.m_coverUrl);
    }
}

void MusicPlaylistQueryItemWidget::downLoadFinished(const QByteArray &data)
{
    MusicImageRenderer *render = new MusicImageRenderer(this);
    connect(render, SIGNAL(renderFinished(QImage)), SLOT(renderFinished(QImage)));
    render->setInputData(data, m_iconLabel->size());
    render->start();
}

void MusicPlaylistQueryItemWidget::renderFinished(const QImage &data)
{
    m_iconLabel->setPixmap(QPixmap::fromImage(data));
    m_topListenButton->raise();
    m_playButton->raise();
}

void MusicPlaylistQueryItemWidget::currentItemClicked()
{
    Q_EMIT currentItemClicked(m_itemData);
}



MusicPlaylistQueryWidget::MusicPlaylistQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent)
{
    m_container->show();
    layout()->removeWidget(m_mainWindow);
    layout()->addWidget(m_container);
    m_container->addWidget(m_mainWindow);

    m_firstInit = false;
    m_categoryChanged = false;
    m_infoWidget = nullptr;
    m_gridLayout = nullptr;
    m_categoryButton = nullptr;
    m_pagingWidgetObject = nullptr;
    m_downloadRequest = M_DOWNLOAD_QUERY_PTR->getPlaylistRequest(this);
    connect(m_downloadRequest, SIGNAL(createPlaylistItem(MusicResultsItem)), SLOT(createPlaylistItem(MusicResultsItem)));
}

MusicPlaylistQueryWidget::~MusicPlaylistQueryWidget()
{
    delete m_infoWidget;
    delete m_gridLayout;
    delete m_categoryButton;
    delete m_downloadRequest;
    delete m_pagingWidgetObject;
}

void MusicPlaylistQueryWidget::setSongName(const QString &name)
{
    MusicAbstractItemQueryWidget::setSongName(name);
    m_downloadRequest->startToSearch(MusicAbstractQueryRequest::OtherQuery, QString());
}

void MusicPlaylistQueryWidget::setSongNameById(const QString &id)
{
    setSongName(id);

    MusicResultsItem item;
    item.m_id = id;
    currentPlaylistClicked(item);
}

void MusicPlaylistQueryWidget::resizeWindow()
{
    if(m_infoWidget)
    {
        m_infoWidget->resizeWindow();
    }

    if(!m_resizeWidgets.isEmpty() && m_gridLayout)
    {
        for(int i=0; i<m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->removeWidget(m_resizeWidgets[i]);
        }

        const int lineNumber = width() / LINE_SPACING_SIZE;
        for(int i=0; i<m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->addWidget(m_resizeWidgets[i], i/lineNumber, i%lineNumber, Qt::AlignCenter);
        }
    }
}

void MusicPlaylistQueryWidget::createPlaylistItem(const MusicResultsItem &item)
{
    if(!m_firstInit)
    {
        delete m_statusLabel;
        m_statusLabel = nullptr;

        m_container->removeWidget(m_mainWindow);
        QScrollArea *scrollArea = new QScrollArea(this);
        scrollArea->setStyleSheet(MusicUIObject::MQSSScrollBarStyle01);
        scrollArea->setWidgetResizable(true);
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setAlignment(Qt::AlignLeft);
        scrollArea->setWidget(m_mainWindow);
        m_container->addWidget(scrollArea);

        m_firstInit = true;
        QHBoxLayout *mainlayout = TTKStatic_cast(QHBoxLayout*, m_mainWindow->layout());
        QWidget *containTopWidget = new QWidget(m_mainWindow);
        QHBoxLayout *containTopLayout  = new QHBoxLayout(containTopWidget);
        containTopLayout->setContentsMargins(30, 0, 30, 0);
        m_categoryButton = new MusicPlaylistFoundCategoryPopWidget(m_mainWindow);
        m_categoryButton->setCategory(m_downloadRequest->getQueryServer(), this);
        containTopLayout->addWidget(m_categoryButton);
        containTopLayout->addStretch(1);
        foreach(const QString &data, QStringList() << tr("Recommend") << tr("Top") << tr("Hot") << tr("New"))
        {
            QLabel *l = new QLabel(data, containTopWidget);
            l->setStyleSheet(QString("QLabel::hover{%1}").arg(MusicUIObject::MQSSColorStyle08));
            QFrame *hline = new QFrame(containTopWidget);
            hline->setFrameShape(QFrame::VLine);
            hline->setStyleSheet(MusicUIObject::MQSSColorStyle06);
            containTopLayout->addWidget(l);
            containTopLayout->addWidget(hline);
        }
        containTopWidget->setLayout(containTopLayout);

        QFrame *line = new QFrame(m_mainWindow);
        line->setFrameShape(QFrame::HLine);
        line->setStyleSheet(MusicUIObject::MQSSColorStyle06);

        QWidget *containWidget = new QWidget(m_mainWindow);
        m_gridLayout = new QGridLayout(containWidget);
        m_gridLayout->setVerticalSpacing(35);
        containWidget->setLayout(m_gridLayout);

        mainlayout->addWidget(containTopWidget);
        mainlayout->addWidget(line);
        mainlayout->addWidget(containWidget);

        m_pagingWidgetObject = new MusicPagingWidgetObject(m_mainWindow);
        connect(m_pagingWidgetObject, SIGNAL(clicked(int)), SLOT(buttonClicked(int)));

        const int total = ceil(m_downloadRequest->getPageTotal() * 1.0 / m_downloadRequest->getPageSize());
        mainlayout->addWidget(m_pagingWidgetObject->createPagingWidget(m_mainWindow, total));
        mainlayout->addStretch(1);
    }

    if(m_categoryChanged && m_pagingWidgetObject)
    {
        m_categoryChanged = false;
        const int total = ceil(m_downloadRequest->getPageTotal() * 1.0 / m_downloadRequest->getPageSize());
        m_pagingWidgetObject->reset(total);
    }

    MusicPlaylistQueryItemWidget *label = new MusicPlaylistQueryItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(MusicResultsItem)), SLOT(currentPlaylistClicked(MusicResultsItem)));
    label->setMusicResultsItem(item);

    const int lineNumber = width() / LINE_SPACING_SIZE;
    m_gridLayout->addWidget(label, m_resizeWidgets.count() / lineNumber, m_resizeWidgets.count() % lineNumber, Qt::AlignCenter);
    m_resizeWidgets << label;
}

void MusicPlaylistQueryWidget::currentPlaylistClicked(const MusicResultsItem &item)
{
    delete m_infoWidget;
    m_infoWidget = new MusicPlaylistQueryInfoWidget(this);
    MusicQueryPlaylistRequest *d = TTKStatic_cast(MusicQueryPlaylistRequest*, M_DOWNLOAD_QUERY_PTR->getPlaylistRequest(this));
    MusicResultsItem it(item);
    if(it.isEmpty())
    {
        d->getPlaylistInfo(it);
    }
    m_infoWidget->setQueryInput(d);
    m_infoWidget->setMusicResultsItem(it, this);
    m_container->addWidget(m_infoWidget);
    m_container->setCurrentIndex(PLAYLIST_WINDOW_INDEX_1);
}

void MusicPlaylistQueryWidget::backToPlaylistMenu()
{
    m_container->setCurrentIndex(PLAYLIST_WINDOW_INDEX_0);
}

void MusicPlaylistQueryWidget::categoryChanged(const MusicResultsCategoryItem &category)
{
    if(m_categoryButton)
    {
        m_songNameFull.clear();
        m_categoryChanged = true;
        m_categoryButton->setText(category.m_name);
        m_categoryButton->closeMenu();

        while(!m_resizeWidgets.isEmpty())
        {
            QWidget *w = m_resizeWidgets.takeLast();
            m_gridLayout->removeWidget(w);
            delete w;
        }
        m_downloadRequest->startToSearch(MusicAbstractQueryRequest::OtherQuery, category.m_id);
    }
}

void MusicPlaylistQueryWidget::buttonClicked(int index)
{
    while(!m_resizeWidgets.isEmpty())
    {
        QWidget *w = m_resizeWidgets.takeLast();
        m_gridLayout->removeWidget(w);
        delete w;
    }

    const int total = ceil(m_downloadRequest->getPageTotal() * 1.0 / m_downloadRequest->getPageSize());
    m_pagingWidgetObject->paging(index, total);
    m_downloadRequest->startToPage(m_pagingWidgetObject->currentIndex() - 1);
}
