/***************************************************************************
 *   Copyright (C) 2015-2019 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <QFile>
#include "xmpmetadatamodel.h"

XmpMetaDataModel::XmpMetaDataModel(const QString &path) : MetaDataModel(true)
{
    m_path = path;
    m_ctx = xmp_create_context();
    int err = xmp_load_module(m_ctx, m_path.toLocal8Bit().data());
    if(err != 0)
    {
        xmp_free_context(m_ctx);
        m_ctx = nullptr;
        qWarning("XmpMetaDataModel: unable to load module file, error = %d", err);
    }
}

XmpMetaDataModel::~XmpMetaDataModel()
{
    if(m_ctx)
    {
        xmp_release_module(m_ctx);
        xmp_free_context(m_ctx);
    }
}

QList<MetaDataItem> XmpMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;

    xmp_module_info mi;
    xmp_get_module_info(m_ctx, &mi);

    ep << MetaDataItem(tr("Volume scale"), mi.vol_base);
    ep << MetaDataItem(tr("Number of patterns"), mi.mod->pat);
    ep << MetaDataItem(tr("Number of tracks"), mi.mod->trk);
    ep << MetaDataItem(tr("Tracks per pattern"), mi.mod->chn);
    ep << MetaDataItem(tr("Number of instruments"), mi.mod->ins);
    ep << MetaDataItem(tr("Number of samples"),mi.mod->smp);
    ep << MetaDataItem(tr("Initial speed"), mi.mod->spd);
    ep << MetaDataItem(tr("Initial BPM"), mi.mod->bpm);
    ep << MetaDataItem(tr("Module length in patterns"), mi.mod->len);

    return ep;
}

QList<MetaDataItem> XmpMetaDataModel::descriptions() const
{
    QList<MetaDataItem> desc;
    QString text;

    xmp_module_info mi;
    xmp_get_module_info(m_ctx, &mi);

    for(int i = 0; i < mi.mod->smp; i++)
    {
        text += QString::fromUtf8(mi.mod->xxs[i].name) + '\n';
    }
    text = text.trimmed();
    if(!text.isEmpty())
        desc << MetaDataItem(tr("Samples"), text);
    text.clear();
    for(int i = 0; i < mi.mod->ins; i++)
    {
        text += QString::fromUtf8(mi.mod->xxi[i].name) + '\n';
    }
    text = text.trimmed();
    if(!text.isEmpty())
        desc << MetaDataItem(tr("Instruments"), text);
    text.clear();

    text = QString::fromUtf8(mi.comment).trimmed();
    if(!text.isEmpty())
        desc << MetaDataItem(tr("Comment"), text);
    return desc;
}
