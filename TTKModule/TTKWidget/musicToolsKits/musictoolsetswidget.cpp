#include "musictoolsetswidget.h"
#include "ui_musictoolsetswidget.h"
#include "musictimerwidget.h"
#include "musictransformwidget.h"
#include "musicnetworkconnectiontestwidget.h"
#include "musicreplaygainwidget.h"
#include "musicsongdlnatransferwidget.h"
#include "musicsongringtonemakerwidget.h"
#include "musicapplication.h"
#include "musicrightareawidget.h"
#include "musicspectrumwidget.h"

MusicToolSetsWidget::MusicToolSetsWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicToolSetsWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setAttribute(Qt::WA_DeleteOnClose);
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->listItemWidget->setSpacing(11);
    m_ui->listItemWidget->setIconSize(QSize(60, 60));
    m_ui->listItemWidget->setFrameShape(QFrame::NoFrame);
    m_ui->listItemWidget->setViewMode(QListView::IconMode);
    m_ui->listItemWidget->setMovement(QListView::Static);
    m_ui->listItemWidget->setStyleSheet(TTK::UI::ListWidgetStyle02);
    m_ui->listItemWidget->verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle01);

    connect(m_ui->listItemWidget, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(currentItemClicked(QListWidgetItem*)));

    addCelltItems();
}

MusicToolSetsWidget::~MusicToolSetsWidget()
{
    TTK_REMOVE_SINGLE_WIDGET(this);
    clear();
    delete m_ui;
}

void MusicToolSetsWidget::addCelltItems()
{
    struct Data
    {
        QString m_icon;
        QString m_name;

        Data() = default;
        Data(const QString &icon, const QString &name) noexcept
            : m_icon(icon),
              m_name(name)
        {

        }
    };
    TTK_DECLARE_LIST(Data);

    DataList pairs;
    pairs << Data(":/tools/lb_bell", tr("Bell"))
          << Data(":/tools/lb_timer", tr("Timing"))
          << Data(":/tools/lb_transform", tr("Transform"))
          << Data(":/tools/lb_spectrum", tr("Spectrum"))
          << Data(":/tools/lb_connections" ,tr("Connection"))
          << Data(":/tools/lb_gain", tr("Gain"))
          << Data(":/tools/lb_dlna", tr("DLNA"))
          << Data(":/tools/lb_detect", tr("Identify"))
          << Data(":/tools/lb_screen_saver", tr("Saver"))
          << Data(":/tools/lb_playlist", tr("Playlist"));

    for(const Data &pair : qAsConst(pairs))
    {
        QListWidgetItem *item = new QListWidgetItem(QIcon(pair.m_icon), pair.m_name, m_ui->listItemWidget);
        item->setForeground(QColor(TTK::UI::Color01));
        item->setSizeHint(QSize(80, 90));
        m_ui->listItemWidget->addItem(item);
    }
}

void MusicToolSetsWidget::currentItemClicked(QListWidgetItem *item)
{
    hide();
    //
    switch(m_ui->listItemWidget->row(item))
    {
        case 0:
        {
            MusicSongRingtoneMaker(this).exec();
            break;
        }
        case 1:
        {
            MusicTimerWidget widget(this);
            QStringList songlist;
            MusicApplication::instance()->currentPlaylist(songlist);
            widget.setSongStringList(songlist);
            widget.exec();
            break;
        }
        case 2:
        {
            MusicTransformWidget(this).exec();
            break;
        }
        case 3:
        {
            TTK_GENERATE_SINGLE_WIDGET(MusicSpectrumWidget);
            break;
        }
        case 4:
        {
            TTK_GENERATE_SINGLE_WIDGET(MusicNetworkConnectionTestWidget);
            break;
        }
        case 5:
        {
            TTK_GENERATE_SINGLE_WIDGET(MusicReplayGainWidget);
            break;
        }
        case 6:
        {
            TTK_GENERATE_SINGLE_WIDGET(MusicSongDlnaTransferWidget);
            break;
        }
        case 7:
        {
            MusicRightAreaWidget::instance()->functionClicked(MusicRightAreaWidget::IndentifyWidget);
            break;
        }
        case 8:
        {
            MusicRightAreaWidget::instance()->functionClicked(MusicRightAreaWidget::ScreenSaverWidget);
            break;
        }
        case 9:
        {
            MusicRightAreaWidget::instance()->functionClicked(MusicRightAreaWidget::PlaylistBackupWidget);
            break;
        }
        default: break;
    }
}

void MusicToolSetsWidget::clear()
{
    m_ui->listItemWidget->clear();
}

void MusicToolSetsWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    QListWidgetItem *it = m_ui->listItemWidget->currentItem();
    if(it)
    {
        currentItemClicked(it);
    }
}
