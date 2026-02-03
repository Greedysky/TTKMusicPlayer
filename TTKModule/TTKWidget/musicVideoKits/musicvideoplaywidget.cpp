#include "musicvideoplaywidget.h"
#include "musicvideoview.h"
#include "musicitemsearchedit.h"
#include "musicvideofloatwidget.h"
#include "musicsongsharingwidget.h"
#include "musicfunctionuiobject.h"
#include "musictinyuiobject.h"
#include "musicapplication.h"
#include "ttkdesktopscreen.h"

#include <QParallelAnimationGroup>

static constexpr int WINDOW_HEIGHT = 508;
static constexpr int WINDOW_WIDTH = 678;

MusicVideoPlayWidget::MusicVideoPlayWidget(QWidget *parent)
    : TTKAbstractMoveWidget(false, parent),
      m_backButton(nullptr)
{
    setWindowTitle(tr("TTKMovie"));

    m_leaverTimer = new QTimer(this);
    m_leaverTimer->setInterval(4 * TTK_DN_S2MS);
    m_leaverTimer->setSingleShot(true);
    connect(m_leaverTimer, SIGNAL(timeout()), SLOT(leaveTimeout()));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_topWidget = new QWidget(this);
    m_topWidget->setStyleSheet(TTK::UI::BackgroundStyle08 + TTK::UI::BorderStyle01);

    QHBoxLayout *topLayout = new QHBoxLayout(m_topWidget);
    topLayout->setContentsMargins(9, 4, 9, 4);

    m_textLabel = new QLabel(m_topWidget);
    m_textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_textLabel->setStyleSheet(TTK::UI::ColorStyle06);

    m_searchEdit = new MusicItemQueryEdit(m_topWidget);
    m_searchEdit->setFixedHeight(25);

    topLayout->addWidget(m_textLabel);
    topLayout->addStretch();
    topLayout->addWidget(m_searchEdit);

    m_closeButton = new QPushButton(this);
    m_closeButton->setToolTip(tr("Close"));
    m_closeButton->setFixedSize(14, 14);
    m_closeButton->setStyleSheet(TTK::UI::BtnPClose);
    m_closeButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_closeButton, SIGNAL(clicked()), parent, SLOT(videoNeedToClose()));
    topLayout->addWidget(m_closeButton);
    m_topWidget->setLayout(topLayout);

#ifdef Q_OS_UNIX
    m_closeButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_stackedWidget = new QStackedWidget(this);
    m_stackedWidget->setStyleSheet(TTK::UI::BorderStyle01);

    QWidget *topMaskWidget = new QWidget(this);
    topMaskWidget->setFixedHeight(35);
    topMaskWidget->setStyleSheet(TTK::UI::BackgroundStyle02);

    layout->addWidget(topMaskWidget);
    layout->addWidget(m_stackedWidget);
    setLayout(layout);

    m_searchEdit->hide();
    m_topWidget->raise();

    m_videoFloatWidget = new MusicVideoFloatWidget(this);
    m_tableWidget = new MusicVideoSearchTableWidget(this);
    m_videoView = new MusicVideoView(this);

    QWidget *videoMaskWidget = new QWidget(this);
    videoMaskWidget->setObjectName("videoMaskWidget");
    videoMaskWidget->setStyleSheet(QString("#%1{ %2 }").arg(videoMaskWidget->objectName(), TTK::UI::BackgroundStyle10));

    QHBoxLayout *videoMaskWidgetLayout = new QHBoxLayout(videoMaskWidget);
    videoMaskWidgetLayout->setContentsMargins(0, 0, 0, 0);
    videoMaskWidgetLayout->addWidget(m_tableWidget);

    m_stackedWidget->addWidget(m_videoView);
    m_stackedWidget->addWidget(videoMaskWidget);
    m_stackedWidget->setCurrentIndex(0);

    m_leaverAnimation = new QParallelAnimationGroup(this);
    QPropertyAnimation *topAnimation = new QPropertyAnimation(m_topWidget, "pos", m_leaverAnimation);
    topAnimation->setDuration(TTK_DN_S2MS / 2);
    QPropertyAnimation *ctrlAnimation = new QPropertyAnimation(m_videoView->controlBarWidget(), "pos", m_leaverAnimation);
    ctrlAnimation->setDuration(TTK_DN_S2MS / 2);
    m_leaverAnimation->addAnimation(topAnimation);
    m_leaverAnimation->addAnimation(ctrlAnimation);

    connect(m_tableWidget, SIGNAL(mediaUrlPathChanged(MusicVideoItem)), SLOT(mediaUrlPathChanged(MusicVideoItem)));
    connect(m_tableWidget, SIGNAL(startToQueryByText(QString)), SLOT(videoResearchButtonSearched(QString)));
    connect(m_searchEdit, SIGNAL(trigger(QString)), SLOT(videoResearchButtonSearched(QString)));

    connect(m_videoFloatWidget, SIGNAL(searchButtonClicked()), SLOT(switchToSearchTable()));
    connect(m_videoFloatWidget, SIGNAL(popupButtonClicked()), SLOT(popupButtonClicked()));
    connect(m_videoFloatWidget, SIGNAL(fullscreenButtonClicked()), SLOT(fullscreenButtonClicked()));
    connect(m_videoFloatWidget, SIGNAL(downloadButtonClicked()), SLOT(downloadButtonClicked()));
    connect(m_videoFloatWidget, SIGNAL(shareButtonClicked()), SLOT(shareButtonClicked()));

    connect(m_videoView, SIGNAL(searchButtonClicked()), SLOT(switchToSearchTable()));
    connect(m_videoView, SIGNAL(popupButtonClicked()), SLOT(popupButtonClicked()));
    connect(m_videoView, SIGNAL(fullscreenButtonClicked()), SLOT(fullscreenButtonClicked()));
    connect(m_videoView, SIGNAL(downloadButtonClicked()), SLOT(downloadButtonClicked()));
    connect(m_videoView, SIGNAL(shareButtonClicked()), SLOT(shareButtonClicked()));
}

