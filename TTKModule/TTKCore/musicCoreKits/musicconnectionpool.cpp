#include "musicconnectionpool.h"
#include "musicplayer.h"
#include "musiclrcmakerwidget.h"
#include "musicdownloadstatusmodule.h"
#include "musicequalizerdialog.h"
#include "musiclrclocallinkwidget.h"
#include "musicvideoqualitypopwidget.h"
#include "musicvideosearchtablewidget.h"
#include "musiccloudtablewidget.h"
#include "musiccloudmanagerwidget.h"

void MusicConnectionPool::removeValue(const QObject *object)
{
    for(auto it = m_parameters.constBegin(); it != m_parameters.constEnd(); ++it)
    {
        if(it.value() == object)
        {
            m_parameters.take(it.key());
            break;
        }
    }
}

void MusicConnectionPool::connect(const QString &from, const QString &to)
{
    const QObject *first = m_parameters.value(from);
    const QObject *second = m_parameters.value(to);
    if(first == nullptr || second == nullptr)
    {
        return;
    }

    if(from == MusicPlayer::className() && to == MusicLrcMakerWidget::className())
    {
        QObject::connect(first, SIGNAL(positionChanged(qint64)), second, SLOT(positionChanged(qint64)));
        QObject::connect(first, SIGNAL(durationChanged(qint64)), second, SLOT(durationChanged(qint64)));
    }
    else if(from == MusicEqualizerDialog::className() && to == MusicPlayer::className())
    {
        QObject::connect(first, SIGNAL(setEnabledEffect(bool)), second, SLOT(setEnabledEffect(bool)));
        QObject::connect(first, SIGNAL(setEqualizerEffect(TTKIntList)), second, SLOT(setEqualizerEffect(TTKIntList)));
    }
    else if(from == MusicLrcLocalLinkWidget::className() && to == MusicDownloadStatusModule::className())
    {
        QObject::connect(first, SIGNAL(currentLrcChanged(QString)), second, SLOT(showDownLoadInfoFinished(QString)));
    }
    else if(from == MusicVideoQualityPopWidget::className() && to == MusicVideoSearchTableWidget::className())
    {
        QObject::connect(first, SIGNAL(queryMediaProps(TTK::MusicSongPropertyList&)), second, SLOT(queryMediaProps(TTK::MusicSongPropertyList&)));
    }
    else if((from == MusicCloudManagerTableWidget::className() && to == MusicCloudUploadTableWidget::className()))
    {
        QObject::connect(first, SIGNAL(uploadFileError(MusicCloudDataItem)), second, SLOT(uploadFileError(MusicCloudDataItem)));
        QObject::connect(second, SIGNAL(reuploadFilesToServer(QStringList)), first, SLOT(reuploadFilesToServer(QStringList)));
    }
}
