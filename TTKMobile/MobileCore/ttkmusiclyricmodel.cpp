#include "ttkmusiclyricmodel.h"
#include "musiclrcanalysis.h"
#include "musicnumberdefine.h"
#include "musicstringutils.h"
#include "musicsettingmanager.h"

#include <QColor>

TTKMusicLyricModel::TTKMusicLyricModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_lrcAnalysis = new MusicLrcAnalysis(this);
    clear();
}

TTKMusicLyricModel::~TTKMusicLyricModel()
{
    delete m_lrcAnalysis;
}

int TTKMusicLyricModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_datas.count();
}

QVariant TTKMusicLyricModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= m_datas.count())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        return m_datas[index.row()];
    }
    else
    {
        return QVariant();
    }
}

QHash<int, QByteArray> TTKMusicLyricModel::roleNames() const
{
    QHash<int, QByteArray> data;
    data[Qt::DisplayRole] = "textLine";
    return data;
}

QVariant TTKMusicLyricModel::value(int row) const
{
    return data( index(row) );
}

void TTKMusicLyricModel::loadCurrentSongLrc(const QString &path)
{
    if(m_lrcAnalysis->transLrcFileToTime(path) == MusicLrcAnalysis::OpenFileSuccess)
    {
        QStringList d = m_lrcAnalysis->getAllLrcs().split("\n");
        beginInsertRows(QModelIndex(), 0, d.count());
        m_datas.clear();
        m_datas << QString() << d;
        endInsertRows();
        m_lrcAnalysis->setCurrentIndex(0);
    }
}

void TTKMusicLyricModel::findText(qint64 position)
{
    if(m_lrcAnalysis->isEmpty())
    {
        return;
    }

    int index = m_lrcAnalysis->getCurrentIndex();
    qint64 time = m_lrcAnalysis->findTime(index);
    if(time < position && time != -1)
    {
        m_lrcAnalysis->setCurrentIndex(++index);
        emit currentIndexChanged(index);
    }
}

void TTKMusicLyricModel::setSongSpeedAndSlow(qint64 position)
{
    m_lrcAnalysis->setSongSpeedAndSlow(position);
    emit currentIndexChanged(m_lrcAnalysis->getCurrentIndex());
}

void TTKMusicLyricModel::setCenterColor(int index, const QString &color)
{
    M_SETTING_PTR->setValue(MusicSettingManager::LrcColorChoiced, index);
    M_SETTING_PTR->setValue(MusicSettingManager::LrcFgColorChoiced,
                            MusicUtils::String::writeColorConfig(QColor(color)));
    initParameter();
}

void TTKMusicLyricModel::setCenterSize(int index, int size)
{
    M_SETTING_PTR->setValue(MusicSettingManager::LrcTypeChoiced, index);
    M_SETTING_PTR->setValue(MusicSettingManager::LrcSizeChoiced, size);
    initParameter();
}

void TTKMusicLyricModel::initParameter()
{
    QString colorStr = M_SETTING_PTR->value(MusicSettingManager::LrcFgColorChoiced).toString();
    QList<QColor> colors(MusicUtils::String::readColorConfig(colorStr));
    QColor color("#00FF00");
    if(!colors.isEmpty())
    {
        color = colors.first();
    }
    int size = M_SETTING_PTR->value(MusicSettingManager::LrcSizeChoiced).toInt();

    int cIndex = M_SETTING_PTR->value(MusicSettingManager::LrcColorChoiced).toInt();
    int sIndex = M_SETTING_PTR->value(MusicSettingManager::LrcTypeChoiced).toInt();

    emit currentParameterIndexChanged(cIndex, sIndex);
    emit currentParameterChanged(color, size);
}

void TTKMusicLyricModel::clear()
{
    m_datas.clear();
    m_datas << tr("No Lrc Found");
}
