#include "musicsoundkmicrosearchwidget.h"
#include "musicdownloadquerykwthread.h"
#include "musicdownloadquerybdlearnthread.h"
#include "musiclocalsongsearchedit.h"
#include "musicgiflabelwidget.h"
#include "musicmessagebox.h"
#include "musicuiobject.h"
#include "musicitemdelegate.h"

#include <QLabel>
#include <QBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>

MusicSoundKMicroSearchTableWidget::MusicSoundKMicroSearchTableWidget(QWidget *parent)
    : MusicQueryItemTableWidget(parent)
{
    setColumnCount(5);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 290);
    headerview->resizeSection(2, 24);
    headerview->resizeSection(3, 24);
    headerview->resizeSection(4, 24);

    m_queryMv = true;
}

MusicSoundKMicroSearchTableWidget::~MusicSoundKMicroSearchTableWidget()
{
    clearAllItems();
}

QString MusicSoundKMicroSearchTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSoundKMicroSearchTableWidget::startSearchQuery(const QString &text)
{
    if(!M_NETWORK_PTR->isOnline())
    {   //no network connection
        clearAllItems();
        emit showDownLoadInfoFor(MusicObject::DW_DisConnection);
        return;
    }

    m_loadingLabel->show();
    m_loadingLabel->start();
    if(m_queryMv)
    {
        MusicDownLoadQueryKWThread *d = new MusicDownLoadQueryKWThread(this);
        d->setQueryExtraMovie(false);
        connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
        setQueryInput( d );
        m_downLoadManager->startToSearch(MusicDownLoadQueryThreadAbstract::MovieQuery, text);
    }
    else
    {
        MusicDownLoadQueryBDLearnThread *d = new MusicDownLoadQueryBDLearnThread(this);
        connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
        setQueryInput( d );
        m_downLoadManager->startToSearch(MusicDownLoadQueryThreadAbstract::MusicQuery, text);
    }
}

void MusicSoundKMicroSearchTableWidget::musicDownloadLocal(int row)
{
    if( row < 0 || (row >= rowCount() - 1))
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }

    MusicObject::MusicSongInfomations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    foreach(const MusicObject::MusicSongAttribute &attr, musicSongInfos[row].m_songAttrs)
    {
        if(m_queryMv)
        {
            if(attr.m_format == "mkv")
            {
                emit mvURLChanged(m_queryMv, attr.m_url, QString());
            }
        }
        else
        {
            emit mvURLChanged(m_queryMv, attr.m_url, musicSongInfos[row].m_lrcUrl);
        }
    }
}

void MusicSoundKMicroSearchTableWidget::setQueryMVFlag(bool flag)
{
    m_queryMv = flag;
}

void MusicSoundKMicroSearchTableWidget::clearAllItems()
{
    MusicQueryItemTableWidget::clearAllItems();
    setColumnCount(5);
}

void MusicSoundKMicroSearchTableWidget::createSearchedItems(const MusicSearchedItem &songItem)
{
    int count = rowCount();
    setRowCount(count + 1);

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, false);
    setItem(count, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip(songItem.m_artistname + " - " + songItem.m_songname);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, 275));
    item->setTextColor(QColor(100, 100, 100));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count, 1, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(randToGetStrength()));
    setItem(count, 2, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/tiny/lb_server_type")));
    item->setToolTip(songItem.m_type);
    setItem(count, 3, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(QString::fromUtf8(":/contextMenu/btn_audition")));
    setItem(count, 4, item);
}

void MusicSoundKMicroSearchTableWidget::itemDoubleClicked(int row, int column)
{
    if(column <= 0)
    {
        return;
    }
    musicDownloadLocal(row);
}

void MusicSoundKMicroSearchTableWidget::listCellClicked(int row, int column)
{
    MusicQueryItemTableWidget::listCellClicked(row, column);
    switch(column)
    {
        case 4:
            musicDownloadLocal(row);
            break;
        default:
            break;
    }
}


