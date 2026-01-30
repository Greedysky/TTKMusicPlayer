#include "musicitemsearchtablewidget.h"
#include "musicdownloadqueryfactory.h"

MusicItemSearchTableWidget::MusicItemSearchTableWidget(QWidget *parent)
    : MusicQueryTableWidget(parent)
{
    m_searchActions = new QActionGroup(this);
    connect(m_searchActions, SIGNAL(triggered(QAction*)), SLOT(searchActionClicked(QAction*)));
}

MusicItemSearchTableWidget::~MusicItemSearchTableWidget()
{
    delete m_searchActions;
}

void MusicItemSearchTableWidget::searchActionClicked(QAction *action)
{
    const int row = currentRow();
    if(!isValid(row))
    {
        return;
    }

    const QString &songName = (row != -1 && rowCount() > 0) ? item(row, 1)->toolTip() : QString();
    const QString &artistName = (row != -1 && rowCount() > 0) ? item(row, 2)->toolTip() : QString();

    switch(action->data().toInt())
    {
        case 0: downloadQueryResult(row); break;
        case 1: Q_EMIT startToQueryByText(songName); break;
        case 2: Q_EMIT startToQueryByText(artistName); break;
        case 3: Q_EMIT startToQueryByText(TTK::generateSongName(songName, artistName)); break;
        default: break;
    }
}

void MusicItemSearchTableWidget::createContextMenu(QMenu &menu)
{
    if(!m_networkRequest)
    {
        return;
    }

    menu.setStyleSheet(TTK::UI::MenuStyle02);
    m_searchActions->addAction(menu.addAction(tr("Download")))->setData(0);
    menu.addSeparator();

    const int row = currentRow();
    const TTK::MusicSongInformationList &songInfos = m_networkRequest->items();
    if(!isValid(row) || row >= songInfos.count())
    {
        return;
    }

    const TTK::MusicSongInformation &info = songInfos[row];
    m_searchActions->addAction(menu.addAction(tr("Search '%1'").arg(info.m_songName)))->setData(1);
    m_searchActions->addAction(menu.addAction(tr("Search '%1'").arg(info.m_artistName)))->setData(2);
    m_searchActions->addAction(menu.addAction(tr("Search '%1 - %2'").arg(info.m_artistName, info.m_songName)))->setData(3);
}

QString MusicItemSearchTableWidget::randSimulation() const noexcept
{
    switch(TTK::random(5) + 3)
    {
        case 0:  return QString(":/video/lb_video_1");
        case 1:  return QString(":/video/lb_video_2");
        case 2:  return QString(":/video/lb_video_3");
        case 3:  return QString(":/video/lb_video_4");
        case 4:  return QString(":/video/lb_video_5");
        default: return QString(":/video/lb_video_5");
    }
}
