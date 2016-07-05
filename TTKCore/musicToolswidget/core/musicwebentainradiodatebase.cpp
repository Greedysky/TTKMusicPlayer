#include "musicwebentainradiodatebase.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDateTime>
#include <QVariant>

MusicWebEntainRadioDatabase::MusicWebEntainRadioDatabase()
{

}

QString MusicWebEntainRadioDatabase::getClassName()
{
    return "MusicWebEntainRadioDatabase";
}

bool MusicWebEntainRadioDatabase::disConnectDatabase() const
{
    QString connectionName;
    {
        QSqlDatabase data = QSqlDatabase::database("radio-data");
        connectionName = data.connectionName();
        if( data.isValid() )
        {
            data.close();
        }
        else
        {
            return false;
        }
    }
    QSqlDatabase::removeDatabase( connectionName );
    return true;
}

bool MusicWebEntainRadioDatabase::connectDatabase() const
{
    try
    {
        QSqlDatabase data = QSqlDatabase::addDatabase(SQLITE_DATABASE, "radio-data");
        data.setDatabaseName(NETDADIOPATH_FULL);
        if( !data.isDriverAvailable(SQLITE_DATABASE) )
        {
            throw QString("The driver name is not available!");
        }
        if( !data.isValid() )
        {
            throw QString("The database has not a valid driver!");
        }
        if (!data.isOpen() && !data.open() )
        {
            throw QString("Can not open database connection!");
        }

        if(!data.tables().contains("channel"))
        {
            QSqlQuery query(data);
            query.exec("create table channel(channel_id int primary key, channel_name varchar, category varchar, "
                       "channel_address varchar)");
        }
        if(!data.tables().contains("collect"))
        {
            QSqlQuery query(data);
            query.exec("create table collect(Id INTEGER PRIMARY KEY AUTOINCREMENT, channel_id int, foreign key (channel_id) references channel(channel_id))");
        }
        if(!data.tables().contains("recently"))
        {
            QSqlQuery query(data);
            query.exec("create table recently(Id INTEGER PRIMARY KEY AUTOINCREMENT, channel_id int, time text, foreign key (channel_id) references channel(channel_id))");
        }
    }
    catch(QString exception)
    {
        M_LOGGER_INFO(QString("Error Message :%1").arg(exception));
        return false;
    }
    return true;
}

QString MusicWebEntainRadioDatabase::getRadioUrl(const QString &channelName) const
{
    QSqlQuery query(QSqlDatabase::database("radio-data"));
    query.exec("select * from channel");
    while(query.next())
    {
        if(query.value(1).toString() == channelName)
        {
            return query.value(3).toString();
        }
    }
    return QString();
}

QStringList MusicWebEntainRadioDatabase::getRadioNames(const QString &category) const
{
    QStringList channelNames;
    QSqlQuery query(QString("select * from channel where category='%1'").arg(category),
                             QSqlDatabase::database("radio-data"));
    while(query.next())
    {
        channelNames << query.value(1).toString();
    }
    return channelNames;
}

QStringList MusicWebEntainRadioDatabase::getRecords(const QString& filter) const
{
    QStringList channelNames;
    QSqlQuery query(filter, QSqlDatabase::database("radio-data"));
    while(query.next())
    {
        QSqlQuery queryC(QString("select * from channel where channel_id='%1'").arg(query.value(1).toInt()),
                                 QSqlDatabase::database("radio-data"));
        if(queryC.next())
        {
            channelNames << queryC.value(1).toString();
        }
    }
    return channelNames;
}

QStringList MusicWebEntainRadioDatabase::getRecentNames() const
{
    return getRecords("select * from recently order by time DESC");
}

QStringList MusicWebEntainRadioDatabase::getFavouriteNames() const
{
    return getRecords("select * from collect");
}

void MusicWebEntainRadioDatabase::radioRecentPlay(const QString &channelName) const
{
    QSqlQuery query(QSqlDatabase::database("radio-data"));
    QSqlQuery queryC(QSqlDatabase::database("radio-data"));
    query.exec("select * from channel");
    while(query.next())
    {
        if(query.value(1).toString() == channelName)
        {
            int channelId = query.value(0).toInt();
            queryC.exec("select * from recently");
            while(queryC.next())
            {
                if(queryC.value(1).toInt() == channelId)
                {
                    queryC.prepare("update recently set time =:datetime where channel_id=:channel_id");
                    queryC.bindValue(":datetime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
                    queryC.bindValue(":channel_id", channelId);
                    queryC.exec();
                    return;
                }
            }
            queryC.prepare("insert into recently(channel_id,time) values(:channel_id,:time)");
            queryC.bindValue(":channel_id", channelId);
            queryC.bindValue(":time", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            queryC.exec();
            return;
        }
    }
}

void MusicWebEntainRadioDatabase::radioCollection(const QString &name) const
{
    QSqlQuery query(QString("select * from channel where channel_name='%1'").arg(name),
                             QSqlDatabase::database("radio-data"));
    int channelId = 0;
    if(query.next())
    {
        channelId = query.value(0).toInt();
    }

    QSqlQuery queryC(QString("select * from collect where channel_id='%1'").arg(channelId),
                             QSqlDatabase::database("radio-data"));
    if(!queryC.next())
    {
        queryC.prepare("insert into collect(channel_id) values(:channel_id)");
        queryC.bindValue(":channel_id", channelId);
        queryC.exec();
    }
}

void MusicWebEntainRadioDatabase::radioDiscollection(const QString &name) const
{
    QSqlQuery query(QString("select * from channel where channel_name='%1'").arg(name),
                             QSqlDatabase::database("radio-data"));
    int channelId = 0;
    if(query.next())
    {
        channelId = query.value(0).toInt();
    }

    QSqlQuery queryC(QString("select * from collect where channel_id='%1'").arg(channelId),
                             QSqlDatabase::database("radio-data"));
    if(queryC.next())
    {
        queryC.prepare("delete from collect where channel_id = :channel_id");
        queryC.bindValue(":channel_id", channelId);
        queryC.exec();
    }
}
