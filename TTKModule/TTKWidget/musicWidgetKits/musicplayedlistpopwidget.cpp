#include "musicplayedlistpopwidget.h"
#include "musicfunctionuiobject.h"
#include "musicsongslistplayedtablewidget.h"
#include "musictinyuiobject.h"
#include "musicapplication.h"
#include "musicplaylist.h"
#include "musicwidgetheaders.h"

#include <QTimer>

static constexpr int MAX_SIZE = 3;

MusicPlayedListTopContainerWidget::MusicPlayedListTopContainerWidget(QWidget *parent)
    : QFrame(parent)
{
    setFixedHeight(37);
}

void MusicPlayedListTopContainerWidget::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);
    QPainter painter(this);

    QLinearGradient gradient;
    gradient.setStart(0, 32);
    gradient.setFinalStop(0, height());
    gradient.setColorAt(0.1, QColor(150, 150, 150, 150));
    gradient.setColorAt(0.9, QColor(180, 180, 180, 50));
    painter.fillRect(0, 32, width(), height(), gradient);

    painter.fillRect(QRect(0, 0, width(), height() - 3), QColor(0xED, 0xF8, 0xFE));
}



MusicPlayedListPopWidget *MusicPlayedListPopWidget::m_instance = nullptr;

MusicPlayedListPopWidget::MusicPlayedListPopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent),
      m_playlist(nullptr)
{
    m_instance = this;

    setToolTip(tr("Played List"));
    setStyleSheet(TTK::UI::BtnPlayedList);

    initialize();
}

MusicPlayedListPopWidget::~MusicPlayedListPopWidget()
{
    delete m_tableWidget;
    qDeleteAll(m_labels);
}

MusicPlayedListPopWidget *MusicPlayedListPopWidget::instance()
{
    return m_instance;
}

void MusicPlayedListPopWidget::setPlaylist(MusicPlaylist *playlist)
{
    m_playlist = playlist;
}

void MusicPlayedListPopWidget::clear()
{
    m_songList.clear();
    m_tableWidget->removeItems();
    setPlaylistCount(0);
}

void MusicPlayedListPopWidget::clearQueueState()
{
    m_tableWidget->clearQueueState();
}

void MusicPlayedListPopWidget::updatePlayedList(const TTK::IndexPropertyList &indexs)
{
    m_playlist->update(indexs);
}

void MusicPlayedListPopWidget::remove(int index)
{
    if(index < 0 || index >= m_songList.count())
    {
        return;
    }

    m_tableWidget->adjustPlayWidgetRow();
    m_tableWidget->removeRow(index);
    m_tableWidget->setPlayRowIndex(TTK_NORMAL_LEVEL);

    m_playlist->remove(index);
    m_songList.removeAt(index);

    setPlaylistSongs();
}

void MusicPlayedListPopWidget::remove(int playlistRow, const QString &path)
{
    int index = -1;
    m_tableWidget->adjustPlayWidgetRow();

    do
    {
        index = m_playlist->remove(playlistRow, path);
        if(index != -1)
        {
            m_songList.removeAt(index);
            m_tableWidget->removeRow(index);
        }
    } while(index != -1);
    m_tableWidget->setPlayRowIndex(TTK_NORMAL_LEVEL);

    setPlaylistSongs();
}

void MusicPlayedListPopWidget::remove(int playlistRow, const MusicSong &song)
{
    remove(playlistRow, song.path());
}

void MusicPlayedListPopWidget::append(int playlistRow, const MusicSong &song)
{
    m_playlist->append(playlistRow, song.path());
    m_songList << song;
    setPlaylistSongs();
}

void MusicPlayedListPopWidget::append(const MusicSongList &song)
{
    clear();
    m_songList = song;
    setPlaylistSongs();
}

void MusicPlayedListPopWidget::insert(int playlistRow, const MusicSong &song)
{
    insert(playlistRow, m_tableWidget->playRowIndex() + 1, song);
}

