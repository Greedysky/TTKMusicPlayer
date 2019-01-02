#include "musicwebdjradiocategorywidget.h"
#include "musicdjradiocategorythread.h"
#include "musicdownloadsourcethread.h"
#include "musicwidgetheaders.h"

#define WIDTH_LABEL_SIZE   60
#define HEIGHT_LABEL_SIZE  105
#define LINE_SPACING_SIZE  105

MusicWebDJRadioCategoryItemWidget::MusicWebDJRadioCategoryItemWidget(QWidget *parent)
    : MusicClickedLabel(parent)
{
    setFixedSize(WIDTH_LABEL_SIZE, HEIGHT_LABEL_SIZE);

    m_iconLabel = new QLabel(this);
    m_iconLabel->setGeometry(0, 0, WIDTH_LABEL_SIZE, WIDTH_LABEL_SIZE);

    m_nameLabel = new QLabel(this);
    m_nameLabel->setGeometry(0, 65, WIDTH_LABEL_SIZE, 25);
    m_nameLabel->setAlignment(Qt::AlignCenter);
    m_nameLabel->setText(" - ");

    connect(this, SIGNAL(clicked()), SLOT(currentItemClicked()));
}

MusicWebDJRadioCategoryItemWidget::~MusicWebDJRadioCategoryItemWidget()
{
    delete m_iconLabel;
    delete m_nameLabel;
}

void MusicWebDJRadioCategoryItemWidget::setMusicResultsItem(const MusicResultsItem &item)
{
    m_itemData = item;
    m_nameLabel->setToolTip(item.m_name);
    m_nameLabel->setText(MusicUtils::Widget::elidedText(m_nameLabel->font(), m_nameLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));

    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != COVER_URL_NULL)
    {
        download->startToDownload(item.m_coverUrl);
    }
}

void MusicWebDJRadioCategoryItemWidget::downLoadFinished(const QByteArray &data)
{
    QPixmap pix;
    pix.loadFromData(data);
    if(!pix.isNull())
    {
        m_iconLabel->setPixmap(pix.copy(0, 0, WIDTH_LABEL_SIZE, WIDTH_LABEL_SIZE));
    }
}

void MusicWebDJRadioCategoryItemWidget::currentItemClicked()
{
    emit currentItemClicked(m_itemData);
}



MusicWebDJRadioCategoryWidget::MusicWebDJRadioCategoryWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    QWidget *mainWindow = new QWidget(this);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setAlignment(Qt::AlignLeft);
    scrollArea->setWidget(mainWindow);
    layout->addWidget(scrollArea);

    m_gridLayout = new QGridLayout(mainWindow);
    m_gridLayout->setVerticalSpacing(35);
    mainWindow->setLayout(m_gridLayout);

    m_categoryThread = new MusicDJRadioCategoryThread(this);
    connect(m_categoryThread, SIGNAL(downLoadDataChanged(QString)), SLOT(createCategoryItems()));
}

MusicWebDJRadioCategoryWidget::~MusicWebDJRadioCategoryWidget()
{
    delete m_gridLayout;
    delete m_categoryThread;
}

void MusicWebDJRadioCategoryWidget::init()
{
    m_categoryThread->startToDownload();
}

void MusicWebDJRadioCategoryWidget::resizeWindow()
{
    if(!m_resizeWidgets.isEmpty())
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

void MusicWebDJRadioCategoryWidget::createCategoryItems()
{
    foreach(const MusicResultsItem &item, m_categoryThread->getSearchedItems())
    {
        MusicWebDJRadioCategoryItemWidget *label = new MusicWebDJRadioCategoryItemWidget(this);
        connect(label, SIGNAL(currentItemClicked(MusicResultsItem)), SIGNAL(currentCategoryClicked(MusicResultsItem)));
        label->setMusicResultsItem(item);

        const int lineNumber = width()/LINE_SPACING_SIZE;
        m_gridLayout->addWidget(label, m_resizeWidgets.count()/lineNumber, m_resizeWidgets.count()%lineNumber, Qt::AlignCenter);
        m_resizeWidgets << label;
    }
}
