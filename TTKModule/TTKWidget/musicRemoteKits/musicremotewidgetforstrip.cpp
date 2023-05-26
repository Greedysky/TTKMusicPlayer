#include "musicremotewidgetforstrip.h"
#include "musictinyuiobject.h"
#include "musicmarqueewidget.h"

MusicRemoteWidgetForStrip::MusicRemoteWidgetForStrip(QWidget *parent)
    : MusicRemoteWidget(parent)
{
    setGeometry(200, 200, 320, 80);
    adjustPosition(this);
    setAttribute(Qt::WA_DeleteOnClose, false);
    setAttribute(Qt::WA_QuitOnClose, false);

    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setContentsMargins(5, 5, 5, 5);
    hbox->addWidget(m_mainWidget);
    setLayout(hbox);

    QWidget *topWidget = new QWidget(m_mainWidget);
    m_bottomWidget = new QWidget(m_mainWidget);

    QVBoxLayout *mhbox = new QVBoxLayout(m_mainWidget);
    mhbox->addWidget(topWidget);
    mhbox->addWidget(m_bottomWidget);
    m_mainWidget->setLayout(mhbox);

    m_songNameLabel = new MusicMarqueeWidget(this);
    m_lrcButton = new QPushButton(this);
    m_wallPaperButton = new QPushButton(this);
    m_listButton = new QPushButton(this);
    m_windowStateButton = new QPushButton(this);
    m_closeButton = new QToolButton(this);
#ifdef Q_OS_UNIX
    m_lrcButton->setFocusPolicy(Qt::NoFocus);
    m_wallPaperButton->setFocusPolicy(Qt::NoFocus);
    m_listButton->setFocusPolicy(Qt::NoFocus);
    m_windowStateButton->setFocusPolicy(Qt::NoFocus);
    m_closeButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_closeButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_songNameLabel->setStyleSheet(TTK::UI::WidgetStyle01);
    m_closeButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_lrcButton->setStyleSheet(TTK::UI::TinyBtnLrcOff);
    m_wallPaperButton->setStyleSheet(TTK::UI::TinyBtnWallpaperOff);
    m_listButton->setStyleSheet(TTK::UI::TinyBtnList);
    m_windowStateButton->setStyleSheet(TTK::UI::TinyBtnCollapse);

    m_lrcButton->setToolTip(tr("Lrc"));
    m_wallPaperButton->setToolTip(tr("WallPaper"));
    m_listButton->setToolTip(tr("List"));
    m_windowStateButton->setToolTip(tr("Collapse"));
    m_closeButton->setToolTip(tr("Close"));

    m_lrcButton->setFixedSize(22, 22);
    m_wallPaperButton->setFixedSize(22, 22);
    m_listButton->setFixedSize(22, 22);
    m_windowStateButton->setFixedSize(22, 22);
    m_closeButton->setFixedSize(22, 22);

    m_lrcButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_closeButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_wallPaperButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_listButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_windowStateButton->setCursor(QCursor(Qt::PointingHandCursor));

    QHBoxLayout *topWidgetBox = new QHBoxLayout(topWidget);
    topWidgetBox->setContentsMargins(0, 0, 0, 0);
    topWidgetBox->addWidget(m_songNameLabel);
    topWidgetBox->setSpacing(1);
    topWidgetBox->addWidget(m_windowStateButton);
    topWidgetBox->addWidget(m_settingButton);
    topWidgetBox->addWidget(m_closeButton);
    topWidget->setLayout(topWidgetBox);

    QHBoxLayout *bottomWidgetBox = new QHBoxLayout(m_bottomWidget);
    bottomWidgetBox->setContentsMargins(0, 0, 0, 0);
    bottomWidgetBox->setSpacing(1);
    bottomWidgetBox->addWidget(m_preSongButton);
    bottomWidgetBox->addWidget(m_playButton);
    bottomWidgetBox->addWidget(m_nextSongButton);
    bottomWidgetBox->addStretch(1);
    bottomWidgetBox->addWidget(m_volumeWidget);
    bottomWidgetBox->addWidget(m_showMainWindowButton);
    bottomWidgetBox->addWidget(m_lrcButton);
    bottomWidgetBox->addWidget(m_wallPaperButton);
    bottomWidgetBox->addWidget(m_listButton);
    m_bottomWidget->setLayout(bottomWidgetBox);

    connect(m_closeButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_windowStateButton, SIGNAL(clicked()), SLOT(windowStateChanged()));
}

MusicRemoteWidgetForStrip::~MusicRemoteWidgetForStrip()
{
    delete m_lrcButton;
    delete m_wallPaperButton;
    delete m_listButton;
    delete m_windowStateButton;
    delete m_closeButton;
    delete m_songNameLabel;
//    delete m_bottomWidget;
}

void MusicRemoteWidgetForStrip::setLabelText(const QString &value)
{
    m_songNameLabel->setText(TTK::Widget::elidedText(font(), value, Qt::ElideRight, width() - 20));
}

void MusicRemoteWidgetForStrip::windowStateChanged()
{
    const bool f = m_windowStateButton->styleSheet().contains("collapse");
    setFixedHeight(f ? 50 : 80);
    m_bottomWidget->setVisible(!f);
    m_windowStateButton->setStyleSheet(f ? TTK::UI::TinyBtnExpand : TTK::UI::TinyBtnCollapse);
    m_windowStateButton->setToolTip(f ? tr("Expand") : tr("Collapse"));
}

void MusicRemoteWidgetForStrip::show()
{
    MusicAbstractMoveWidget::show();
}

bool MusicRemoteWidgetForStrip::close()
{
    return MusicAbstractMoveWidget::close();
}

void MusicRemoteWidgetForStrip::contextMenuEvent(QContextMenuEvent *event)
{
    MusicAbstractMoveWidget::contextMenuEvent(event);
    QMenu menu(this);
    menu.setWindowFlags(menu.windowFlags() | Qt::FramelessWindowHint);
    menu.setAttribute(Qt::WA_TranslucentBackground);
    menu.setStyleSheet(TTK::UI::MenuStyle03);
    menu.addAction(QIcon(":/contextMenu/btn_selected"), tr("WindowTop"))->setEnabled(false);
    menu.addAction(tr("Show MainWindow"), this, SIGNAL(showMainWindow()));
    menu.addAction(tr("Quit"), this, SLOT(close()));
    menu.exec(QCursor::pos());
}
