#ifndef MUSICXMLABSTRACT_H
#define MUSICXMLABSTRACT_H

#include <QtXml/QDomDocument>
#include <QTextStream>
#include <QFile>

#include "musiclibexportglobal.h"
#include "musicobject.h"

class MUSIC_CORE_EXPORT MusicXmlAbstract : public QObject
{
    Q_OBJECT
public:
    explicit MusicXmlAbstract(QObject *parent = 0);
    ~MusicXmlAbstract();

protected:
    bool readConfig(const QString &type);
    bool writeConfig(const QString &type);
    QString readXmlByTagNameAndAttribute(const QString &tagName);
    QFile *m_file;
    QDomDocument *m_ddom;

};

#endif // MUSICXMLABSTRACT_H