void MusicPlayedListPopWidget::insert(int playlistRow, int index, const MusicSong &song)
{
    if(index < 0 || index > m_songList.count())
    {
        return;
    }

    (index != m_songList.count()) ? m_songList.insert(index, song) : m_songList.append(song);
    m_playlist->appendQueue(playlistRow, song.path());

    const int row = m_tableWidget->playRowIndex();
    m_tableWidget->removeItems();
    setPlaylistSongs();

    m_tableWidget->setPlayRowIndex(row);
    m_tableWidget->selectPlayedRow();

    for(const MusicPlayItem &item : qAsConst(m_playlist->queueList()))
    {
        m_tableWidget->setQueueState(item.m_playlistRow);
    }
}

void MusicPlayedListPopWidget::selectCurrentIndex()
{
    const int index = m_playlist->currentIndex();
    m_tableWidget->selectRow(index);
}

void MusicPlayedListPopWidget::selectCurrentIndex(int playlistRow, const MusicSong &song)
{
    m_playlist->setCurrentIndex(playlistRow, song.path());
    selectCurrentIndex();
}

void MusicPlayedListPopWidget::popupMenu()
{
    QPoint pos = mapToGlobal(QPoint(0, 0));
    pos.setY(pos.y() - m_containWidget->height() - 10);
    pos.setX(pos.x() - m_containWidget->width() + width() + 3);

    m_tableWidget->selectPlayedRow();
    m_menu->exec(pos);
}

void MusicPlayedListPopWidget::removeItemAt(const TTKIntList &index)
{
    if(index.isEmpty())
    {
        return;
    }

    clearQueueState();

    const int id = m_tableWidget->playRowIndex();
    bool contains = false;

    for(int i = index.count() - 1; i >= 0; --i)
    {
        const int row = index[i];
        if(id == row)
        {
            contains = true;
        }
        m_playlist->remove(row);
    }

    if(contains)
    {
        MusicApplication *w = MusicApplication::instance();
        if(w->isPlaying())
        {
            w->playNext();
        }
        else
        {
            m_playlist->setCurrentIndex(TTK_LOW_LEVEL);
        }

        if(m_playlist->isEmpty())
        {
            clearPlaylist();
        }
    }
    else
    {
        int offset = 0;
        for(int i = 0; i < index.count(); ++i)
        {
            if(index[i] >= id)
            {
                break;
            }
            ++offset;
        }
        m_tableWidget->selectRow(id - offset);
    }

    setPlaylistCount(m_songList.count());
}

void MusicPlayedListPopWidget::clearItems()
{
    if(m_songList.isEmpty())
    {
        return;
    }

    m_tableWidget->adjustPlayWidgetRow();
    const int count = m_tableWidget->rowCount();
    for(int i = 0; i < count; ++i)
    {
        m_tableWidget->removeRow(0);
    }

    clearPlaylist();
}

void MusicPlayedListPopWidget::itemDoubleClicked()
{
    const int row = m_tableWidget->currentRow();
    if(row < 0)
    {
        return;
    }

    clearQueueState();
    m_playlist->removeQueue();
    MusicApplication::instance()->playedIndexBy(row);
}

void MusicPlayedListPopWidget::itemDoubleClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    TTK_SIGNLE_SHOT(itemDoubleClicked, TTK_SLOT);
}

void MusicPlayedListPopWidget::initialize()
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    layout->addStretch(2);
    for(int i = 0; i < MAX_SIZE; ++i)
    {
        QLabel *label = new QLabel(this);
        label->setFixedWidth(9);
        label->setPixmap(QPixmap(":/tiny/lb_number0"));
        layout->addWidget(label);
        m_labels << label;
    }
    layout->addStretch(1);

    setLayout(layout);

    m_containWidget->setFixedSize(LEFT_SIDE_WIDTH_MIN, 400);
    QVBoxLayout *containLayout = new QVBoxLayout(m_containWidget);
    containLayout->setContentsMargins(0, 0, 0, 0);
    containLayout->setSpacing(0);
    containLayout->addWidget(createContainerWidget());

    m_scrollArea = new QScrollArea(this);

    const QString &background = TTK::UI::BackgroundStyle10;
    QWidget *view = m_scrollArea->viewport();
    view->setObjectName("Viewport");
    view->setStyleSheet(QString("#Viewport{%1}").arg(background));

    m_tableWidget = new MusicSongsListPlayedTableWidget(this);
    m_tableWidget->setSongsList(&m_songList);
    connect(m_tableWidget, SIGNAL(deleteItemAt(TTKIntList)), SLOT(removeItemAt(TTKIntList)));
    connect(m_tableWidget, SIGNAL(cellDoubleClicked(int,int)), SLOT(itemDoubleClicked(int,int)));

    QWidget *playedListContainer = new QWidget(m_scrollArea);
    QVBoxLayout *playedListLayout = new QVBoxLayout(playedListContainer);
    playedListLayout->setContentsMargins(0, 0, 0, 0);
    playedListLayout->setSpacing(0);
    playedListLayout->addWidget(m_tableWidget);
    playedListContainer->setLayout(playedListLayout);

    TTK::Widget::generateVScrollAreaFormat(m_scrollArea, playedListContainer);
    m_tableWidget->setMovedScrollBar(m_scrollArea->verticalScrollBar());

    containLayout->addWidget(m_scrollArea);
    m_containWidget->setLayout(containLayout);
}

