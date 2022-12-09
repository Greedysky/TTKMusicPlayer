#include "musicwebdjradioquerywidget.h"
#include "musicdjradioprogramcategoryrequest.h"
#include "musictinyuiobject.h"
#include "musicpagequerywidget.h"
#include "musicsettingmanager.h"
#include "musicdownloadqueryfactory.h"
#include "musicdownloadcoverrequest.h"
#include "musicwebdjradioinfowidget.h"

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

void MusicWebDJRadioQueryItemWidget::setResultDataItem(const MusicResultDataItem &item)
{
    m_itemData = item;
    m_nameLabel->setToolTip(item.m_name);
    m_nameLabel->setText(MusicUtils::Widget::elidedText(m_nameLabel->font(), m_nameLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));
    m_creatorLabel->setToolTip("by " + item.m_nickName);
    m_creatorLabel->setText(MusicUtils::Widget::elidedText(m_creatorLabel->font(), m_creatorLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));

    if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != TTK_NULL_STR)
    {
        MusicDownloadCoverRequest *d = new MusicDownloadCoverRequest(this);
        connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
        d->startRequest(item.m_coverUrl);
    }
}

void MusicWebDJRadioQueryItemWidget::downLoadFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Input byte data is empty");
        return;
    }

    MusicImageRenderer *render = new MusicImageRenderer(sender());
    connect(render, SIGNAL(renderFinished(QPixmap)), SLOT(renderFinished(QPixmap)));
    render->setInputData(bytes, m_iconLabel->size());
    render->start();
}

void MusicWebDJRadioQueryItemWidget::renderFinished(const QPixmap &data)
{
    m_iconLabel->setPixmap(data);
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

    m_initialized = false;
    m_infoWidget = nullptr;
    m_gridLayout = nullptr;
    m_pageQueryWidget = nullptr;

    m_networkRequest = new MusicDJRadioProgramCategoryRequest(this);
    connect(m_networkRequest, SIGNAL(createProgramItem(MusicResultDataItem)), SLOT(createProgramItem(MusicResultDataItem)));
}

MusicWebDJRadioQueryWidget::~MusicWebDJRadioQueryWidget()
{
    delete m_infoWidget;
    delete m_gridLayout;
    delete m_pageQueryWidget;
}

void MusicWebDJRadioQueryWidget::setSongName(const QString &name)
{
    MusicAbstractItemQueryWidget::setSongName(name);
    m_networkRequest->startToSearch(MusicAbstractQueryRequest::QueryType::Other, name);
}

void MusicWebDJRadioQueryWidget::setSongNameById(const QString &id)
{
    MusicAbstractItemQueryWidget::setSongName(id);
    MusicResultDataItem result;
    result.m_id = id;
    currentRadioClicked(result);
}

void MusicWebDJRadioQueryWidget::resizeWidget()
{
    if(m_infoWidget)
    {
        m_infoWidget->resizeWidget();
    }

    if(!m_resizeWidgets.isEmpty() && m_gridLayout)
    {
        for(int i = 0; i < m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->removeWidget(m_resizeWidgets[i].m_label);
        }

        const int lineNumber = width() / LINE_SPACING_SIZE;
        for(int i = 0; i < m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->addWidget(m_resizeWidgets[i].m_label, i / lineNumber, i % lineNumber, Qt::AlignCenter);
        }
    }
}

void MusicWebDJRadioQueryWidget::createProgramItem(const MusicResultDataItem &item)
{
    if(!m_initialized)
    {
        delete m_statusLabel;
        m_statusLabel = nullptr;

        m_container->removeWidget(m_mainWindow);
        QScrollArea *scrollArea = new QScrollArea(this);
        MusicUtils::Widget::generateVScrollAreaFormat(scrollArea, m_mainWindow);
        m_container->addWidget(scrollArea);

        m_initialized = true;
        QVBoxLayout *mainlayout = TTKObject_cast(QVBoxLayout*, m_mainWindow->layout());
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
#ifdef Q_OS_UNIX
        backButton->setFocusPolicy(Qt::NoFocus);
#endif
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

        m_pageQueryWidget = new MusicPageQueryWidget(m_mainWindow);
        connect(m_pageQueryWidget, SIGNAL(clicked(int)), SLOT(buttonClicked(int)));

        const int pageTotal = ceil(m_networkRequest->totalSize() * 1.0 / m_networkRequest->pageSize());
        mainlayout->addWidget(m_pageQueryWidget->createPageWidget(m_mainWindow, pageTotal));
        mainlayout->addStretch(1);
    }

    if(m_pageQueryWidget)
    {
        const int pageTotal = ceil(m_networkRequest->totalSize() * 1.0 / m_networkRequest->pageSize());
        m_pageQueryWidget->reset(pageTotal);
    }

    MusicWebDJRadioQueryItemWidget *label = new MusicWebDJRadioQueryItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(MusicResultDataItem)), SLOT(currentRadioClicked(MusicResultDataItem)));
    label->setResultDataItem(item);

    int lineNumber = width() / LINE_SPACING_SIZE;
    m_gridLayout->addWidget(label, m_resizeWidgets.count() / lineNumber, m_resizeWidgets.count() % lineNumber, Qt::AlignCenter);

    m_resizeWidgets.push_back({label, label->font()});
}

void MusicWebDJRadioQueryWidget::currentRadioClicked(const MusicResultDataItem &item)
{
    delete m_infoWidget;
    m_infoWidget = new MusicWebDJRadioInfoWidget(this);
    connect(m_infoWidget, SIGNAL(backToMainMenu()), SLOT(backToMainMenuClicked()));
    m_infoWidget->setSongName(item.m_id);

    m_container->addWidget(m_infoWidget);
    m_container->setCurrentIndex(1);
}

void MusicWebDJRadioQueryWidget::backToMainMenuClicked()
{
    if(!m_initialized)
    {
        Q_EMIT backToMainMenu();
        return;
    }
    m_container->setCurrentIndex(0);
}

void MusicWebDJRadioQueryWidget::buttonClicked(int index)
{
    while(!m_resizeWidgets.isEmpty())
    {
        QWidget *w = m_resizeWidgets.takeLast().m_label;
        m_gridLayout->removeWidget(w);
        delete w;
    }

    const int pageTotal = ceil(m_networkRequest->totalSize() * 1.0 / m_networkRequest->pageSize());
    m_pageQueryWidget->page(index, pageTotal);
    m_networkRequest->startToPage(m_pageQueryWidget->currentIndex() - 1);
}
