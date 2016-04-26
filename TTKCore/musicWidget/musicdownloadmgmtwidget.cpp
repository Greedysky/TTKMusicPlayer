#include "ui_musicdownloadresetwidget.h"
#include "musicdownloadmgmtwidget.h"
#include "musicconnectionpool.h"
#include "musicdownloadwidget.h"
#include "musicsettingmanager.h"
#include "musicbackgroundmanager.h"
#include "musicutils.h"

MusicDownloadResetWidget::MusicDownloadResetWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      ui(new Ui::MusicDownloadResetWidget)
{
    ui->setupUi(this);

    m_parentClass = parent;

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);

    ui->downloadButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->openDetailButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->openDirButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);

    connect(ui->downloadButton, SIGNAL(clicked()), SLOT(restartToDownload()));
    connect(ui->openDetailButton, SIGNAL(clicked()), SLOT(openDetailInfo()));
    connect(ui->openDirButton, SIGNAL(clicked()), SLOT(openFileLocation()));

    M_CONNECTION->setValue("MusicDownloadResetWidget", this);
    M_CONNECTION->poolConnect("MusicDownloadResetWidget", "MusicLeftAreaWidget");
}

MusicDownloadResetWidget::~MusicDownloadResetWidget()
{
    M_CONNECTION->poolDisConnect("MusicDownloadResetWidget");
    delete ui;
}

void MusicDownloadResetWidget::setSongName(const QString &name)
{
    m_currentName = name;
}

void MusicDownloadResetWidget::show()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
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
    MusicUtils::openUrl(M_SETTING->value(MusicSettingManager::DownloadMusicExistPathChoiced).toString());
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
        bool exist = M_SETTING->value(MusicSettingManager::DownloadMusicExistChoiced).toBool();
        if(exist)
        {
            MusicDownloadResetWidget *resetWidget = new MusicDownloadResetWidget(m_parentClass);
            resetWidget->setSongName(name);
            resetWidget->show();
            return;
        }
    }
    MusicDownloadWidget *download = new MusicDownloadWidget(m_parentClass);
    if(parent()->metaObject()->indexOfMethod("musicDownloadSongFinished()") != -1)
    {
        connect(download, SIGNAL(dataDownloadChanged()), parent(),
                          SLOT(musicDownloadSongFinished()));
    }
    download->setSongName(name, type);
    download->show();
}
