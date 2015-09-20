#include "musicbackgroundlistwidget.h"
#include "musicuiobject.h"

MusicBackgroundListWidget::MusicBackgroundListWidget(QWidget *parent)
    : QListWidget(parent)
{
    setFrameShape(QFrame::NoFrame); //Set No Border
    setStyleSheet(MusicUIObject::MScrollBarStyle01);
    setIconSize(QSize(100,80));
    setViewMode(QListView::IconMode);
    setMovement(QListView::Static);
}

MusicBackgroundListWidget::~MusicBackgroundListWidget()
{
    clearAllItems();
}

void MusicBackgroundListWidget::setCurrentItemName(const QString &name)
{
    //Set the current theme index
    for(int i=0; i<count(); ++i)
    {
        QListWidgetItem *it = item(i);
        if(it && it->data(MUSIC_BG_ROLE) == name )
        {
            setCurrentItem(it);
            break;
        }
    }
}

void MusicBackgroundListWidget::clearAllItems()
{
    clear();
}

void MusicBackgroundListWidget::createItem(const QString &name, const QIcon &icon)
{
    QListWidgetItem *it = new QListWidgetItem(this);
    it->setIcon(icon);
    it->setData(MUSIC_BG_ROLE, name);
    it->setToolTip(name);
    addItem(it);
}
