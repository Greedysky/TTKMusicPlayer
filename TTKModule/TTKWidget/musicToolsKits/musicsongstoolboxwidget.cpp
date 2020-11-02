#include "musicsongstoolboxwidget.h"
#include "musicsongstoolitemrenamedwidget.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"
#include "musictinyuiobject.h"
#include "musicsong.h"
#include "musicimageutils.h"
#include "musicapplication.h"
#include "musictopareawidget.h"

#include <QPainter>

MusicSongsToolBoxTopWidget::MusicSongsToolBoxTopWidget(int index, const QString &text, QWidget *parent)
    : MusicFunctionToolBoxTopWidget(index, text, parent)
{
    m_musicSort = nullptr;
    m_renameLine = nullptr;

    QPushButton *enhanceButton = new QPushButton(this);
    enhanceButton->setToolTip(tr("enhanceLossless"));
    enhanceButton->setStyleSheet(MusicUIObject::MQSSTinyBtnEnhanceLossless);
    enhanceButton->setCursor(QCursor(Qt::PointingHandCursor));
    enhanceButton->setGeometry(240, 12, 16, 16);

    QPushButton *shareListButton = new QPushButton(this);
    shareListButton->setToolTip(tr("shareList"));
    shareListButton->setStyleSheet(MusicUIObject::MQSSTinyBtnShare);
    shareListButton->setCursor(QCursor(Qt::PointingHandCursor));
    shareListButton->setGeometry(265, 12, 16, 16);

    QPushButton *menuButton = new QPushButton(this);
    menuButton->setToolTip(tr("listMenu"));
    menuButton->setStyleSheet(MusicUIObject::MQSSTinyBtnListMenu);
    menuButton->setCursor(QCursor(Qt::PointingHandCursor));
    menuButton->setGeometry(290, 12, 16, 16);
    connect(menuButton, SIGNAL(clicked()), SLOT(showMenu()));

#ifdef Q_OS_UNIX
    enhanceButton->setFocusPolicy(Qt::NoFocus);
    shareListButton->setFocusPolicy(Qt::NoFocus);
    menuButton->setFocusPolicy(Qt::NoFocus);
#endif

}

MusicSongsToolBoxTopWidget::~MusicSongsToolBoxTopWidget()
{
    delete m_renameLine;
}

void MusicSongsToolBoxTopWidget::deleteRowItem()
{
    Q_EMIT deleteRowItem(m_index);
}

void MusicSongsToolBoxTopWidget::deleteRowItemAll()
{
    Q_EMIT deleteRowItemAll(m_index);
}

void MusicSongsToolBoxTopWidget::changRowItemName()
{
    if(!m_renameLine)
    {
        m_renameLine = new MusicSongsToolItemRenamedWidget(getTitle(), this);
        connect(m_renameLine, SIGNAL(renameFinished(QString)), SLOT(setChangItemName(QString)));
        m_renameLine->setGeometry(m_labelIcon->width(), 3, RENAME_WIDTH, height() - 6);
    }
    m_renameLine->show();
}

void MusicSongsToolBoxTopWidget::setChangItemName(const QString &name)
{
    setTitle(name + m_suffixString);
    Q_EMIT changRowItemName(m_index, name);

    m_renameLine->deleteLater();
    m_renameLine = nullptr;
}

void MusicSongsToolBoxTopWidget::addNewFiles()
{
    Q_EMIT addNewFiles(m_index);
}

void MusicSongsToolBoxTopWidget::addNewDir()
{
    Q_EMIT addNewDir(m_index);
}

void MusicSongsToolBoxTopWidget::exportSongsItemList()
{
   MusicApplication::instance()->musicExportSongsItemList(m_index);
}

