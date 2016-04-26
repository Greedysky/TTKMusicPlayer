#include "musicbackgroundremotewidget.h"
#include "musicbackgroundlistwidget.h"
#include "musicdownloadqueuecache.h"

#include <QPushButton>
#include <QButtonGroup>
#include <QBoxLayout>
#include <QDir>

MusicBackgroundRemoteWidget::MusicBackgroundRemoteWidget(QWidget *parent)
    : QWidget(parent), m_downloadQueue(nullptr)
{
    m_currentIndex = -1;
    m_group = new QButtonGroup(this);

    initWidget();
    createUrls();

    m_downloadQueue = new MusicDownloadQueueCache(QStringList(), QStringList(),
                            MusicDownLoadThreadAbstract::Download_BigBG, this);
    connect(m_downloadQueue, SIGNAL(musicDownLoadFinished(QString)), m_listWidget,
                             SLOT(reCreateItem(QString)));
}

MusicBackgroundRemoteWidget::~MusicBackgroundRemoteWidget()
{
    delete m_downloadQueue;
    delete m_listWidget;
    delete m_group;
}

void MusicBackgroundRemoteWidget::initWidget()
{
    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setContentsMargins(0, 0, 0, 0);

    createButton();
    for(int i=0; i<4; ++i)
    {
        hbox->addWidget( m_group->button(i) );
    }
    vbox->addLayout(hbox);
    m_listWidget = new MusicBackgroundListWidget(this);
    connect(m_listWidget, SIGNAL(itemClicked(QListWidgetItem*)),
                          SLOT(itemUserClicked(QListWidgetItem*)));
    vbox->addWidget(m_listWidget);
    setLayout(vbox);
}

void MusicBackgroundRemoteWidget::createButton()
{
    for(int i=0; i<4; ++i)
    {
        QPushButton *button = new QPushButton(this);
        button->setFixedHeight(25);
        button->setText(tr("NetButton%1").arg(i + 1));
        button->setStyleSheet(MusicUIObject::MPushButtonStyle08);
        button->setCursor(QCursor(Qt::PointingHandCursor));
        m_group->addButton(button, i);
    }
    connect(m_group, SIGNAL(buttonClicked(int)), SLOT(buttonClicked(int)));
}

void MusicBackgroundRemoteWidget::buttonClicked(int index)
{
    int count = 0;
    if(m_currentIndex != index)
    {
        m_downloadQueue->abort();
    }
    switch(m_currentIndex = index)
    {
        case 0: count = 13; break;
        case 1: count = 14; break;
        case 2: count = 27; break;
        case 3: count = 9; break;
    }

    QDir dir( "." );
    dir.mkpath( QString("%1%2").arg(DATA_CACHED_AL).arg(m_currentIndex) );

    m_listWidget->clearAllItems();
    for(int i=0; i<count; i++)
    {
        m_listWidget->createItem(QString(), ":/image/noneImage");
    }

    m_downloadQueue->addImageQueue(m_urls[m_currentIndex], createPaths());
    m_downloadQueue->startToDownload();
}

void MusicBackgroundRemoteWidget::itemUserClicked(QListWidgetItem *item)
{
    if(!item->data(MUSIC_BG_ROLE).toString().isEmpty())
    {
        emit showCustomSkin(QString("%1%2/%3%4").arg(DATA_CACHED_AL)
                           .arg(m_currentIndex).arg(m_listWidget->currentRow()).arg(JPG_FILE));
    }
}

void MusicBackgroundRemoteWidget::createUrls()
{
    QString prefix = "http://img1.kwcdn.kuwo.cn/star/KuwoPhotoArt/0/0/";
    ////////////////////1
    m_urls << (QStringList()
           << prefix + "1392706532786_0.jpg" << prefix + "1392706533257_0.jpg"
           << prefix + "1392706531729_0.jpg" << prefix + "1392706530884_0.jpg"
           << prefix + "1392706530322_0.jpg" << prefix + "1392706529683_0.jpg"
           << prefix + "1392706772382_0.jpg" << prefix + "1392706773800_0.jpg"
           << prefix + "1392706857712_0.jpg" << prefix + "1392706858550_0.jpg"
           << prefix + "1392707046854_0.jpg" << prefix + "1392707043813_0.jpg"
           << prefix + "1392707042612_0.jpg")
    ////////////////////2
           << (QStringList()
           << prefix + "1392710159240_0.jpg" << prefix + "1392710158654_0.jpg"
           << prefix + "1392716523048_0.jpg" << prefix + "1392716523976_0.jpg"
           << prefix + "1392716641276_0.jpg" << prefix + "1392716644700_0.jpg"
           << prefix + "1392716646260_0.jpg" << prefix + "1392716647355_0.jpg"
           << prefix + "1392716642799_0.jpg" << prefix + "1392716685115_0.jpg"
           << prefix + "1392716688192_0.jpg" << prefix + "1392716690865_0.jpg"
           << prefix + "1392716691391_0.jpg" << prefix + "1392716691992_0.jpg")
    ////////////////////3
           << (QStringList()
           << prefix + "1392630277898_0.jpg" << prefix + "1392630274891_0.jpg"
           << prefix + "1392630274257_0.jpg" << prefix + "1392630272320_0.jpg"
           << prefix + "1392630269669_0.jpg" << prefix + "1392633744378_0.jpg"
           << prefix + "1392633739646_0.jpg" << prefix + "1392633739218_0.jpg"
           << prefix + "1392633734129_0.jpg" << prefix + "1392633472586_0.jpg"
           << prefix + "1392633476617_0.jpg" << prefix + "1392633475965_0.jpg"
           << prefix + "1392633460419_0.jpg" << prefix + "1392633462485_0.jpg"
           << prefix + "1392633465121_0.jpg" << prefix + "1392633040098_0.jpg"
           << prefix + "1392633042823_0.jpg" << prefix + "1392633043634_0.jpg"
           << prefix + "1392633034581_0.jpg" << prefix + "1392633017171_0.jpg"
           << prefix + "1392632216085_0.jpg" << prefix + "1392632215251_0.jpg"
           << prefix + "1392632211699_0.jpg" << prefix + "1392632209023_0.jpg"
           << prefix + "1392632031561_0.jpg" << prefix + "1392632022038_0.jpg"
           << prefix + "1392632018561_0.jpg")
    ////////////////////4
           << (QStringList()
           << prefix + "1392707254886_0.jpg" << prefix + "1392707296538_0.jpg"
           << prefix + "1392707322411_0.jpg" << prefix + "1392707398965_0.jpg"
           << prefix + "1392707420038_0.jpg" << prefix + "1392707447194_0.jpg"
           << prefix + "1392707472146_0.jpg" << prefix + "1392707690215_0.jpg"
           << prefix + "1392707678198_0.jpg");
}

QStringList MusicBackgroundRemoteWidget::createPaths()
{
    QStringList paths;
    int count = 0;
    switch(m_currentIndex)
    {
        case 0: count = 13; break;
        case 1: count = 14; break;
        case 2: count = 27; break;
        case 3: count = 9; break;
    }
    for(int i=0; i<count; ++i)
    {
        paths << QString("%1%2/%3%4").arg(DATA_CACHED_AL)
                 .arg(m_currentIndex).arg(i).arg(JPG_FILE);
    }
    return paths;
}
