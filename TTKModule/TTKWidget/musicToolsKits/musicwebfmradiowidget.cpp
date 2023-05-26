#include "musicwebfmradiowidget.h"
#include "musicfmradiochannelrequest.h"
#include "musicwebfmradioplaywidget.h"
#include "musiccoversourcerequest.h"
#include "musicwidgetheaders.h"
#include "musicitemdelegate.h"
#include "musicimageutils.h"

#define ICON_SIZE       50

MusicWebFMRadioWidget::MusicWebFMRadioWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent),
      m_fmRadio(nullptr)
{
    setIconSize(QSize(ICON_SIZE, ICON_SIZE));
    setColumnCount(4);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, ICON_SIZE + 20);
    headerview->resizeSection(2, 157);
    headerview->resizeSection(3, 75);

    TTK::Widget::setTransparent(this, 0);
    verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle03);

    connect(this, SIGNAL(cellDoubleClicked(int,int)), SLOT(itemDoubleClicked(int,int)));

    m_channelThread = new MusicFMRadioChannelRequest(this);
    connect(m_channelThread, SIGNAL(downLoadDataChanged(QString)), SLOT(addListWidgetItem()));
    m_channelThread->startRequest(QString());
}

MusicWebFMRadioWidget::~MusicWebFMRadioWidget()
{
    delete m_fmRadio;
    delete m_channelThread;
}

void MusicWebFMRadioWidget::itemCellEntered(int row, int column)
{
    QTableWidgetItem *it = item(m_previousColorRow, 3);
    if(it)
    {
        it->setIcon(QIcon());
    }

    if((it = item(row, 3)))
    {
        it->setIcon(QIcon(":/contextMenu/btn_play"));
    }

    if(column == 3)
    {
        setCursor(QCursor(Qt::PointingHandCursor));
    }
    else
    {
        unsetCursor();
    }

    MusicAbstractTableWidget::itemCellEntered(row, column);
}

void MusicWebFMRadioWidget::itemCellClicked(int row, int column)
{
    Q_UNUSED(row);
    if(column == 3)
    {
        itemDoubleClicked(row, TTK_NORMAL_LEVEL);
    }
}

void MusicWebFMRadioWidget::itemDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    if(!m_channelThread)
    {
        return;
    }

    const MusicFMRadioChannelDataList &channels = m_channelThread->musicChannel();
    if(m_fmRadio == nullptr)
    {
        m_fmRadio = new MusicWebFMRadioPlayWidget(this);
    }

    if(!channels.isEmpty())
    {
        m_fmRadio->updateRadioSong(channels[row].m_id);
    }
    m_fmRadio->show();
}

void MusicWebFMRadioWidget::addListWidgetItem()
{
    const MusicFMRadioChannelDataList &channels = m_channelThread->musicChannel();
    for(const MusicFMRadioChannelData &channel : qAsConst(channels))
    {
        const int index = rowCount();
        setRowCount(index + 1);
        setRowHeight(index, TTK_ITEM_SIZE_XL);

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(TTK_DATA_ROLE, channel.m_coverUrl);
        setItem(index, 0, item);

                          item = new QTableWidgetItem;
        item->setIcon(TTK::Image::roundedPixmap(QPixmap(":/image/lb_default_art"), QPixmap(":/image/lb_mask_50"), iconSize()));
        setItem(index, 1, item);

                          item = new QTableWidgetItem;
        QFont font = item->font();
        font.setBold(true);
        item->setFont(font);
        item->setText(channel.m_name);
        item->setForeground(QColor(TTK::UI::Color02));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(index, 2, item);

                          item = new QTableWidgetItem;
        setItem(index, 3, item);

        if(!channel.m_coverUrl.isEmpty() && channel.m_coverUrl != TTK_NULL_STR)
        {
            MusicCoverSourceRequest *d = new MusicCoverSourceRequest(this);
            connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
            d->setHeader("id", index);
            d->startRequest(channel.m_coverUrl);
        }
    }
}

void MusicWebFMRadioWidget::downLoadFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Input byte data is empty");
        return;
    }

    MusicCoverSourceRequest *d(TTKObjectCast(MusicCoverSourceRequest*, sender()));
    if(!d)
    {
        return;
    }

    QTableWidgetItem *it = item(d->header("id").toInt(), 1);
    if(it)
    {
        QPixmap pix;
        pix.loadFromData(bytes);
        it->setIcon(TTK::Image::roundedPixmap(pix, QPixmap(":/image/lb_mask_50"), iconSize()));
    }
}

void MusicWebFMRadioWidget::playClicked()
{
    const int row = currentRow();
    if(row >= 0)
    {
        itemDoubleClicked(row, TTK_NORMAL_LEVEL);
    }
}

void MusicWebFMRadioWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicAbstractTableWidget::contextMenuEvent(event);

    QMenu menu(this);
    menu.setStyleSheet(TTK::UI::MenuStyle02);

    menu.addAction(tr("Play"), this, SLOT(playClicked()));
    menu.addSeparator();
    menu.addAction(QIcon(":/contextMenu/btn_mobile"), tr("Song To Mobile"));

    menu.exec(QCursor::pos());
}
