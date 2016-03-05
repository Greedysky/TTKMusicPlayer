#include "musicwebmusicradiolistview.h"
#include "musicutils.h"
#include "musicuiobject.h"

MusicWebMusicRadioListView::MusicWebMusicRadioListView(QWidget *parent)
    : QListWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFrameShape(QFrame::NoFrame);//Set No Border
    setStyleSheet(MusicUIObject::MScrollBarStyle01);
    setViewMode(QListView::IconMode);
    setMovement(QListView::Static);
    MusicUtils::setTransparent(this, 50);

#ifdef Q_OS_WIN
    setSpacing(20);
    addListWidgetItem();
#else
    setSpacing(19);
    QTimer::singleShot(1, this, SLOT(addListWidgetItem()));
#endif
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(itemHasClicked(QListWidgetItem*)));
}

void MusicWebMusicRadioListView::addListWidgetItem()
{
    QListWidgetItem *item = new QListWidgetItem("guowai", this);
    item->setSizeHint(QSize(80, 30));
    addItem(item);
    item = new QListWidgetItem("guowai", this);
    item->setSizeHint(QSize(80, 30));
    addItem(item);
    item = new QListWidgetItem("guowai", this);
    item->setSizeHint(QSize(80, 30));
    addItem(item);
    item = new QListWidgetItem("guowai", this);
    item->setSizeHint(QSize(80, 30));
    addItem(item);
    item = new QListWidgetItem("guowai", this);
    item->setSizeHint(QSize(80, 30));
    addItem(item);
}

void MusicWebMusicRadioListView::itemHasClicked(QListWidgetItem *item)
{

}
