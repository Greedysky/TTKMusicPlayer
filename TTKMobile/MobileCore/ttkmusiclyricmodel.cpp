#include "ttkmusiclyricmodel.h"
#include "musiclrcanalysis.h"
#include "musicnumberdefine.h"

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

void TTKMusicLyricModel::clear()
{
    m_datas.clear();
    m_datas << tr("No Lrc Found");
}
