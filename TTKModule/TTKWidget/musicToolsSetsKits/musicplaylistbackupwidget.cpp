#include "musicplaylistbackupwidget.h"
#include "musicwidgetheaders.h"
#include "musicwidgetutils.h"
#include "musictkplconfigmanager.h"

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
    QWidget *topWidget = new QWidget(this);
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
    QFrame *frame = new QFrame(this);
    frame->setFixedHeight(1);
    frame->setFrameShape(QFrame::HLine);
    frame->setFrameShadow(QFrame::Plain);
    frame->setStyleSheet(TTK::UI::ColorStyle05);
    mainLayout->addWidget(frame);
    //
    QWidget *functionWidget = new QWidget(this);
    functionWidget->setStyleSheet(TTK::UI::BackgroundStyle01);
    QHBoxLayout *functionWidgetLayout = new QHBoxLayout(functionWidget);
    functionWidgetLayout->setContentsMargins(10, 10, 10, 10);
    functionWidget->setLayout(functionWidgetLayout);
    mainLayout->addWidget(functionWidget);

    m_listWidget = new QListWidget(functionWidget);
    m_listWidget->setFixedWidth(200);
    functionWidgetLayout->addWidget(m_listWidget);

    QWidget *widget = new QWidget(functionWidget);
    functionWidgetLayout->addWidget(widget);

    initialize();

    connect(m_dateBox, SIGNAL(currentTextChanged(QString)), SLOT(currentDateChanged(QString)));
    connect(m_timeBox, SIGNAL(currentTextChanged(QString)), SLOT(currentTimeChanged(QString)));
}

MusicPlaylistBackupWidget::~MusicPlaylistBackupWidget()
{
    delete m_dateBox;
    delete m_timeBox;
    delete m_listWidget;
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

    MusicSongItemList items;
    manager.readBuffer(items);

    for(const MusicSongItem &item : qAsConst(items))
    {
        m_listWidget->addItem(item.m_itemName);
    }
}

void MusicPlaylistBackupWidget::initialize()
{
    QDir dir(TTK_STR_CAT(APPBACKUP_DIR_FULL, "playlist"));
    m_dateBox->addItems(dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Time | QDir::Reversed));
}
