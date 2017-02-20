#include "ui_musicdownloadresetwidget.h"
#include "musicdownloadmgmtwidget.h"
#include "musicdownloadwidget.h"
#include "musicsettingmanager.h"
#include "musicleftareawidget.h"
#include "musicapplication.h"
#include "musiccoreutils.h"

MusicDownloadResetWidget::MusicDownloadResetWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      ui(new Ui::MusicDownloadResetWidget)
{
    ui->setupUi(this);

    m_parentClass = parent;

    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    setAttribute(Qt::WA_DeleteOnClose);

    ui->downloadButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    ui->openDetailButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    ui->openDirButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);

    connect(ui->downloadButton, SIGNAL(clicked()), SLOT(restartToDownload()));
    connect(ui->openDetailButton, SIGNAL(clicked()), SLOT(openDetailInfo()));
    connect(ui->openDirButton, SIGNAL(clicked()), SLOT(openFileLocation()));
    connect(this, SIGNAL(openStackedDownloadWidget()), MusicLeftAreaWidget::instance(),
                  SLOT(musicStackedMyDownWidgetChanged()));
}

MusicDownloadResetWidget::~MusicDownloadResetWidget()
{
    delete ui;
}

QString MusicDownloadResetWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownloadResetWidget::setSongName(const QString &name)
{
    m_currentName = name;
}

void MusicDownloadResetWidget::show()
{
    setBackgroundPixmap(ui->background, size());
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
    QString path = MusicApplication::instance()->musicDownloadContains(exist);
    if(exist)
    {
        MusicUtils::Core::openUrl( path, true );
    }
    close();
}


MusicDownloadMgmtWidget::MusicDownloadMgmtWidget(QObject *parent)
    : QObject(parent)
{
    m_parentClass = MStatic_cast(QWidget*, parent);
}

QString MusicDownloadMgmtWidget::getClassName()
{
    return staticMetaObject.className();
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
