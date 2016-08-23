#ifndef MUSICABSTRACTXML_H
#define MUSICABSTRACTXML_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QtXml/QDomDocument>
#include <QTextStream>
#include <QFile>

#include "musicglobaldefine.h"
#include "musicsong.h"
#include "musicobject.h"

/*! @brief The class of the base class of xml interface.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicAbstractXml : public QObject
{
    Q_OBJECT
public:
    explicit MusicAbstractXml(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicAbstractXml();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    bool readConfig(const QString &name);
    /*!
     * Read datas from xml file by given name.
     */
    bool writeConfig(const QString &name);
    /*!
     * Write datas into xml file by given name.
     */
    QString readXmlAttributeByTagNameValue(const QString &tagName) const;
    /*!
     * Read xml attribute by tagName and default name "value".
     */
    QString readXmlAttributeByTagName(const QString &tagName,
                                      const QString &attrName) const;
    /*!
     * Read xml attribute by tagName and attribute name.
     */
    QString readXmlTextByTagName(const QString &tagName) const;
    /*!
     * Read xml attribute's text by tagName.
     */
    MusicObject::MStriantMap readXmlAttributesByTagName(const QString &tagName) const;
    /*!
     * Read xml attributes by tagName and default name "value".
     */
    void createProcessingInstruction();
    /*!
     * Create processing instruction in header.
     */
    QDomElement createRoot(const QString &node);
    /*!
     * Create xml node nodes by node name.
     */
    QDomElement writeDom(QDomElement &element, const QString &node);
    /*!
     * Append xml element nodes by node name.
     */
    QDomElement writeDomElement(QDomElement &element, const QString &node,
                                const QString &key, const QVariant &value);
    /*!
     * Append xml element nodes by node name\ key name and value.
     */
    QDomElement writeDomElementMutil(QDomElement &element, const QString &node,
                                     const QStringList &keys, const QVariantList &values);
    /*!
     * Append xml elements nodes by node name\ keys name and values.
     */
    QDomElement writeDomElementText(QDomElement &element, const QString &node,
                                    const QString &key, const QVariant &value,
                                    const QString &text);
    /*!
     * Append xml element nodes by node name\ key name \ value and attribute's text.
     */
    QDomElement writeDomElementMutilText(QDomElement &element, const QString &node,
                                         const QStringList &keys, const QVariantList &values,
                                         const QString &text);
    /*!
     * Append xml elements nodes by node name\ keys name \ values and attribute's text.
     */
    QDomElement writeDomText(QDomElement &element, const QString &node, const QString &text);
    /*!
     * Append xml element nodes by node name and attribute's text.
     */
    void writeAttribute(QDomElement &element, const QString &key, const QVariant &value);
    /*!
     * Append xml element nodes key name and value.
     */

protected:
    QFile *m_file;
    QDomDocument *m_ddom;

};

#endif // MUSICABSTRACTXML_H
