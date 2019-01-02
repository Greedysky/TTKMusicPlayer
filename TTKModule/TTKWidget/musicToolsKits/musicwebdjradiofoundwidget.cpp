#include "musicwebdjradiofoundwidget.h"
#include "musicdjradioprogramcategorythread.h"
#include "musictinyuiobject.h"
#include "musicpagingwidgetobject.h"
#include "musicsettingmanager.h"
#include "musicdownloadqueryfactory.h"
#include "musicdownloadsourcethread.h"
#include "musicwebdjradioinfowidget.h"
#include "musicotherdefine.h"

#include <qmath.h>

#define WIDTH_LABEL_SIZE   150
#define HEIGHT_LABEL_SIZE  200
#define LINE_SPACING_SIZE  200

MusicWebDJRadioFoundItemWidget::MusicWebDJRadioFoundItemWidget(QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(WIDTH_LABEL_SIZE, HEIGHT_LABEL_SIZE);

    m_playButton = new QPushButton(this);
    m_playButton->setGeometry(110, 110, 30, 30);
    m_playButton->setCursor(Qt::PointingHandCursor);
    m_playButton->setStyleSheet(MusicUIObject::MKGTinyBtnPlaylist);
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

MusicWebDJRadioFoundItemWidget::~MusicWebDJRadioFoundItemWidget()
{
    delete m_playButton;
    delete m_iconLabel;
    delete m_nameLabel;
    delete m_creatorLabel;
}

void MusicWebDJRadioFoundItemWidget::setMusicResultsItem(const MusicResultsItem &item)
{
    m_itemData = item;
    m_nameLabel->setToolTip(item.m_name);
    m_nameLabel->setText(MusicUtils::Widget::elidedText(m_nameLabel->font(), m_nameLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));
    m_creatorLabel->setToolTip("by " + item.m_nickName);
    m_creatorLabel->setText(MusicUtils::Widget::elidedText(m_creatorLabel->font(), m_creatorLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));

    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != COVER_URL_NULL)
    {
        download->startToDownload(item.m_coverUrl);
    }
}

void MusicWebDJRadioFoundItemWidget::downLoadFinished(const QByteArray &data)
{
    QPixmap pix;
    pix.loadFromData(data);
    if(!pix.isNull())
    {
        QPixmap cv(":/image/lb_album_cover");
        cv = cv.scaled(m_iconLabel->size());
        pix = pix.scaled(m_iconLabel->size());
        MusicUtils::Widget::fusionPixmap(pix, cv, QPoint(0, 0));
        m_iconLabel->setPixmap(pix);
    }
    m_playButton->raise();
}

void MusicWebDJRadioFoundItemWidget::currentItemClicked()
{
    emit currentItemClicked(m_itemData);
}



MusicWebDJRadioFoundWidget::MusicWebDJRadioFoundWidget(QWidget *parent)
    : MusicFoundAbstractWidget(parent)
{
    m_container->show();
    layout()->removeWidget(m_mainWindow);
    layout()->addWidget(m_container);
    m_container->addWidget(m_mainWindow);

    m_firstInit = false;
    m_infoWidget = nullptr;
    m_gridLayout = nullptr;
    m_pagingWidgetObject = nullptr;
    m_downloadThread = new MusicDJRadioProgramCategoryThread(this);
    connect(m_downloadThread, SIGNAL(createProgramItem(MusicResultsItem)), SLOT(createProgramItem(MusicResultsItem)));
}

MusicWebDJRadioFoundWidget::~MusicWebDJRadioFoundWidget()
{
    delete m_infoWidget;
    delete m_gridLayout;
    delete m_downloadThread;
    delete m_pagingWidgetObject;
}

void MusicWebDJRadioFoundWidget::setSongName(const QString &name)
{
    MusicFoundAbstractWidget::setSongName(name);
    m_downloadThread->startToSearch(MusicDownLoadQueryThreadAbstract::OtherQuery, name);
}

void MusicWebDJRadioFoundWidget::setSongNameById(const QString &id)
{
    MusicFoundAbstractWidget::setSongName(id);
    MusicResultsItem item;
    item.m_id = id;
    currentRadioClicked(item);
}

void MusicWebDJRadioFoundWidget::resizeWindow()
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

        const int lineNumber = width()/LINE_SPACING_SIZE;
        for(int i=0; i<m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->addWidget(m_resizeWidgets[i], i/lineNumber, i%lineNumber, Qt::AlignCenter);
        }
    }
}

void MusicWebDJRadioFoundWidget::createProgramItem(const MusicResultsItem &item)
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
        containTopLayout->addStretch(1);

        QPushButton *backButton = new QPushButton(tr("Back"), containTopWidget);
        backButton->setFixedSize(90, 30);
        backButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
        backButton->setCursor(QCursor(Qt::PointingHandCursor));
        connect(backButton, SIGNAL(clicked()), this, SIGNAL(backToMainMenu()));
        containTopLayout->addWidget(backButton);

        containTopWidget->setLayout(containTopLayout);

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

        m_pagingWidgetObject = new MusicPagingWidgetObject(m_mainWindow);
        connect(m_pagingWidgetObject, SIGNAL(mapped(int)), SLOT(buttonClicked(int)));

        const int total = ceil(m_downloadThread->getPageTotal()*1.0/m_downloadThread->getPageSize());
        mainlayout->addWidget(m_pagingWidgetObject->createPagingWidget(m_mainWindow, total));
        mainlayout->addStretch(1);
    }

    if(m_pagingWidgetObject)
    {
        const int total = ceil(m_downloadThread->getPageTotal()*1.0/m_downloadThread->getPageSize());
        m_pagingWidgetObject->reset(total);
    }

    MusicWebDJRadioFoundItemWidget *label = new MusicWebDJRadioFoundItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(MusicResultsItem)), SLOT(currentRadioClicked(MusicResultsItem)));
    label->setMusicResultsItem(item);

    int lineNumber = width()/LINE_SPACING_SIZE;
    m_gridLayout->addWidget(label, m_resizeWidgets.count()/lineNumber, m_resizeWidgets.count()%lineNumber, Qt::AlignCenter);
    m_resizeWidgets << label;
}

void MusicWebDJRadioFoundWidget::currentRadioClicked(const MusicResultsItem &item)
{
    delete m_infoWidget;
    m_infoWidget = new MusicWebDJRadioInfoWidget(this);
    connect(m_infoWidget, SIGNAL(backToMainMenu()), SLOT(backToMainMenuClicked()));
    m_infoWidget->setSongName(item.m_id);

    m_container->addWidget(m_infoWidget);
    m_container->setCurrentIndex(PLAYLIST_WINDOW_INDEX_1);
}

void MusicWebDJRadioFoundWidget::backToMainMenuClicked()
{
    if(!m_firstInit)
    {
        emit backToMainMenu();
        return;
    }
    m_container->setCurrentIndex(PLAYLIST_WINDOW_INDEX_0);
}

void MusicWebDJRadioFoundWidget::buttonClicked(int index)
{
    while(!m_resizeWidgets.isEmpty())
    {
        QWidget *w = m_resizeWidgets.takeLast();
        m_gridLayout->removeWidget(w);
        delete w;
    }

    const int total = ceil(m_downloadThread->getPageTotal()*1.0/m_downloadThread->getPageSize());
    m_pagingWidgetObject->paging(index, total);
    m_downloadThread->startToPage(m_pagingWidgetObject->currentIndex() - 1);
}
