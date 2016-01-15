#include "musicbackgroundlistwidget.h"
#include "musicuiobject.h"

#include <QCryptographicHash>
#include <QFile>

MusicBackgroundListWidget::MusicBackgroundListWidget(QWidget *parent)
    : QListWidget(parent)
{
    setFrameShape(QFrame::NoFrame); //Set No Border
    setStyleSheet(MusicUIObject::MScrollBarStyle01);
    setIconSize(QSize(100, 80));
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
    it->setIcon( icon );
    it->setData(MUSIC_BG_ROLE, name);
    it->setData(MUSIC_FILEHASH_ROLE, fileHash(name));
    it->setToolTip(name);
    addItem(it);
}

void MusicBackgroundListWidget::createItem(const QString &name, const QString &path)
{
    QListWidgetItem *it = new QListWidgetItem(this);
    it->setIcon( QIcon(QPixmap(path).scaled(90, 70)) );
    it->setData(MUSIC_BG_ROLE, name);
    it->setData(MUSIC_FILEHASH_ROLE, fileHash(path));
    it->setToolTip(name);
    addItem(it);
}

bool MusicBackgroundListWidget::contains(const QString &name)
{
    QListWidgetItem *it = nullptr;
    for(int i=0; i<count(); ++i)
    {
        if((it = item(i)) &&
            it->data(MUSIC_FILEHASH_ROLE).toString() == fileHash(name))
        {
            return true;
        }
    }
    return false;
}

bool MusicBackgroundListWidget::contains(QListWidgetItem *item)
{
    QListWidgetItem *it = nullptr;
    for(int i=0; i<count(); ++i)
    {
        if((it = this->item(i)) && it == item)
        {
            return true;
        }
    }
    return false;
}

int MusicBackgroundListWidget::indexOf(const QString &name)
{
    QListWidgetItem *it = nullptr;
    for(int i=0; i<count(); ++i)
    {
        if((it = item(i)) &&
            it->data(MUSIC_FILEHASH_ROLE).toString() == fileHash(name))
        {
            return i;
        }
    }
    return -1;
}

int MusicBackgroundListWidget::indexOf(QListWidgetItem *item)
{
    QListWidgetItem *it = nullptr;
    for(int i=0; i<count(); ++i)
    {
        if((it = this->item(i)) && it == item)
        {
            return i;
        }
    }
    return -1;
}

QString MusicBackgroundListWidget::fileHash(const QString &name)
{
    QFile file(name);
    if( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        return QString();
    }
    QByteArray data = file.readAll();
    file.close();
    return QCryptographicHash::hash(data, QCryptographicHash::Md5);
}

void MusicBackgroundListWidget::recreateItem(const QString &name)
{
    QListWidgetItem *it = nullptr;
    for(int i=0; i<count(); ++i)
    {
        if((it = item(i))->data(MUSIC_BG_ROLE).toString().isEmpty())
        {
            break;
        }
    }
    it->setIcon(QIcon(QPixmap(name).scaled(90, 70)));
    it->setData(MUSIC_BG_ROLE, name);
    it->setToolTip(name);
}
