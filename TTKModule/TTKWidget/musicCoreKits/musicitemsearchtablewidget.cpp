#include "musicitemsearchtablewidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicgiflabelwidget.h"

#include <qmath.h>

MusicItemSearchTableWidget::MusicItemSearchTableWidget(QWidget *parent)
    : MusicQueryTableWidget(parent)
{
    m_loadingLabel = new MusicGifLabelWidget(MusicGifLabelWidget::Module::CicleBlue, this);
    m_actionGroup = new QActionGroup(this);

    m_labelDelegate = new TTKLabelItemDelegate(this);
    m_labelDelegate->setStyleSheet(TTK::UI::BackgroundStyle13);

    connect(m_actionGroup, SIGNAL(triggered(QAction*)), SLOT(actionGroupClick(QAction*)));
}

MusicItemSearchTableWidget::~MusicItemSearchTableWidget()
{
    delete m_loadingLabel;
    delete m_actionGroup;
    delete m_labelDelegate;
}

void MusicItemSearchTableWidget::startSearchQuery(const QString &text)
{
    Q_UNUSED(text);
    MusicAbstractQueryRequest *d = G_DOWNLOAD_QUERY_PTR->makeQueryRequest(this);
    connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
    setQueryInput(d);
}

void MusicItemSearchTableWidget::itemCellClicked(int row, int column)
{
    MusicQueryTableWidget::itemCellClicked(row, column);

    if(rowCount() > 0 && row == rowCount() - 1)
    {
        QTableWidgetItem *it = item(row, 0);
        if(it && it->data(TTK_DISPLAY_ROLE).toString() == tr("Query more data"))
        {
            setItemDelegateForRow(row, nullptr);
            clearSpans();
            removeRow(row);

            m_loadingLabel->run(true);
            m_networkRequest->startToPage(m_networkRequest->pageIndex() + 1);
        }
    }
}

void MusicItemSearchTableWidget::removeItems()
{
    if(rowCount() > 0)
    {
        setItemDelegateForRow(rowCount() - 1, nullptr);
    }
    MusicQueryTableWidget::removeItems();
}

void MusicItemSearchTableWidget::actionGroupClick(QAction *action)
{
    const int row = currentRow();
    if(row < 0 || (row >= rowCount() - 1))
    {
        return;
    }

    const QString &songName = (row != -1 && rowCount() > 0) ? item(row, 1)->toolTip() : QString();
    const QString &artistName = (row != -1 && rowCount() > 0) ? item(row, 2)->toolTip() : QString();

    switch(action->data().toInt())
    {
        case 0: downloadQuery(row); break;
        case 1: Q_EMIT restartSearchQuery(songName); break;
        case 2: Q_EMIT restartSearchQuery(artistName); break;
        case 3: Q_EMIT restartSearchQuery(artistName + " - " + songName); break;
        default: break;
    }
}

void MusicItemSearchTableWidget::createFinishedItem()
{
    m_loadingLabel->run(false);

    setRowCount(rowCount() + 1);
    const int count = rowCount() - 1;
    for(int i = 0; i < columnCount(); ++i)
    {
        setItem(count, i, new QTableWidgetItem);
    }
    setSpan(count, 0, 1, columnCount());

    QTableWidgetItem *it = item(count, 0);
    if(it)
    {
        const int pageTotal = ceil(m_networkRequest->totalSize() * 1.0 / m_networkRequest->pageSize());
        const bool more = (pageTotal > m_networkRequest->pageIndex() + 1);
        it->setData(TTK_DISPLAY_ROLE, more ? tr("Query more data") : tr("No more data"));
        setItemDelegateForRow(count, m_labelDelegate);
    }
}

void MusicItemSearchTableWidget::resizeEvent(QResizeEvent *event)
{
    MusicQueryTableWidget::resizeEvent(event);
    m_loadingLabel->move((width() - m_loadingLabel->width()) / 2, (height() - m_loadingLabel->height()) / 2);
}

void MusicItemSearchTableWidget::createContextMenu(QMenu &menu)
{
    if(!m_networkRequest)
    {
        return;
    }

    menu.setStyleSheet(TTK::UI::MenuStyle02);
    m_actionGroup->addAction(menu.addAction(tr("Download")))->setData(0);
    menu.addSeparator();

    const int row = currentRow();
    const TTK::MusicSongInformationList songInfos(m_networkRequest->songInfoList());
    if(row < 0 || row >= songInfos.count())
    {
        return;
    }

    const TTK::MusicSongInformation &info = songInfos[row];
    m_actionGroup->addAction(menu.addAction(tr("Search '%1'").arg(info.m_songName)))->setData(1);
    m_actionGroup->addAction(menu.addAction(tr("Search '%1'").arg(info.m_singerName)))->setData(2);
    m_actionGroup->addAction(menu.addAction(tr("Search '%1 - %2'").arg(info.m_singerName, info.m_songName)))->setData(3);
}

QString MusicItemSearchTableWidget::randSimulation() const
{
    switch(TTK::random(5))
    {
        case 0:  return QString(":/video/lb_video_1");
        case 1:  return QString(":/video/lb_video_2");
        case 2:  return QString(":/video/lb_video_3");
        case 3:  return QString(":/video/lb_video_4");
        case 4:  return QString(":/video/lb_video_5");
        default: return QString(":/video/lb_video_5");
    }
}