void MusicSongsToolBoxTopWidget::showMenu()
{
    QMenu menu(this);
    menu.setStyleSheet(MusicUIObject::MQSSMenuStyle02);
    menu.addAction(tr("addNewItem"), this, SIGNAL(addNewRowItem()));
    menu.addSeparator();

    QMenu musicAddNewFiles(tr("addNewFiles"), &menu);
    bool disable = !(m_index == MUSIC_LOVEST_LIST || m_index == MUSIC_NETWORK_LIST || m_index == MUSIC_RECENT_LIST);
    menu.addMenu(&musicAddNewFiles)->setEnabled(disable);
    musicAddNewFiles.addAction(tr("openOnlyFiles"), this, SLOT(addNewFiles()));
    musicAddNewFiles.addAction(tr("openOnlyDir"), this, SLOT(addNewDir()));

    menu.addAction(tr("playLater"), this, SLOT(addToPlayLater()));
    menu.addAction(tr("addToPlaylist"), this, SLOT(addToPlayedList()));

    QMenu musicSortFiles(tr("sort"), &menu);
    musicSortFiles.addAction(tr("sortByFileName"))->setData(0);
    musicSortFiles.addAction(tr("sortBySinger"))->setData(1);
    musicSortFiles.addAction(tr("sortByFileSize"))->setData(2);
    musicSortFiles.addAction(tr("sortByAddTime"))->setData(3);
    musicSortFiles.addAction(tr("sortByPlayTime"))->setData(4);
    musicSortFiles.addAction(tr("sortByPlayCount"))->setData(5);
    connect(&musicSortFiles, SIGNAL(triggered(QAction*)), SLOT(musicListSongSortBy(QAction*)));
    if(m_musicSort)
    {
        const QList<QAction*> actions(musicSortFiles.actions());
        if(-1 < m_musicSort->m_index && m_musicSort->m_index < actions.count())
        {
            const bool asc = m_musicSort->m_sortType == Qt::AscendingOrder;
            actions[m_musicSort->m_index]->setIcon(QIcon(asc ? ":/tiny/lb_sort_asc" : ":/tiny/lb_sort_desc"));
        }
    }
    menu.addMenu(&musicSortFiles);

    menu.addAction(tr("collectAll"));
    menu.addAction(tr("exportList"), this, SLOT(exportSongsItemList()));
    menu.addSeparator();

    disable = isItemEnabled();
    menu.addAction(tr("deleteAll"), this, SLOT(deleteRowItemAll()));
    menu.addAction(QIcon(":/contextMenu/btn_delete"), tr("deleteItem"), this, SLOT(deleteRowItem()))->setEnabled(disable);
    menu.addAction(tr("changItemName"), this, SLOT(changRowItemName()))->setEnabled(disable);

    menu.exec(QCursor::pos());
}

void MusicSongsToolBoxTopWidget::musicListSongSortBy(QAction *action)
{
    if(m_musicSort)
    {
        const int bIndex = m_musicSort->m_index;
        const int newIndex = action->data().toInt();
        m_musicSort->m_index = newIndex;
        if(bIndex == newIndex)
        {
            const bool asc = m_musicSort->m_sortType == Qt::AscendingOrder;
            m_musicSort->m_sortType = asc ? Qt::DescendingOrder : Qt::AscendingOrder;
        }
        else
        {
            m_musicSort->m_sortType = Qt::AscendingOrder;
        }
        Q_EMIT musicListSongSortBy(m_index);
    }
}

void MusicSongsToolBoxTopWidget::addToPlayLater()
{
    Q_EMIT addToPlayLater(m_index);
}

void MusicSongsToolBoxTopWidget::addToPlayedList()
{
    Q_EMIT addToPlayedList(m_index);
}

bool MusicSongsToolBoxTopWidget::isItemEnabled() const
{
    return !(m_index == MUSIC_NORMAL_LIST || m_index == MUSIC_LOVEST_LIST || m_index == MUSIC_NETWORK_LIST || m_index == MUSIC_RECENT_LIST);
}

void MusicSongsToolBoxTopWidget::contextMenuEvent(QContextMenuEvent *event)
{
//    QWidget::contextMenuEvent(event);
    Q_UNUSED(event);
    showMenu();
}



MusicSongsToolBoxMaskWidget::MusicSongsToolBoxMaskWidget(QWidget *parent)
    : MusicSongsToolBoxTopWidget(-1, QString(), parent)
{
    move(0, 0);
    setFixedWidth(LEFT_SIDE_WIDTH_MIN);
    m_isBlockMoveExpand = true;

    hide();

    m_updateTimer = new QTimer(this);
    m_updateTimer->setInterval(MT_S*200);
    connect(m_updateTimer, SIGNAL(timeout()), SLOT(update()));
    m_updateTimer->start();
}

