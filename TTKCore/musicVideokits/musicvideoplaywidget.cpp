#include "musicvideoplaywidget.h"
#include "musicvideoview.h"
#include "musicvideotablewidget.h"
#include "musiclocalsongsearchedit.h"
#include "musicvideofloatwidget.h"
#include "musicsongsharingwidget.h"
#include "musicobject.h"

#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#ifdef Q_OS_UNIX
#include <QDesktopWidget>
#endif

#define WINDOW_HEIGHT   460
#define WINDOW_WIDTH    635


MusicVideoPlayWidget::MusicVideoPlayWidget(bool popup, QWidget *parent)
    : MusicAbstractMoveWidget(parent), m_closeButton(nullptr)
{
    if(popup)
    {
        resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    }

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    m_topWidget = new QWidget(this);
    m_topWidget->setObjectName("#topWidget");
    m_topWidget->setStyleSheet(MusicUIObject::MCustomStyle19 + \
                               MusicUIObject::MCustomStyle20);
    QHBoxLayout *topLayout = new QHBoxLayout(m_topWidget);
    topLayout->setSpacing(0);

    m_textLabel = new QLabel(m_topWidget);
    m_textLabel->setStyleSheet(MusicUIObject::MCustomStyle11);
    m_searchEdit = new MusicLocalSongSearchEdit(m_topWidget);
    m_searchEdit->setFixedHeight(18);
    m_searchButton = new QPushButton(m_topWidget);
    m_searchButton->setIcon(QIcon(":/share/searchlineleft"));
    m_searchButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_searchButton->setIconSize(QSize(18, 18));
    topLayout->addStretch();
    topLayout->addWidget(m_textLabel);
    topLayout->addStretch();
    topLayout->addWidget(m_searchEdit);
    topLayout->addWidget(m_searchButton);
    m_searchEdit->hide();
    m_searchButton->hide();

    if(popup)
    {
        m_closeButton = new QPushButton(this);
        m_closeButton->setToolTip(tr("Close"));
        m_closeButton->setIcon(QIcon(":/image/close"));
        m_closeButton->setCursor(QCursor(Qt::PointingHandCursor));
        m_closeButton->setIconSize(QSize(18, 18));
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
        connect(m_closeButton, SIGNAL(clicked()), object, SLOT(deleteVideoWidget()));
    }
}

QString MusicVideoPlayWidget::getSearchText() const
{
    return m_searchEdit->text().trimmed();
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
    if(desktopWidget && desktopWidget->screen())
    {
        s = desktopWidget->screen()->size();
    }
    else
    {
        s = QSize(WINDOW_WIDTH, WINDOW_HEIGHT);
        showNormal();
    }
#endif
    m_videoView->resizeWindow(resize, s);
    m_videoTable->resizeWindow(s.width()*1.0 / WINDOW_WIDTH);
    m_videoFloatWidget->resizeWindow(resize ?  s.width() - WINDOW_WIDTH : 0,
                                     resize ? (s.height() - WINDOW_HEIGHT)/2 : 0);
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
    m_textLabel->setText(MusicUtils::elidedText(font(), name, Qt::ElideRight, 275));
    mvURLChanged(data);
}