MusicSoundKMicroSearchWidget::MusicSoundKMicroSearchWidget(QWidget *parent)
    : MusicAbstractMoveSingleWidget(parent)
{
    ///Remove the title bar
    setWindowFlags( windowFlags() | Qt::WindowStaysOnTopHint | Qt::Tool);
    blockMoveOption(true);

    raise();
    resize(408, 498);

    QVBoxLayout *layout = new QVBoxLayout(m_container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QLabel *topWidget = new QLabel(tr(" Search"), this);
    topWidget->setStyleSheet(MusicUIObject::MBackgroundStyle06 + MusicUIObject::MColorStyle01);
    topWidget->setFixedHeight(35);

    QWidget *searchWidget = new QWidget(this);
    searchWidget->setStyleSheet(MusicUIObject::MBackgroundStyle15);
    searchWidget->setFixedHeight(35);
    QHBoxLayout *searchLayout = new QHBoxLayout(searchWidget);
    searchLayout->setContentsMargins(5, 0, 0, 0);
    searchWidget->setLayout(searchLayout);

    m_searchEdit = new MusicLocalSongSearchEdit(this);
    m_searchEdit->setFixedHeight(25);
    m_searchEdit->setStyleSheet(MusicUIObject::MBorderStyle04);
    QPushButton *searchButton = new QPushButton(searchWidget);
    searchButton->setStyleSheet(MusicUIObject::MBackgroundStyle01);
    searchButton->setIcon(QIcon(":/tiny/btn_search_main_hover"));
    searchButton->setCursor(QCursor(Qt::PointingHandCursor));
    searchButton->setIconSize(QSize(25, 25));
    QRadioButton *mvButton = new QRadioButton(tr("MV"), searchWidget);
    mvButton->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    QRadioButton *songButton = new QRadioButton(tr("Song"), searchWidget);
    songButton->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(mvButton, 0);
    group->addButton(songButton, 1);
    connect(group, SIGNAL(buttonClicked(int)), SLOT(setQueryMVFlag(int)));
    searchLayout->addWidget(mvButton);
    searchLayout->addWidget(songButton);
    searchLayout->addWidget(m_searchEdit);
    searchLayout->addWidget(searchButton);
#ifdef Q_OS_UNIX
    mvButton->setFocusPolicy(Qt::NoFocus);
    songButton->setFocusPolicy(Qt::NoFocus);
    searchButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_searchTableWidget = new MusicSoundKMicroSearchTableWidget(this);

    layout->addWidget(topWidget);
    layout->addWidget(searchWidget);
    layout->addWidget(m_searchTableWidget);
    m_container->setLayout(layout);

    m_queryMv = true;
    mvButton->setChecked(true);

    connect(searchButton, SIGNAL(clicked()), SLOT(startToSearch()));
    connect(m_searchEdit, SIGNAL(enterFinished(QString)), SLOT(startToSearch()));
}

MusicSoundKMicroSearchWidget::~MusicSoundKMicroSearchWidget()
{
    delete m_searchEdit;
    delete m_searchTableWidget;
}

QString MusicSoundKMicroSearchWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSoundKMicroSearchWidget::connectTo(QObject *obj)
{
    connect(m_searchTableWidget, SIGNAL(mvURLChanged(bool,QString,QString)), obj,
                                 SLOT(mvURLChanged(bool,QString,QString)));
}

void MusicSoundKMicroSearchWidget::startSeachKMicro(const QString &name)
{
    m_searchEdit->setText(name);
    startToSearch();
}

void MusicSoundKMicroSearchWidget::startToSearch()
{
    m_searchTableWidget->setQueryMVFlag(m_queryMv);
    m_searchTableWidget->startSearchQuery(m_searchEdit->text());
}
void MusicSoundKMicroSearchWidget::setQueryMVFlag(int flag)
{
    m_queryMv = (flag == 0);
}