MusicSongsToolBoxMaskWidget::~MusicSongsToolBoxMaskWidget()
{
    m_updateTimer->stop();
    delete m_updateTimer;
}

void MusicSongsToolBoxMaskWidget::paintEvent(QPaintEvent *event)
{
    int alpha = M_SETTING_PTR->value(MusicSettingManager::BackgroundListTransparent).toInt();
        alpha = MusicUtils::Image::reRenderValue<int>(0xFF, 0x1F, 100 - alpha);
    QWidget::paintEvent(event);
    QPainter painter(this);

    QLinearGradient gradient;
    gradient.setStart(0, 32);
    gradient.setFinalStop(0, height());
    gradient.setColorAt(0.1, QColor(150, 150, 150, 150));
    gradient.setColorAt(0.9, QColor(180, 180, 180, 50));

    painter.setPen(Qt::transparent);
    painter.setBrush(gradient);
    painter.drawRect(0, 32, width(), height());

    QPixmap pix(MusicTopAreaWidget::instance()->getRendererPixmap());
    painter.drawPixmap(0, 0, width(), height() - 3, pix.copy(1, 91, width(), height() - 3));
    painter.fillRect(QRect(0, 0, width(), height() - 3), QColor(255, 255, 255, alpha));
}



MusicSongsToolBoxWidgetItem::MusicSongsToolBoxWidgetItem(int index, const QString &text, QWidget *parent)
    : MusicFunctionToolBoxWidgetItem(parent)
{
    m_topWidget = new MusicSongsToolBoxTopWidget(index, text, this);
    connect(m_topWidget, SIGNAL(addNewRowItem()), SIGNAL(addNewRowItem()));
    connect(m_topWidget, SIGNAL(deleteRowItem(int)), SIGNAL(deleteRowItem(int)));
    connect(m_topWidget, SIGNAL(deleteRowItemAll(int)), SIGNAL(deleteRowItemAll(int)));
    connect(m_topWidget, SIGNAL(changRowItemName(int,QString)), SIGNAL(changRowItemName(int,QString)));
    connect(m_topWidget, SIGNAL(addNewFiles(int)), SIGNAL(addNewFiles(int)));
    connect(m_topWidget, SIGNAL(addNewDir(int)), SIGNAL(addNewDir(int)));
    connect(m_topWidget, SIGNAL(musicListSongSortBy(int)), SIGNAL(musicListSongSortBy(int)));
    connect(m_topWidget, SIGNAL(addToPlayLater(int)), SIGNAL(addToPlayLater(int)));
    connect(m_topWidget, SIGNAL(addToPlayedList(int)), SIGNAL(addToPlayedList(int)));

    initialize();
}

void MusicSongsToolBoxWidgetItem::setMusicSongSort(MusicSongSort *sort)
{
    TTKStatic_cast(MusicSongsToolBoxTopWidget*, m_topWidget)->setMusicSongSort(sort);
}



MusicSongsToolBoxWidget::MusicSongsToolBoxWidget(QWidget *parent)
    : MusicFunctionToolBoxWidget(parent)
{

}

void MusicSongsToolBoxWidget::setMusicSongSort(QWidget *item, MusicSongSort *sort)
{
    for(int i=0; i<m_itemList.count(); ++i)
    {
        MusicFunctionToolBoxWidgetItem *it = m_itemList[i].m_widgetItem;
        for(int j=0; j<it->count(); ++j)
        {
            if(it->item(j) == item)
            {
                TTKStatic_cast(MusicSongsToolBoxWidgetItem*, it)->setMusicSongSort(sort);
                return;
            }
        }
    }
}

MusicFunctionToolBoxWidgetItem *MusicSongsToolBoxWidget::createItem(QWidget *item, const QString &text)
{
    MusicFunctionToolBoxWidgetItem *it = new MusicSongsToolBoxWidgetItem(m_itemIndexRaise, text, this);
    it->addItem(item);
    it->setItemExpand(true);
    return it;
}
