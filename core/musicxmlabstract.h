#ifndef MUSICXMLABSTRACT_H
#define MUSICXMLABSTRACT_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

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
    QString readXmlByTagNameAndAttribute(const QString &tagName) const;

    void createProcessingInstruction();
    QDomElement createRoot(const QString &node);
    QDomElement writeDom(QDomElement &element, const QString &node);
    QDomElement writeDomElement(QDomElement &element, const QString &node,
                                const QString &key, const QVariant &value);
    QDomElement writeDomEleText(QDomElement &element, const QString &node,
                                const QString &key, const QVariant &value,
                                const QString &text);
    QDomElement writeDomText(QDomElement &element, const QString &node, const QString &text);

    QFile *m_file;
    QDomDocument *m_ddom;

};

#endif // MUSICXMLABSTRACT_H
