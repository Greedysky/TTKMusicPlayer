#ifndef TTKMUSICLYRICMODEL_H
#define TTKMUSICLYRICMODEL_H

#include <QAbstractListModel>

class MusicLrcAnalysis;

class TTKMusicLyricModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TTKMusicLyricModel(QObject *parent = 0);
    ~TTKMusicLyricModel();

    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void loadCurrentSongLrc(const QString &path);


signals:

public slots:

protected:
    QStringList m_datas;
    MusicLrcAnalysis *m_lrcAnalysis;

};

#endif // TTKMUSICLYRICMODEL_H