MusicVideoPlayWidget::~MusicVideoPlayWidget()
{
    delete m_leaverTimer;
    delete m_leaverAnimation;
    delete m_closeButton;
    delete m_textLabel;
    delete m_searchEdit;
    delete m_backButton;
    delete m_videoView;
    delete m_stackedWidget;
}

void MusicVideoPlayWidget::popupMode(bool popup)
{
    m_videoFloatWidget->popupMode(popup);
    setBlockOption(!popup);

    if(popup)
    {
        const QRect &rect = TTKDesktopScreen::currentGeometry();
        setGeometry(rect.x() + (rect.width() - WINDOW_WIDTH) / 2,
                    rect.y() + (rect.height() - WINDOW_HEIGHT) / 2,
                    WINDOW_WIDTH, WINDOW_HEIGHT);
        resizeGeometry(0, 0);
        setParent(nullptr);
        show();
    }
    else
    {
#ifdef Q_OS_UNIX
        if(isFullScreen())
        {
            showNormal();
        }
#endif
        m_videoFloatWidget->fullscreenMode(false);
    }
}

bool MusicVideoPlayWidget::isPopupMode() const
{
    return !m_blockOption;
}

void MusicVideoPlayWidget::resizeGeometry()
{
    const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    const int height = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().height();

    if(!isFullScreen())
    {
#ifdef Q_OS_UNIX
        const bool v = isPopupMode();
        resizeGeometry(v ? (this->width() - 680) : (width - WINDOW_WIDTH_MIN), v ? (this->height() - 508) : (height - WINDOW_HEIGHT_MIN));
#else
        resizeGeometry(width - WINDOW_WIDTH_MIN, height - WINDOW_HEIGHT_MIN);
#endif
    }
}

void MusicVideoPlayWidget::resizeGeometry(bool resize)
{
    if(resize)
    {
        showFullScreen();
    }
    else
    {
        showNormal();
        setGeometry(250, 150, WINDOW_WIDTH, WINDOW_HEIGHT);
    }

    QSize s = size();
#ifdef Q_OS_UNIX
    const QRect &rect = TTKDesktopScreen::currentGeometry();
    if(isFullScreen() && !rect.isNull())
    {
        s = rect.size();
    }
    else
    {
        s = QSize(WINDOW_WIDTH, WINDOW_HEIGHT);
        showNormal();
    }
#endif
    resizeGeometry(s.width() - WINDOW_WIDTH, s.height() - WINDOW_HEIGHT);
}

void MusicVideoPlayWidget::resizeGeometry(int width, int height)
{
    m_topWidget->setGeometry(0, 0, 680 + width, 35);
    m_tableWidget->resizeSection(width);
    m_videoView->resizeGeometry(width, height);
    m_videoFloatWidget->resizeGeometry(width, height);
}

void MusicVideoPlayWidget::switchToSearchTable()
{
    QHBoxLayout *topLayout = TTKObjectCast(QHBoxLayout*, m_topWidget->layout());
    delete m_backButton;
    m_backButton = new QToolButton(m_topWidget);
    m_backButton->setFixedSize(20, 20);
    m_backButton->setToolTip(tr("Back"));
    m_backButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_backButton->setStyleSheet(TTK::UI::BtnBackBack);
    connect(m_backButton, SIGNAL(clicked()), SLOT(switchToPlayView()));
    topLayout->insertWidget(0, m_backButton);

    m_textLabel->clear();
    m_searchEdit->show();
    m_stackedWidget->setCurrentIndex(1);
}

void MusicVideoPlayWidget::switchToPlayView()
{
    delete m_backButton;
    m_backButton = nullptr;

    setTitleText(m_videoItem.m_name);
    m_searchEdit->hide();
    m_stackedWidget->setCurrentIndex(0);
}

void MusicVideoPlayWidget::videoResearchButtonSearched(const QString &name)
{
    switchToSearchTable();
    m_searchEdit->editor()->setText(name);
    m_tableWidget->startToSearchByValue(name);
}

