#include "musicvideoplaywidget.h"
#include "musicvideoview.h"
#include "musicvideotablewidget.h"
#include "musiclocalsongsearchedit.h"
#include "musicvideofloatwidget.h"
#include "musicsongsharingwidget.h"
#include "musicobject.h"
#include "musicttkuiobject.h"
#include "musicsettingmanager.h"

#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#ifdef Q_OS_UNIX
#include <QDesktopWidget>
#endif

#define WINDOW_HEIGHT   503
#define WINDOW_WIDTH    661


MusicVideoPlayWidget::MusicVideoPlayWidget(bool popup, QWidget *parent)
    : MusicAbstractMoveWidget(parent), m_closeButton(nullptr)
{
    if(popup)
    {
        resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    }

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    m_topWidget = new QWidget(this);
    m_topWidget->setObjectName("#topWidget");
    m_topWidget->setStyleSheet(MusicUIObject::MCustomStyle19 + \
                               MusicUIObject::MCustomStyle20);
    QHBoxLayout *topLayout = new QHBoxLayout(m_topWidget);
    topLayout->setSpacing(0);

    m_textLabel = new QLabel(m_topWidget);
    m_textLabel->setStyleSheet(MusicUIObject::MCustomStyle11);

    QWidget *searchWidget = new QWidget(m_topWidget);
    searchWidget->setStyleSheet("background:rgba(255,255,255,125)");
    QHBoxLayout *searchLayout = new QHBoxLayout(searchWidget);
    searchLayout->setContentsMargins(0, 0, 0, 0);
    searchLayout->setSpacing(0);
    m_searchEdit = new MusicLocalSongSearchEdit(searchWidget);
    m_searchEdit->setFixedHeight(18);
    m_searchButton = new QPushButton(searchWidget);
    m_searchButton->setIcon(QIcon(":/tiny/btn_search_main_hover"));
    m_searchButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_searchButton->setIconSize(QSize(18, 18));
    searchLayout->addWidget(m_searchEdit);
    searchLayout->addWidget(m_searchButton);
    searchWidget->setLayout(searchLayout);

    topLayout->addStretch();
    topLayout->addWidget(m_textLabel);
    topLayout->addStretch();
    topLayout->addWidget(searchWidget);
    m_searchEdit->hide();
    m_searchButton->hide();

    if(popup)
    {
        m_closeButton = new QPushButton(this);
        m_closeButton->setToolTip(tr("Close"));
        m_closeButton->setFixedSize(14, 14);
        m_closeButton->setStyleSheet(MusicTTKUIObject::MKGBtnPClose);
        m_closeButton->setCursor(QCursor(Qt::PointingHandCursor));
        topLayout->addWidget(m_closeButton);
    }
    m_topWidget->setLayout(topLayout);
    m_topWidget->setFixedHeight(35);

    m_stackedWidget = new QStackedWidget(this);
    m_stackedWidget->setStyleSheet(MusicUIObject::MCustomStyle20);
    layout->addWidget(m_topWidget);
    layout->addWidget(m_stackedWidget);
    setLayout(layout);

    m_videoFloatWidget = new MusicVideoFloatWidget(this);
    m_videoTable = new MusicVideoTableWidget(this);
    m_videoView = new MusicVideoView(popup, this);
    m_stackedWidget->addWidget(m_videoView);
    m_stackedWidget->addWidget(m_videoTable);
    m_stackedWidget->setCurrentIndex(0);
    m_videoFloatWidget->setText(MusicVideoFloatWidget::FreshType,
                                popup ? " " + tr("InlineMode") : " " + tr("PopupMode"));

    connect(m_searchButton,SIGNAL(clicked(bool)), SLOT(searchButtonClicked()));
    connect(m_videoTable, SIGNAL(mvURLNameChanged(QString,QString)),
                          SLOT(mvURLNameChanged(QString,QString)));
    connect(m_videoTable, SIGNAL(restartSearchQuery(QString)),
                          SLOT(videoResearchButtonSearched(QString)));
    connect(m_searchEdit, SIGNAL(enterFinished(QString)), SLOT(videoResearchButtonSearched(QString)));

    connect(m_videoFloatWidget, SIGNAL(searchButtonClicked()), SLOT(switchToSearchTable()));
    connect(m_videoFloatWidget, SIGNAL(freshButtonClicked()), SLOT(freshButtonClicked()));
    connect(m_videoFloatWidget, SIGNAL(fullscreenButtonClicked()), SLOT(fullscreenButtonClicked()));
    connect(m_videoFloatWidget, SIGNAL(downloadButtonClicked()), SLOT(downloadButtonClicked()));
    connect(m_videoFloatWidget, SIGNAL(shareButtonClicked()), SLOT(shareButtonClicked()));

}

