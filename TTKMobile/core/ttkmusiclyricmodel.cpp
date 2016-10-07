#include "ttkmusiclyricmodel.h"
#include "musiclrcanalysis.h"
#include<QDebug>
TTKMusicLyricModel::TTKMusicLyricModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_lrcAnalysis = new MusicLrcAnalysis(this);
}

TTKMusicLyricModel::~TTKMusicLyricModel()
{
    delete m_lrcAnalysis;
}

int TTKMusicLyricModel::rowCount(const QModelIndex & parent) const
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

void TTKMusicLyricModel::loadCurrentSongLrc(const QString &path)
{
    if(m_lrcAnalysis->transLrcFileToTime(path) == MusicLrcAnalysis::OpenFileSuccess)
    {
        m_datas = m_lrcAnalysis->getAllLrcs().split("\n");
    }
}