void MusicVideoPlayWidget::videoResearchButtonSearched(const QVariant &data)
{
    m_tableWidget->startToSearchByID(data);

    const TTK::MusicSongInformation info(data.value<TTK::MusicSongInformation>());
    const TTK::MusicSongPropertyList &props = info.m_songProps;
    if(!props.isEmpty())
    {
        const TTK::MusicSongProperty &prop = props.first();
        MusicVideoItem data;
        data.m_name = TTK::generateSongName(info.m_songName, info.m_artistName);
        data.m_url = prop.m_url;
        data.m_id = info.m_songId;
        data.m_cover = info.m_coverUrl;
        data.m_server = MUSIC_MOVIE_RADIO;
        mediaUrlPathChanged(data);
    }
}

void MusicVideoPlayWidget::startToSearchByID(const QString &name)
{
    switchToSearchTable();
    m_tableWidget->startToSearchByID(name);
}

void MusicVideoPlayWidget::mediaUrlChanged(const QString &url)
{
    if(url.isEmpty())
    {
        return;
    }

    MusicApplication *w = MusicApplication::instance();
    if(w->isPlaying())
    {
        w->switchToPlayState();
    }

    ///stop current song play while mv starts.
    m_videoView->setMedia(url);
    m_videoView->play();

    switchToPlayView();
}

void MusicVideoPlayWidget::mediaUrlPathChanged(const MusicVideoItem &item)
{
    m_videoItem = item;
    setTitleText(item.m_name);
    mediaUrlChanged(item.m_url);
    //
    m_videoView->setBarrage(item.m_name, item.m_id);
}

void MusicVideoPlayWidget::popupButtonClicked()
{
    Q_EMIT popupButtonClicked(!m_videoFloatWidget->isPopupMode());
}

void MusicVideoPlayWidget::fullscreenButtonClicked()
{
    if(!m_videoFloatWidget->isPopupMode())
    {
        return;
    }

    const bool full = !m_videoFloatWidget->isFullscreenMode();
    m_videoFloatWidget->fullscreenMode(full);
    Q_EMIT fullscreenButtonClicked(full);
}

void MusicVideoPlayWidget::downloadButtonClicked()
{
    m_tableWidget->downloadLocalFromControl();
}

void MusicVideoPlayWidget::shareButtonClicked()
{
    const QString &name = m_videoItem.m_name.trimmed();
    const QString &id = m_videoItem.m_id.trimmed();

    if(name.isEmpty() || id.isEmpty())
    {
        return;
    }

    MusicSongSharingWidget::Data item;
    item.m_id = id;
    item.m_name = name;
    item.m_cover = m_videoItem.m_cover;
    item.m_server = m_videoItem.m_server;

    MusicSongSharingWidget widget(this);
    widget.initialize(MusicSongSharingWidget::Module::Movie, item);
    widget.exec();
}

void MusicVideoPlayWidget::leaveTimeout()
{
    QWidget *w(m_videoView->controlBarWidget());
    if(w->y() != height())
    {
        const int topHeight = (m_stackedWidget->currentIndex() == 1) ? 0 : -m_topWidget->height();
        start(0, topHeight, height() - w->height() - m_topWidget->height(), height());
    }
}

void MusicVideoPlayWidget::resizeEvent(QResizeEvent *event)
{
    TTKAbstractMoveWidget::resizeEvent(event);
    resizeGeometry();

    m_leaverTimer->stop();
    m_leaverAnimation->stop();
}

void MusicVideoPlayWidget::enterEvent(QtEnterEvent *event)
{
    TTKAbstractMoveWidget::enterEvent(event);
    m_leaverTimer->stop();

    QWidget *w(m_videoView->controlBarWidget());
    if(w->y() >= height())
    {
        const int topHeight = (m_stackedWidget->currentIndex() == 1) ? 0 : -m_topWidget->height();
        start(topHeight, 0, height(), height() - w->height() - m_topWidget->height());
    }
}

void MusicVideoPlayWidget::leaveEvent(QEvent *event)
{
    TTKAbstractMoveWidget::leaveEvent(event);
    m_leaverTimer->start();
}

void MusicVideoPlayWidget::setTitleText(const QString &text)
{
    m_textLabel->setText(TTK::Widget::elidedText(font(), text, Qt::ElideRight, width() - 50));
}

void MusicVideoPlayWidget::start(int st, int end, int ctrlst, int ctrlend)
{
    m_leaverAnimation->stop();
    QPropertyAnimation *animation = TTKObjectCast(QPropertyAnimation*, m_leaverAnimation->animationAt(0));
    animation->setStartValue(QPoint(0, st));
    animation->setEndValue(QPoint(0, end));
                        animation = TTKObjectCast(QPropertyAnimation*, m_leaverAnimation->animationAt(1));
    animation->setStartValue(QPoint(0, ctrlst));
    animation->setEndValue(QPoint(0, ctrlend));
    m_leaverAnimation->start();
}