MusicVideoPlayWidget::~MusicVideoPlayWidget()
{
    delete m_closeButton;
    delete m_textLabel;
    delete m_searchEdit;
    delete m_searchButton;
    delete m_topWidget;
    delete m_videoView;
    delete m_stackedWidget;
}

QString MusicVideoPlayWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicVideoPlayWidget::setObjectToClose(QObject *object)
{
    if(m_closeButton)
    {
        connect(m_closeButton, SIGNAL(clicked()), object, SLOT(deleteStackedFuncWidget()));
    }
}

QString MusicVideoPlayWidget::getSearchText() const
{
    return m_searchEdit->text().trimmed();
}

void MusicVideoPlayWidget::resizeEvent(QResizeEvent *event)
{
    MusicAbstractMoveWidget::resizeEvent(event);
    int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    int height = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().height();
    if(!isFullScreen())
    {
        resizeWindow(width - WINDOW_WIDTH_MIN, height - WINDOW_HEIGHT_MIN);
    }
}

void MusicVideoPlayWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}

void MusicVideoPlayWidget::resizeWindow(bool resize)
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
    QDesktopWidget* desktopWidget = QApplication::desktop();
    if(isFullScreen() && desktopWidget && desktopWidget->screen())
    {
        s = desktopWidget->screen()->size();
    }
    else
    {
        s = QSize(WINDOW_WIDTH, WINDOW_HEIGHT);
        showNormal();
    }
#endif
    resizeWindow(s.width() - WINDOW_WIDTH, s.height() - WINDOW_HEIGHT);
}

void MusicVideoPlayWidget::resizeWindow(int width, int height)
{
    m_videoView->resizeWindow(width, height);
    m_videoTable->resizeWindow(width);
    m_videoFloatWidget->resizeWindow(width, height);
}

void MusicVideoPlayWidget::switchToSearchTable()
{
    m_searchEdit->show();
    m_searchButton->show();
    m_stackedWidget->setCurrentIndex(1);
}

void MusicVideoPlayWidget::freshButtonClicked()
{
    QString text = m_videoFloatWidget->getText(MusicVideoFloatWidget::FreshType);
    emit freshButtonClicked( text == tr("PopupMode"));
}

void MusicVideoPlayWidget::fullscreenButtonClicked()
{
    if(m_videoFloatWidget->getText(MusicVideoFloatWidget::FreshType) == tr("PopupMode"))
    {
        return;
    }

    m_videoView->setFullScreen();
    QString text = m_videoFloatWidget->getText(MusicVideoFloatWidget::FullscreenType) ==
                                tr("NormalMode") ? tr("FullScreenMode") : tr("NormalMode");
    m_videoFloatWidget->setText(MusicVideoFloatWidget::FullscreenType, " " + text);
}

void MusicVideoPlayWidget::downloadButtonClicked()
{
    m_videoTable->downloadLocalFromControl();
}

void MusicVideoPlayWidget::shareButtonClicked()
{
    QString text = m_textLabel->text().trimmed();
    if(text.isEmpty())
    {
        return;
    }

    MusicSongSharingWidget shareWidget(this);
    shareWidget.setSongName(text);
    shareWidget.exec();
}

void MusicVideoPlayWidget::searchButtonClicked()
{
    videoResearchButtonSearched( getSearchText() );
}

void MusicVideoPlayWidget::videoResearchButtonSearched(const QString &name)
{
    switchToSearchTable();
    m_searchEdit->setText(name);
    m_videoTable->startSearchQuery(name);
}

void MusicVideoPlayWidget::mvURLChanged(const QString &data)
{
    m_videoView->setMedia(data);
    m_videoView->play();
    m_searchEdit->hide();
    m_searchButton->hide();
    m_stackedWidget->setCurrentIndex(0);
}

void MusicVideoPlayWidget::mvURLNameChanged(const QString &name, const QString &data)
{
    m_textLabel->setText(MusicUtils::UWidget::elidedText(font(), name, Qt::ElideRight, 275));
    mvURLChanged(data);
}
