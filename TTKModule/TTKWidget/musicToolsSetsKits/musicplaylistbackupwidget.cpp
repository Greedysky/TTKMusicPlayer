#include "musicplaylistbackupwidget.h"
#include "musicwidgetheaders.h"
#include "musictkplconfigmanager.h"

MusicPlaylistBackupTableWidget::MusicPlaylistBackupTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(2);

    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 350);
    headerView->resizeSection(1, 45);
}

void MusicPlaylistBackupTableWidget::addCellItems(const MusicSongList &items)
{
    setRowCount(items.count());
    QHeaderView *headerView = horizontalHeader();

    for(int i = 0; i < items.count(); ++i)
    {
        const MusicSong &v = items[i];

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(TTK::Widget::elidedText(font(), v.name(), Qt::ElideRight, headerView->sectionSize(0) - 10));
        item->setForeground(QColor(TTK::UI::Color01));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 0, item);

                          item = new QTableWidgetItem(v.duration());
        item->setForeground(QColor(TTK::UI::Color01));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);
    }
}



MusicPlaylistBackupWidget::MusicPlaylistBackupWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(TTK::UI::BackgroundStyle10 + TTK::UI::ColorStyle02);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainWidget->setLayout(mainLayout);
    layout->addWidget(mainWidget);
    setLayout(layout);
    //
    QWidget *topWidget = new QWidget(mainWidget);
    topWidget->setFixedHeight(60);
    QHBoxLayout *topWidgetLayout = new QHBoxLayout(topWidget);
    topWidgetLayout->setContentsMargins(15, 10, 20, 10);

    QLabel *pLabel = new QLabel(tr("Playlist Backup"), topWidget);
    QFont pLabelFont = pLabel->font();
    pLabelFont.setPixelSize(20);
    pLabel->setFont(pLabelFont);
    pLabel->setStyleSheet(TTK::UI::ColorStyle01);

    QLabel *iLabel = new QLabel(tr("Automatically backup your music"), topWidget);
    QFont iLabelFont = iLabel->font();
    iLabelFont.setPixelSize(15);
    iLabel->setFont(iLabelFont);
    iLabel->setStyleSheet(TTK::UI::ColorStyle03);

    m_dateBox = new QComboBox(topWidget);
    m_dateBox->setFixedSize(90, 25);
    TTK::Widget::generateComboBoxFormat(m_dateBox);

    m_timeBox = new QComboBox(topWidget);
    m_timeBox->setFixedSize(120, 25);
    TTK::Widget::generateComboBoxFormat(m_timeBox);

    topWidgetLayout->addWidget(pLabel);
    topWidgetLayout->addWidget(iLabel);
    topWidgetLayout->addStretch(1);
    topWidgetLayout->addWidget(m_dateBox);
    topWidgetLayout->addWidget(m_timeBox);
    topWidget->setLayout(topWidgetLayout);
    mainLayout->addWidget(topWidget);
    //
    QFrame *frame = new QFrame(mainWidget);
    frame->setFixedHeight(1);
    frame->setFrameShape(QFrame::HLine);
    frame->setFrameShadow(QFrame::Plain);
    frame->setStyleSheet(TTK::UI::ColorStyle05);
    mainLayout->addWidget(frame);
    //
    QWidget *functionWidget = new QWidget(mainWidget);
    QHBoxLayout *functionWidgetLayout = new QHBoxLayout(functionWidget);
    functionWidgetLayout->setContentsMargins(10, 0, 10, 0);
    functionWidget->setLayout(functionWidgetLayout);
    mainLayout->addWidget(functionWidget);
    //
    QWidget *listWidget = new QWidget(functionWidget);
    listWidget->setFixedWidth(200);
    QHBoxLayout *listWidgetLayout = new QHBoxLayout(listWidget);
    listWidgetLayout->setContentsMargins(0, 10, 0, 10);
    listWidget->setLayout(listWidgetLayout);

    m_listWidget = new QListWidget(listWidget);
    m_listWidget->setStyleSheet(TTK::UI::BorderStyle01);
    listWidgetLayout->addWidget(m_listWidget);
    //
    QFrame *hFrame = new QFrame(functionWidget);
    hFrame->setFixedWidth(1);
    hFrame->setFrameShape(QFrame::VLine);
    hFrame->setFrameShadow(QFrame::Plain);
    hFrame->setStyleSheet(TTK::UI::ColorStyle05);
    //
    QWidget *containerWidget = new QWidget(functionWidget);
    QVBoxLayout *containerWidgetLayout = new QVBoxLayout(containerWidget);
    containerWidgetLayout->setContentsMargins(10, 10, 10, 10);
    containerWidget->setLayout(containerWidgetLayout);

    functionWidgetLayout->addWidget(listWidget);
    functionWidgetLayout->addWidget(hFrame);
    functionWidgetLayout->addWidget(containerWidget);
    //
    QWidget *containerTopWidget = new QWidget(containerWidget);
    containerTopWidget->setFixedHeight(30);
    m_tableWidget = new MusicPlaylistBackupTableWidget(containerWidget);

    containerWidgetLayout->addWidget(containerTopWidget);
    containerWidgetLayout->addWidget(m_tableWidget);

    QHBoxLayout *containerTopWidgetLayout = new QHBoxLayout(containerTopWidget);
    containerTopWidgetLayout->setContentsMargins(0, 0, 0, 0);
    containerTopWidget->setLayout(containerTopWidgetLayout);
    //
    QFrame *blueFrame = new QFrame(containerTopWidget);
    blueFrame->setFixedWidth(3);
    blueFrame->setFrameShape(QFrame::VLine);
    blueFrame->setFrameShadow(QFrame::Plain);
    blueFrame->setStyleSheet(TTK::UI::ColorStyle08);

    m_titleLabel = new QLabel(containerTopWidget);
    pLabelFont = m_titleLabel->font();
    pLabelFont.setPixelSize(16);
    m_titleLabel->setFont(pLabelFont);
    //
    containerTopWidgetLayout->addWidget(blueFrame);
    containerTopWidgetLayout->addWidget(m_titleLabel);
    containerTopWidgetLayout->addStretch(1);
    containerTopWidgetLayout->addWidget(new QPushButton(containerTopWidget));

    initialize();

    connect(m_dateBox, SIGNAL(currentTextChanged(QString)), SLOT(currentDateChanged(QString)));
    connect(m_timeBox, SIGNAL(currentTextChanged(QString)), SLOT(currentTimeChanged(QString)));
    connect(m_listWidget, SIGNAL(currentRowChanged(int)), SLOT(currentItemChanged(int)));
}

