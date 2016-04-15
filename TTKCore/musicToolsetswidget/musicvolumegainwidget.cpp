#include "musicvolumegainwidget.h"
#include "ui_musicvolumegainwidget.h"
#include "musicbackgroundmanager.h"
#include "musicsemaphoreeventloop.h"
#include "musicmessagebox.h"
#include "musicuiobject.h"

#include <QDebug>
#include <QProcess>
#include <QFileDialog>

#define TRACKDB "Recommended \"Track\" dB change:"
#define ALBUMDB "Recommended \"Album\" dB change for all files:"

MusicVolumeGainWidget::MusicVolumeGainWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicVolumeGainWidget), m_process(nullptr)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->addFileButton->setIcon(QIcon(":/toolSets/gainAddFile"));
    ui->addFileButton->setIconSize(QSize(40, 40));
    ui->addFileButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->addFileButton->setToolTip(tr("addFile"));
    ui->addFileButton->setCursor(QCursor(Qt::PointingHandCursor));

    ui->addFilesButton->setIcon(QIcon(":/toolSets/gainAddFiles"));
    ui->addFilesButton->setIconSize(QSize(40, 40));
    ui->addFilesButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->addFilesButton->setToolTip(tr("addFiles"));
    ui->addFilesButton->setCursor(QCursor(Qt::PointingHandCursor));

    ui->rmFileButton->setIcon(QIcon(":/toolSets/gainRmFile"));
    ui->rmFileButton->setIconSize(QSize(40, 40));
    ui->rmFileButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->rmFileButton->setToolTip(tr("rmFile"));
    ui->rmFileButton->setCursor(QCursor(Qt::PointingHandCursor));

    ui->rmFilesButton->setIcon(QIcon(":/toolSets/gainRmFiles"));
    ui->rmFilesButton->setIconSize(QSize(40, 40));
    ui->rmFilesButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->rmFilesButton->setToolTip(tr("rmFiles"));
    ui->rmFilesButton->setCursor(QCursor(Qt::PointingHandCursor));

    ui->analysisButton->setIcon(QIcon(":/toolSets/analysis"));
    ui->analysisButton->setIconSize(QSize(40, 40));
    ui->analysisButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->analysisButton->setToolTip(tr("analysis"));
    ui->analysisButton->setCursor(QCursor(Qt::PointingHandCursor));

    ui->applyButton->setIcon(QIcon(":/toolSets/apply"));
    ui->applyButton->setIconSize(QSize(50, 50));
    ui->applyButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->applyButton->setToolTip(tr("apply"));
    ui->applyButton->setCursor(QCursor(Qt::PointingHandCursor));

    ui->volumeLineEdit->setValidator(new QRegExpValidator(QRegExp("-?[0-9]+$"), this));

    m_currentIndex = -1;
    m_process = new QProcess(this);
    m_process->setProcessChannelMode(QProcess::MergedChannels);
    connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(readyReadStandardOutput()));

    connect(ui->addFileButton, SIGNAL(clicked()), SLOT(addFileButtonClicked()));
    connect(ui->addFilesButton, SIGNAL(clicked()), SLOT(addFilesButtonClicked()));
    connect(ui->rmFileButton, SIGNAL(clicked()), SLOT(rmFileButtonClicked()));
    connect(ui->rmFilesButton, SIGNAL(clicked()), SLOT(rmFilesButtonClicked()));

}

MusicVolumeGainWidget::~MusicVolumeGainWidget()
{
    delete m_process;
    delete ui;
}

void MusicVolumeGainWidget::createItemFinished(const QString &track, const QString &album)
{
    if(m_currentIndex >= m_paths.count())
    {
        return;
    }

    qDebug() << m_paths[m_currentIndex];
    qDebug() << track.toDouble() << album.toDouble();
    qDebug() << (track.toDouble() - ui->volumeLineEdit->text().toDouble());
    qDebug() << (album.toDouble() - ui->volumeLineEdit->text().toDouble());
}

void MusicVolumeGainWidget::addFileButtonClicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles );
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setNameFilters( QStringList() << "All File(*.*)" << "MP3 File(*.mp3)" );
    if(dialog.exec())
    {
        int orcount = m_paths.count();
        m_paths << dialog.selectedFiles();
        for(int i=orcount; i<m_paths.count(); ++i)
        {
            m_currentIndex = i;
            qDebug() << "Ddsdf";
//            MusicSemaphoreEventLoop loop;
//            connect(this, SIGNAL(createItemFinished()), &loop, SLOT(quit()));
            m_process->start(MAKE_GAIN_AL, QStringList() << m_paths[i]);
//            loop.exec();
            qDebug() << "Dd";
        }
    }
}

void MusicVolumeGainWidget::addFilesButtonClicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory );
    dialog.setViewMode(QFileDialog::Detail);
    if(dialog.exec())
    {
        QList<QFileInfo> file(dialog.directory().entryInfoList());
        foreach(QFileInfo info, file)
        {
            if( QString("mp3").contains(info.suffix().toLower()) )
            {
                m_paths << info.absoluteFilePath();
            }
        }
    }
}

void MusicVolumeGainWidget::rmFileButtonClicked()
{

}

void MusicVolumeGainWidget::rmFilesButtonClicked()
{

}

void MusicVolumeGainWidget::readyReadStandardOutput()
{
    QString track, album;
    while(m_process->canReadLine())
    {
        QByteArray data = m_process->readLine();
        if(data.contains( TRACKDB ))
        {
            data.replace(TRACKDB, QByteArray());
            data.replace("\r\n", QByteArray());
            track = QString(data.trimmed());
        }
        if(data.contains( ALBUMDB ))
        {
            data.replace(ALBUMDB, QByteArray());
            data.replace("\r\n", QByteArray());
            album = QString(data.trimmed());
        }
    }

    if(!track.isEmpty() || !album.isEmpty())
    {
        createItemFinished(track, album);
    }
}

int MusicVolumeGainWidget::exec()
{
    if(!QFile::exists(MAKE_GAIN_AL))
    {
        MusicMessageBox message;
        message.setText(tr("Lack of plugin file!"));
        message.exec();
        return -1;
    }

    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}