QWidget *MusicPlayedListPopWidget::createContainerWidget()
{
    MusicPlayedListTopContainerWidget *topWidget = new MusicPlayedListTopContainerWidget(this);
#ifdef Q_OS_UNIX
    topWidget->setObjectName("TopWidget");
    topWidget->setStyleSheet(QString("#TopWidget{%1}").arg(TTK::UI::BackgroundStyle10));
#endif
    QHBoxLayout *topWidgetLayout = new QHBoxLayout(topWidget);
    topWidgetLayout->setSpacing(15);
    QLabel *label = new QLabel(tr("Played List"), topWidget);
    label->setStyleSheet(TTK::UI::ColorStyle08 + TTK::UI::FontStyle01 + TTK::UI::FontStyle03);

    QPushButton *shareButton = new QPushButton(this);
    shareButton->setFixedSize(16, 16);
    shareButton->setToolTip(tr("Share List"));
    shareButton->setCursor(QCursor(Qt::PointingHandCursor));
    shareButton->setStyleSheet(TTK::UI::TinyBtnShare);

    QPushButton *deleteButton = new QPushButton(this);
    deleteButton->setFixedSize(16, 16);
    deleteButton->setToolTip(tr("Clear List"));
    deleteButton->setCursor(QCursor(Qt::PointingHandCursor));
    deleteButton->setStyleSheet(TTK::UI::TinyBtnDelete);
    connect(deleteButton, SIGNAL(clicked()), SLOT(clearItems()));

#ifdef Q_OS_UNIX
    shareButton->setFocusPolicy(Qt::NoFocus);
    deleteButton->setFocusPolicy(Qt::NoFocus);
#endif

    QToolButton *closeButton = new QToolButton(this);
    closeButton->setFixedSize(16, 16);
    closeButton->setToolTip(tr("Close List"));
    closeButton->setCursor(QCursor(Qt::PointingHandCursor));
    closeButton->setStyleSheet(TTK::UI::TinyBtnClose);
    connect(closeButton, SIGNAL(clicked()), m_menu, SLOT(close()));

    topWidgetLayout->addWidget(label);
    topWidgetLayout->addStretch(1);
    topWidgetLayout->addWidget(shareButton);
    topWidgetLayout->addWidget(deleteButton);
    topWidgetLayout->addWidget(closeButton);
    topWidget->setLayout(topWidgetLayout);

    return topWidget;
}

void MusicPlayedListPopWidget::setPlaylistSongs()
{
    setPlaylistCount(m_songList.count());
    m_tableWidget->updateSongsList(m_songList);
}

void MusicPlayedListPopWidget::setPlaylistCount(int count)
{
    if(count >= 1000)
    {
        for(int i = MAX_SIZE - 1; i >= 0; --i)
        {
            m_labels[i]->setPixmap(QPixmap(QString(":/tiny/lb_number%1").arg(9)));
        }
    }
    else
    {
        for(int i = MAX_SIZE - 1; i >= 0; --i)
        {
            m_labels[i]->setPixmap(QPixmap(QString(":/tiny/lb_number%1").arg(count % 10)));
            count /= 10;
        }
    }
}

void MusicPlayedListPopWidget::clearPlaylist()
{
    m_tableWidget->setPlayRowIndex(TTK_NORMAL_LEVEL);
    m_songList.clear();
    setPlaylistCount(0);

    MusicApplication::instance()->playIndexBy(TTK_NORMAL_LEVEL);
}
