#include "musicwebentainradiolistview.h"
#include "musicwebentainradiowidget.h"
#include "musicutils.h"

#include <QTimer>

MusicWebEntainRadioListView::MusicWebEntainRadioListView(QWidget *parent)
    : QListWidget(parent), m_radioDialog(nullptr)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFrameShape(QFrame::NoFrame);//Set No Border
    setStyleSheet(MusicUIObject::MScrollBarStyle01);
    setIconSize(QSize(60, 60));
    setViewMode(QListView::IconMode);
    setMovement(QListView::Static);
    MusicUtils::UWidget::setTransparent(this, 50);

#ifdef Q_OS_WIN
    setSpacing(16);
    addListWidgetItem();
#else
    setSpacing(16);
    QTimer::singleShot(MT_MS, this, SLOT(addListWidgetItem()));
#endif
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(itemHasClicked(QListWidgetItem*)));
}

MusicWebEntainRadioListView::~MusicWebEntainRadioListView()
{
    delete m_radioDialog;
    clearAllItems();
}

QString MusicWebEntainRadioListView::getClassName()
{
    return staticMetaObject.className();
}

void MusicWebEntainRadioListView::clearAllItems()
{
    clear();
}

void MusicWebEntainRadioListView::addListWidgetItem()
{
    QListWidgetItem *item = new QListWidgetItem(QIcon(":/radio/lb_guowai")
                                                ,tr("guowai"), this);
    item->setSizeHint(QSize(80, 90));
    addItem(item);
    QListWidgetItem *item1 = new QListWidgetItem(QIcon(":/radio/lb_jiaotong")
                                                ,tr("jiaotong"), this);
    item1->setSizeHint(QSize(80, 90));
    addItem(item1);
    QListWidgetItem *item2 = new QListWidgetItem(QIcon(":/radio/lb_jingji")
                                                ,tr("jingji"), this);
    item2->setSizeHint(QSize(80, 90));
    addItem(item2);
    QListWidgetItem *item3 = new QListWidgetItem(QIcon(":/radio/lb_music")
                                                ,tr("music"), this);
    item3->setSizeHint(QSize(80, 90));
    addItem(item3);
    QListWidgetItem *item4 = new QListWidgetItem(QIcon(":/radio/lb_news")
                                                ,tr("news"), this);
    item4->setSizeHint(QSize(80, 90));
    addItem(item4);
    QListWidgetItem *item5 = new QListWidgetItem(QIcon(":/radio/lb_quyi")
                                                ,tr("quyi"), this);
    item5->setSizeHint(QSize(80, 90));
    addItem(item5);
    QListWidgetItem *item6 = new QListWidgetItem(QIcon(":/radio/lb_shenghuo")
                                                ,tr("shenghuo"), this);
    item6->setSizeHint(QSize(80, 90));
    addItem(item6);
    QListWidgetItem *item7 = new QListWidgetItem(QIcon(":/radio/lb_sports")
                                                ,tr("sports"), this);
    item7->setSizeHint(QSize(80, 90));
    addItem(item7);
    QListWidgetItem *item8 = new QListWidgetItem(QIcon(":/radio/lb_zhongyang")
                                                ,tr("zhongyang"), this);
    item8->setSizeHint(QSize(80, 90));
    addItem(item8);
    QListWidgetItem *item9 = new QListWidgetItem(QIcon(":/radio/lb_favorite")
                                                ,tr("favorite"), this);
    item9->setSizeHint(QSize(80, 90));
    addItem(item9);
    QListWidgetItem *item10 = new QListWidgetItem(QIcon(":/radio/lb_latest")
                                                ,tr("latest"), this);
    item10->setSizeHint(QSize(80, 90));
    addItem(item10);
}

void MusicWebEntainRadioListView::itemHasClicked(QListWidgetItem *item)
{
    if(m_radioDialog == nullptr)
    {
        m_radioDialog = new MusicWebEntainRadioWidget(this);
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
