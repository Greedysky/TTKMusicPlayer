#ifndef TTKMUSICLYRICMODEL_H
#define TTKMUSICLYRICMODEL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QAbstractListModel>
#include "musicmobileglobaldefine.h"

class MusicLrcAnalysis;

/*! @brief The class of the music lrc model
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_MOBILE_EXPORT TTKMusicLyricModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TTKMusicLyricModel(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~TTKMusicLyricModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    /*!
     * Get current rowcount.
     */
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    /*!
     * Get current row data.
     */
    virtual QHash<int, QByteArray> roleNames() const override;
    /*!
     * Get current row names.
     */

    Q_INVOKABLE QVariant value(int row) const;
   /*!
    * Get current row data.
    */
    Q_INVOKABLE void loadCurrentSongLrc(const QString &path);
    /*!
     * Load local lrc file to analysis.
     */
    Q_INVOKABLE void findText(qint64 position);
    /*!
     * Find current lrc index by pos time.
     */
    Q_INVOKABLE void setSongSpeedAndSlow(qint64 position);
    /*!
     * Set song speed and slow by given time, return new time.
     */

    Q_INVOKABLE void setCenterColor(int index, const QString &color);
    /*!
     * Set center color.
     */
    Q_INVOKABLE void setCenterSize(int index, int size);
    /*!
     * Set center size.
     */
    Q_INVOKABLE void initParameter();
    /*!
     * Init the first setting parameter from configure.
     */

Q_SIGNALS:
    void currentIndexChanged(int index);
    /*!
     * Current play index changed emit.
     */
    void currentParameterChanged(const QColor &color, int size);
    /*!
     * Current setting parameter data changed.
     */
    void currentParameterIndexChanged(int cIndex, int sIndex);
    /*!
     * Current setting parameter index changed.
     */

public Q_SLOTS:
    void clear();
    /*!
     * Clean all contain data.
     */

protected:
    QStringList m_datas;
    MusicLrcAnalysis *m_lrcAnalysis;

};

#endif // TTKMUSICLYRICMODEL_H
