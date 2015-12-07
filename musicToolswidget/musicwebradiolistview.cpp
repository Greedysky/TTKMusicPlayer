#include "musicwebradiolistview.h"
#include "musicwebradiowidget.h"

MusicWebRadioListView::MusicWebRadioListView(QWidget *parent)
    : QListWidget(parent), m_radioDialog(nullptr)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFrameShape(QFrame::NoFrame);//Set No Border
    setStyleSheet(MusicUIObject::MScrollBarStyle01);
    setIconSize(QSize(60, 60));
    setViewMode(QListView::IconMode);
    setMovement(QListView::Static);
    setSpacing(20);
    setTransparent(50);
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)),
                  SLOT(itemHasClicked(QListWidgetItem*)));

    addListWidgetItem();
}

MusicWebRadioListView::~MusicWebRadioListView()
{
    delete m_radioDialog;
    clearAllItems();
}

void MusicWebRadioListView::setTransparent(int angle)
{
    QPalette pal = palette();
    pal.setBrush(QPalette::Base,QBrush(QColor(255, 255, 255, angle)));
    setPalette(pal);
    repaint();
}

void MusicWebRadioListView::clearAllItems()
{
    clear();
}

void MusicWebRadioListView::addListWidgetItem()
{
    QListWidgetItem *item = new QListWidgetItem(QIcon(":/radio/guowai")
                                                ,tr("guowai"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
    QListWidgetItem *item1 = new QListWidgetItem(QIcon(":/radio/jiaotong")
                                                ,tr("jiaotong"), this);
    item1->setSizeHint(QSize(80, 90));
    addItem(item1);
    QListWidgetItem *item2 = new QListWidgetItem(QIcon(":/radio/jingji")
                                                ,tr("jingji"), this);
    item2->setSizeHint(QSize(80, 90));
    addItem(item2);
    QListWidgetItem *item3 = new QListWidgetItem(QIcon(":/radio/music")
                                                ,tr("music"), this);
    item3->setSizeHint(QSize(80, 90));
    addItem(item3);
    QListWidgetItem *item4 = new QListWidgetItem(QIcon(":/radio/news")
                                                ,tr("news"), this);
    item4->setSizeHint(QSize(80, 90));
    addItem(item4);
    QListWidgetItem *item5 = new QListWidgetItem(QIcon(":/radio/quyi")
                                                ,tr("quyi"), this);
    item5->setSizeHint(QSize(80, 90));
    addItem(item5);
    QListWidgetItem *item6 = new QListWidgetItem(QIcon(":/radio/shenghuo")
                                                ,tr("shenghuo"), this);
    item6->setSizeHint(QSize(80, 90));
    addItem(item6);
    QListWidgetItem *item7 = new QListWidgetItem(QIcon(":/radio/sports")
                                                ,tr("sports"), this);
    item7->setSizeHint(QSize(80, 90));
    addItem(item7);
    QListWidgetItem *item8 = new QListWidgetItem(QIcon(":/radio/zhongyang")
                                                ,tr("zhongyang"), this);
    item8->setSizeHint(QSize(80, 90));
    addItem(item8);
    QListWidgetItem *item9 = new QListWidgetItem(QIcon(":/radio/favorite")
                                                ,tr("favorite"), this);
    item9->setSizeHint(QSize(80, 90));
    addItem(item9);
    QListWidgetItem *item10 = new QListWidgetItem(QIcon(":/radio/latest")
                                                ,tr("latest"), this);
    item10->setSizeHint(QSize(80, 90));
    addItem(item10);
}

void MusicWebRadioListView::itemHasClicked(QListWidgetItem *item)
{
    if(m_radioDialog == nullptr)
    {
        m_radioDialog = new MusicWebRadioWidget(this);
    }
    switch(row(item))
    {
        case 0:m_radioDialog->updateRadioList("international"); break;
        case 1:m_radioDialog->updateRadioList("traffic"); break;
        case 2:m_radioDialog->updateRadioList("economic"); break;
        case 3:m_radioDialog->updateRadioList("music"); break;
        case 4:m_radioDialog->updateRadioList("news"); break;
        case 5:m_radioDialog->updateRadioList("opera"); break;
        case 6:m_radioDialog->updateRadioList("life"); break;
        case 7:m_radioDialog->updateRadioList("sports"); break;
        case 8:m_radioDialog->updateRadioList("cctv"); break;
        case 9:m_radioDialog->updateFavouriteList(); break;
        case 10:m_radioDialog->updateRecentList(); break;
    }
    m_radioDialog->show();
}
