#include "ui_musicdownloadresetwidget.h"
#include "musicdownloadmgmtwidget.h"
#include "musicdownloadwidget.h"
#include "musicsettingmanager.h"
#include "musicleftareawidget.h"
#include "musicapplication.h"
#include "musicurlutils.h"

MusicDownloadResetWidget::MusicDownloadResetWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicDownloadResetWidget)
{
    m_ui->setupUi(this);

    m_parentClass = parent;

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    setAttribute(Qt::WA_DeleteOnClose);

    m_ui->downloadButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    m_ui->openDetailButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    m_ui->openDirButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);

#ifdef Q_OS_UNIX
    m_ui->downloadButton->setFocusPolicy(Qt::NoFocus);
    m_ui->openDetailButton->setFocusPolicy(Qt::NoFocus);
    m_ui->openDirButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->downloadButton, SIGNAL(clicked()), SLOT(restartToDownload()));
    connect(m_ui->openDetailButton, SIGNAL(clicked()), SLOT(openDetailInfo()));
    connect(m_ui->openDirButton, SIGNAL(clicked()), SLOT(openFileLocation()));
    connect(this, SIGNAL(openStackedDownloadWidget()), MusicLeftAreaWidget::instance(), SLOT(musicStackedMyDownWidgetChanged()));
}

MusicDownloadResetWidget::~MusicDownloadResetWidget()
{
    delete m_ui;
}

void MusicDownloadResetWidget::setSongName(const QString &name)
{
    m_currentName = name;
}

void MusicDownloadResetWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveWidget::show();
}

void MusicDownloadResetWidget::restartToDownload()
{
    MusicDownloadWidget *download = new MusicDownloadWidget(m_parentClass);
    download->setSongName(m_currentName, MusicDownLoadQueryThreadAbstract::MusicQuery);
    download->show();

    close();
}

void MusicDownloadResetWidget::openDetailInfo()
{
    close();
    emit openStackedDownloadWidget();
}

void MusicDownloadResetWidget::openFileLocation()
{
    bool exist = false;
    const QString &path = MusicApplication::instance()->musicDownloadContains(exist);
    if(exist)
    {
        MusicUtils::Url::openUrl( path, true );
    }
    close();
}


MusicDownloadMgmtWidget::MusicDownloadMgmtWidget(QObject *parent)
    : QObject(parent)
{
    m_parentClass = MStatic_cast(QWidget*, parent);
}

void MusicDownloadMgmtWidget::setSongName(const QString &name, MusicDownLoadQueryThreadAbstract::QueryType type)
{
    if(type == MusicDownLoadQueryThreadAbstract::MusicQuery)
    {
        bool exist = false;
        MusicApplication::instance()->musicDownloadContains(exist);
        if(exist)
        {
            MusicDownloadResetWidget *resetWidget = new MusicDownloadResetWidget(m_parentClass);
            resetWidget->setSongName(name);
            resetWidget->show();
            return;
        }
    }

    MusicDownloadWidget *download = new MusicDownloadWidget(m_parentClass);
    if(parent()->metaObject()->indexOfSlot("musicDownloadSongFinished()") != -1)
    {
        connect(download, SIGNAL(dataDownloadChanged()), parent(), SLOT(musicDownloadSongFinished()));
    }
    download->setSongName(name, type);
    download->show();
}
