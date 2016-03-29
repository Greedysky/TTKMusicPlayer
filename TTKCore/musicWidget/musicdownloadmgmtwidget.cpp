#include "ui_musicdownloadresetwidget.h"
#include "musicdownloadmgmtwidget.h"
#include "musicdownloadwidget.h"
#include "musicsettingmanager.h"

MusicDownloadResetWidget::MusicDownloadResetWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      ui(new Ui::MusicDownloadResetWidget)
{
    ui->setupUi(this);
    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));

    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);

}

MusicDownloadResetWidget::~MusicDownloadResetWidget()
{
    delete ui;
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
            resetWidget->show();
            return;
        }
    }
    MusicDownloadWidget *download = new MusicDownloadWidget(m_parentClass);
    download->setSongName(name, type);
    download->show();
}
