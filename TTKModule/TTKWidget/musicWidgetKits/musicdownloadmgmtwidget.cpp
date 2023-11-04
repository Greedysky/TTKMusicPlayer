#include "ui_musicdownloadresetwidget.h"
#include "musicdownloadmgmtwidget.h"
#include "musicdownloadwidget.h"
#include "musicleftareawidget.h"
#include "musicapplication.h"
#include "musicurlutils.h"

MusicDownloadResetWidget::MusicDownloadResetWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicDownloadResetWidget),
      m_parent(parent)
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

    m_ui->downloadButton->setStyleSheet(TTK::UI::PushButtonStyle03);
    m_ui->openDetailButton->setStyleSheet(TTK::UI::PushButtonStyle03);
    m_ui->openDirButton->setStyleSheet(TTK::UI::PushButtonStyle03);

#ifdef Q_OS_UNIX
    m_ui->downloadButton->setFocusPolicy(Qt::NoFocus);
    m_ui->openDetailButton->setFocusPolicy(Qt::NoFocus);
    m_ui->openDirButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->downloadButton, SIGNAL(clicked()), SLOT(restartRequest()));
    connect(m_ui->openDetailButton, SIGNAL(clicked()), SLOT(openDetailInfo()));
    connect(m_ui->openDirButton, SIGNAL(clicked()), SLOT(openFileLocation()));
    connect(this, SIGNAL(openStackedDownloadWidget()), MusicLeftAreaWidget::instance(), SLOT(stackedMyDownWidgetChanged()));
}

MusicDownloadResetWidget::~MusicDownloadResetWidget()
{
    delete m_ui;
}

void MusicDownloadResetWidget::setSongName(const QString &name)
{
    m_currentName = name;
}

void MusicDownloadResetWidget::restartRequest()
{
    MusicDownloadWidget *download = new MusicDownloadWidget(m_parent);
    download->setSongName(m_currentName, MusicAbstractQueryRequest::QueryType::Music);
    download->show();

    close();
}

void MusicDownloadResetWidget::openDetailInfo()
{
    close();
    Q_EMIT openStackedDownloadWidget();
}

void MusicDownloadResetWidget::openFileLocation()
{
    bool exist = false;
    const QString &path = MusicApplication::instance()->containsDownloadItem(exist);
    if(exist)
    {
        TTK::Url::openUrl(path);
    }
    close();
}


MusicDownloadMgmtWidget::MusicDownloadMgmtWidget(QObject *parent)
    : QObject(parent),
      m_parent(TTKObjectCast(QWidget*, parent))
{

}

void MusicDownloadMgmtWidget::setSongName(const QString &name, MusicAbstractQueryRequest::QueryType type)
{
    if(type == MusicAbstractQueryRequest::QueryType::Music)
    {
        bool exist = false;
        MusicApplication::instance()->containsDownloadItem(exist);
        if(exist)
        {
            MusicDownloadResetWidget *resetWidget = new MusicDownloadResetWidget(m_parent);
            resetWidget->setSongName(name);
            resetWidget->show();
            return;
        }
    }

    MusicDownloadWidget *download = new MusicDownloadWidget(m_parent);
    if(parent()->metaObject()->indexOfSlot("downloadSongFinished()") != -1)
    {
        connect(download, SIGNAL(dataDownloadChanged()), parent(), SLOT(downloadSongFinished()));
    }
    download->setSongName(name, type);
    download->show();
}