MusicPlaylistBackupWidget::~MusicPlaylistBackupWidget()
{
    delete m_dateBox;
    delete m_timeBox;
    delete m_titleLabel;
    delete m_listWidget;
    delete m_tableWidget;
}

void MusicPlaylistBackupWidget::resizeWidget()
{

}

void MusicPlaylistBackupWidget::currentDateChanged(const QString &text)
{
    m_timeBox->clear();

    QDir dir(TTK_STR_CAT(APPBACKUP_DIR_FULL, "playlist/") + text);
    for(const QFileInfo &fin : dir.entryInfoList(QDir::Files, QDir::Time | QDir::Reversed))
    {
        m_timeBox->addItem(fin.baseName());
    }

    currentTimeChanged(m_timeBox->currentText());
}

void MusicPlaylistBackupWidget::currentTimeChanged(const QString &text)
{
    m_listWidget->clear();

    MusicTKPLConfigManager manager;
    if(!manager.fromFile(TTK_STR_CAT(APPBACKUP_DIR_FULL, "playlist/") + m_dateBox->currentText() + "/" + text + TKF_FILE))
    {
        TTK_LOG_STREAM("Backup playlist read error");
        return;
    }

    manager.readBuffer(m_items);

    for(const MusicSongItem &item : qAsConst(m_items))
    {
        m_listWidget->addItem(QString("%1[%2]").arg(item.m_itemName).arg(item.m_songs.count()));
    }

    currentItemChanged(0);
}

void MusicPlaylistBackupWidget::currentItemChanged(int index)
{
    const MusicSongItem &item = m_items[index];
    m_titleLabel->setText(QString("%1[%2]").arg(item.m_itemName).arg(item.m_songs.count()));
    m_tableWidget->addCellItems(item.m_songs);
}

void MusicPlaylistBackupWidget::initialize()
{
    QDir dir(TTK_STR_CAT(APPBACKUP_DIR_FULL, "playlist"));
    m_dateBox->addItems(dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Time | QDir::Reversed));

    currentDateChanged(m_dateBox->currentText());
}
