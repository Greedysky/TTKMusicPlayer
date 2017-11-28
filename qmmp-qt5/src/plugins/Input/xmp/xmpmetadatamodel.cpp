/***************************************************************************
 *   Copyright (C) 2015 by Ilya Kotov                                      *
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

XmpMetaDataModel::XmpMetaDataModel(const QString &path, QObject *parent) : MetaDataModel(parent)
{
    m_path = path;
    m_ctx = xmp_create_context();
    int err = xmp_load_module(m_ctx, m_path.toLocal8Bit().data());
    if(err != 0)
    {
        xmp_free_context(m_ctx);
        m_ctx = 0;
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

QHash<QString, QString> XmpMetaDataModel::audioProperties()
{
    QHash<QString, QString> ap;
    ap.insert(tr("File name"), m_path.section('/',-1));

    xmp_module_info mi;
    xmp_get_module_info(m_ctx, &mi);

    ap.insert(tr("Format"), mi.mod->type);
    ap.insert(tr("Volume scale"), QString::number(mi.vol_base));
    ap.insert(tr("Number of patterns"), QString::number(mi.mod->pat));
    ap.insert(tr("Number of tracks"), QString::number(mi.mod->trk));
    ap.insert(tr("Tracks per pattern"), QString::number(mi.mod->chn));
    ap.insert(tr("Number of instruments"), QString::number(mi.mod->ins));
    ap.insert(tr("Number of samples"), QString::number(mi.mod->smp));
    ap.insert(tr("Initial speed"), QString::number(mi.mod->spd));
    ap.insert(tr("Initial BPM"), QString::number(mi.mod->bpm));
    ap.insert(tr("Module length in patterns"), QString::number(mi.mod->len));

    return ap;
}

QHash<QString, QString> XmpMetaDataModel::descriptions()
{
    QHash<QString, QString> desc;
    QString text;

    xmp_module_info mi;
    xmp_get_module_info(m_ctx, &mi);

    for(int i = 0; i < mi.mod->smp; i++)
    {
        text += QString::fromUtf8(mi.mod->xxs[i].name) + '\n';
    }
    text = text.trimmed();
    if(!text.isEmpty())
        desc.insert(tr("Samples"), text);
    text.clear();
    for(int i = 0; i < mi.mod->ins; i++)
    {
        text += QString::fromUtf8(mi.mod->xxi[i].name) + '\n';
    }
    text = text.trimmed();
    if(!text.isEmpty())
        desc.insert(tr("Instruments"), text);
    text.clear();

    text = QString::fromUtf8(mi.comment).trimmed();
    if(!text.isEmpty())
        desc.insert(tr("Comment"), text);
    return desc;
}
