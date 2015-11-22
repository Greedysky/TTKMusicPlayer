#ifndef MUSICABSTRACTXML_H
#define MUSICABSTRACTXML_H

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
#include "musicsong.h"
#include "musicobject.h"

class MUSIC_CORE_EXPORT MusicAbstractXml : public QObject
{
    Q_OBJECT
public:
    explicit MusicAbstractXml(QObject *parent = 0);
    ~MusicAbstractXml();

protected:
    bool readConfig(const QString &type);
    bool writeConfig(const QString &type);
    QString readXmlAttributeByTagNameValue(const QString &tagName) const;
    QString readXmlAttributeByTagName(const QString &tagName,
                                      const QString &attrName) const;
    QString readXmlTextByTagName(const QString &tagName) const;
    MStriantMap readXmlAttributesByTagName(const QString &tagName) const;

    void createProcessingInstruction();
    QDomElement createRoot(const QString &node);
    QDomElement writeDom(QDomElement &element, const QString &node);
    QDomElement writeDomElement(QDomElement &element, const QString &node,
                                const QString &key, const QVariant &value);
    QDomElement writeDomElementMutil(QDomElement &element, const QString &node,
                                     const QStringList &keys, const QList<QVariant> &values);
    QDomElement writeDomElementText(QDomElement &element, const QString &node,
                                    const QString &key, const QVariant &value,
                                    const QString &text);
    QDomElement writeDomElementMutilText(QDomElement &element, const QString &node,
                                         const QStringList &keys, const QList<QVariant> &values,
                                         const QString &text);
    QDomElement writeDomText(QDomElement &element, const QString &node, const QString &text);
    void writeAttribute(QDomElement &element, const QString &key, const QVariant &value);

    QFile *m_file;
    QDomDocument *m_ddom;

};

#endif // MUSICABSTRACTXML_H
