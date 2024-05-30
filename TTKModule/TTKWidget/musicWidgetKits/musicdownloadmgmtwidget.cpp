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
    m_ui->openDirButton->setStyleSheet(TTK::UI::PushButtonStyle03);

#ifdef Q_OS_UNIX
    m_ui->downloadButton->setFocusPolicy(Qt::NoFocus);
    m_ui->openDirButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->downloadButton, SIGNAL(clicked()), SLOT(restartToRequest()));
    connect(m_ui->openDirButton, SIGNAL(clicked()), SLOT(openFileLocation()));

    TTK::Widget::adjustWidgetPosition(this);
}

MusicDownloadResetWidget::~MusicDownloadResetWidget()
{
    delete m_ui;
}

void MusicDownloadResetWidget::initialize(const QString &name)
{
    m_currentName = name;
}

void MusicDownloadResetWidget::restartToRequest()
{
    MusicDownloadWidget *widget = new MusicDownloadWidget(m_parent);
    widget->initialize(m_currentName);
    widget->show();

    close();
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

void MusicDownloadMgmtWidget::initialize(const QString &name)
{
    bool exist = false;
    MusicApplication::instance()->containsDownloadItem(exist);

    if(exist)
    {
        MusicDownloadResetWidget *resetWidget = new MusicDownloadResetWidget(m_parent);
        resetWidget->initialize(name);
        resetWidget->show();
        return;
    }

    MusicDownloadWidget *widget = new MusicDownloadWidget(m_parent);
    if(parent()->metaObject()->indexOfSlot("downloadSongFinished()") != -1)
    {
        connect(widget, SIGNAL(dataDownloadChanged()), parent(), SLOT(downloadSongFinished()));
    }
    widget->initialize(name);
    widget->show();
}
