#include "tagmodel.h"

TagModel::TagModel(ModelCaps f)
    : m_f(f)
{

}

void TagModel::setValue(Qmmp::MetaData key, int value)
{
    setValue(key, QString::number(value));
}

QList<Qmmp::MetaData> TagModel::keys() const
{
    QList<Qmmp::MetaData> list;
    list << Qmmp::TITLE;
    list << Qmmp::ARTIST;
    list << Qmmp::ALBUMARTIST;
    list << Qmmp::ALBUM;
    list << Qmmp::COMMENT;
    list << Qmmp::GENRE;
    list << Qmmp::COMPOSER;
    list << Qmmp::YEAR;
    list << Qmmp::TRACK;
    list << Qmmp::DISCNUMBER;
    return list;
}

TagModel::ModelCaps TagModel::caps() const
{
    return m_f;
}

bool TagModel::exists() const
{
    return true;
}

void TagModel::create()
{

}

void TagModel::remove()
{

}

void TagModel::save()
{

}
