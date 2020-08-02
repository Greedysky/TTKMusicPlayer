#include "musicwebdjradioquerywidget.h"
#include "musicdjradioprogramcategoryrequest.h"
#include "musictinyuiobject.h"
#include "musicpagingwidgetobject.h"
#include "musicsettingmanager.h"
#include "musicdownloadqueryfactory.h"
#include "musicdownloadsourcerequest.h"
#include "musicwebdjradioinfowidget.h"
#include "musicimageutils.h"

#include <qmath.h>

#define WIDTH_LABEL_SIZE   150
#define HEIGHT_LABEL_SIZE  200
#define LINE_SPACING_SIZE  200

MusicWebDJRadioQueryItemWidget::MusicWebDJRadioQueryItemWidget(QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(WIDTH_LABEL_SIZE, HEIGHT_LABEL_SIZE);

    m_playButton = new QPushButton(this);
    m_playButton->setGeometry(110, 110, 30, 30);
    m_playButton->setCursor(Qt::PointingHandCursor);
    m_playButton->setStyleSheet(MusicUIObject::MQSSTinyBtnPlaylist);
    connect(m_playButton, SIGNAL(clicked()), SLOT(currentItemClicked()));

#ifdef Q_OS_UNIX
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

MusicWebDJRadioQueryItemWidget::~MusicWebDJRadioQueryItemWidget()
{
    delete m_playButton;
    delete m_iconLabel;
    delete m_nameLabel;
    delete m_creatorLabel;
}

void MusicWebDJRadioQueryItemWidget::setMusicResultsItem(const MusicResultsItem &item)
{
    m_itemData = item;
    m_nameLabel->setToolTip(item.m_name);
    m_nameLabel->setText(MusicUtils::Widget::elidedText(m_nameLabel->font(), m_nameLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));
    m_creatorLabel->setToolTip("by " + item.m_nickName);
    m_creatorLabel->setText(MusicUtils::Widget::elidedText(m_creatorLabel->font(), m_creatorLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));

    MusicDownloadSourceRequest *download = new MusicDownloadSourceRequest(this);
    connect(download, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != COVER_URL_NULL)
    {
        download->startToDownload(item.m_coverUrl);
    }
}

void MusicWebDJRadioQueryItemWidget::downLoadFinished(const QByteArray &data)
{
    MusicImageRenderer *render = new MusicImageRenderer(this);
    connect(render, SIGNAL(renderFinished(QImage)), SLOT(renderFinished(QImage)));
    render->setInputData(data, m_iconLabel->size());
    render->start();
}

void MusicWebDJRadioQueryItemWidget::renderFinished(const QImage &data)
{
    m_iconLabel->setPixmap(QPixmap::fromImage(data));
    m_playButton->raise();
}

void MusicWebDJRadioQueryItemWidget::currentItemClicked()
{
    Q_EMIT currentItemClicked(m_itemData);
}



MusicWebDJRadioQueryWidget::MusicWebDJRadioQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent)
{
    m_container->show();
    layout()->removeWidget(m_mainWindow);
    layout()->addWidget(m_container);
    m_container->addWidget(m_mainWindow);

    m_firstInit = false;
    m_infoWidget = nullptr;
    m_gridLayout = nullptr;
    m_pagingWidgetObject = nullptr;
    m_downloadRequest = new MusicDJRadioProgramCategoryRequest(this);
    connect(m_downloadRequest, SIGNAL(createProgramItem(MusicResultsItem)), SLOT(createProgramItem(MusicResultsItem)));
}

MusicWebDJRadioQueryWidget::~MusicWebDJRadioQueryWidget()
{
    delete m_infoWidget;
    delete m_gridLayout;
    delete m_downloadRequest;
    delete m_pagingWidgetObject;
}

void MusicWebDJRadioQueryWidget::setSongName(const QString &name)
{
    MusicAbstractItemQueryWidget::setSongName(name);
    m_downloadRequest->startToSearch(MusicAbstractQueryRequest::OtherQuery, name);
}

void MusicWebDJRadioQueryWidget::setSongNameById(const QString &id)
{
    MusicAbstractItemQueryWidget::setSongName(id);
    MusicResultsItem item;
    item.m_id = id;
    currentRadioClicked(item);
}

void MusicWebDJRadioQueryWidget::resizeWindow()
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

void MusicWebDJRadioQueryWidget::createProgramItem(const MusicResultsItem &item)
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
        containTopLayout->addStretch(1);

        QPushButton *backButton = new QPushButton(tr("Back"), containTopWidget);
        backButton->setFixedSize(90, 30);
        backButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
        backButton->setCursor(QCursor(Qt::PointingHandCursor));
        connect(backButton, SIGNAL(clicked()), this, SIGNAL(backToMainMenu()));
        containTopLayout->addWidget(backButton);

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

    if(m_pagingWidgetObject)
    {
        const int total = ceil(m_downloadRequest->getPageTotal() * 1.0 / m_downloadRequest->getPageSize());
        m_pagingWidgetObject->reset(total);
    }

    MusicWebDJRadioQueryItemWidget *label = new MusicWebDJRadioQueryItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(MusicResultsItem)), SLOT(currentRadioClicked(MusicResultsItem)));
    label->setMusicResultsItem(item);

    int lineNumber = width() / LINE_SPACING_SIZE;
    m_gridLayout->addWidget(label, m_resizeWidgets.count() / lineNumber, m_resizeWidgets.count() % lineNumber, Qt::AlignCenter);
    m_resizeWidgets << label;
}

void MusicWebDJRadioQueryWidget::currentRadioClicked(const MusicResultsItem &item)
{
    delete m_infoWidget;
    m_infoWidget = new MusicWebDJRadioInfoWidget(this);
    connect(m_infoWidget, SIGNAL(backToMainMenu()), SLOT(backToMainMenuClicked()));
    m_infoWidget->setSongName(item.m_id);

    m_container->addWidget(m_infoWidget);
    m_container->setCurrentIndex(PLAYLIST_WINDOW_INDEX_1);
}

void MusicWebDJRadioQueryWidget::backToMainMenuClicked()
{
    if(!m_firstInit)
    {
        Q_EMIT backToMainMenu();
        return;
    }
    m_container->setCurrentIndex(PLAYLIST_WINDOW_INDEX_0);
}

void MusicWebDJRadioQueryWidget::buttonClicked(int index)
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
