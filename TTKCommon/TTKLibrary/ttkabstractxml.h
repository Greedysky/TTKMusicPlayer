#ifndef TTKABSTRACTXML_H
#define TTKABSTRACTXML_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2023 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QFile>
#include <QTextStream>
#include <QtXml/QDomDocument>

#include "ttktime.h"
#include "ttkglobaldefine.h"

/*! @brief The class of the ttk xml attribute.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT TTKXmlAttribute
{
    QString m_key;
    QVariant m_value;

    TTKXmlAttribute(const QString &key, const QVariant &value)
        : m_key(key),
          m_value(value)
    {

    }
};
TTK_DECLARE_LIST(TTKXmlAttribute);

/*! @brief The class of the ttk xml node hepler.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKXmlNodeHelper
{
public:
    /*!
     * Object contsructor.
     */
    explicit TTKXmlNodeHelper(const QDomNode &root);

    /*!
     * Load xml node helper.
     */
    void load();
    /*!
     * Check xml has next node.
     */
    bool hasNext();

    /*!
     * Get xml next node.
     */
    QDomNode next() const;
    /*!
     * Get match node name case insensitive.
     */
    QString nodeName(const QString &name) const;

private:
    QDomNode m_root, m_current;
    QSet<QString> m_nodeNames;

};

/*! @brief The class of the ttk xml interface.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKAbstractXml : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(TTKAbstractXml)
public:
    /*!
     * Object contsructor.
     */
    explicit TTKAbstractXml(QObject *parent = nullptr);
    ~TTKAbstractXml();

    /*!
     * Read datas from xml file by given name.
     */
    bool fromFile(const QString &name);
    /*!
     * Xml stream data from string.
     */
    bool fromString(const QString &data);
    /*!
     * Xml stream data from byteArray.
     */
    bool fromByteArray(const QByteArray &data);
    /*!
     * Write datas into xml file by given name.
     */
    bool toFile(const QString &name);

    /*!
     * Xml stream data to string.
     */
    QString toString() const;
    /*!
     * Xml stream data to byteArray.
     */
    QByteArray toByteArray() const;

    /*!
     * Read xml attribute by tagName and default name "value".
     */
    QString readXmlAttributeByTagNameValue(const QString &tagName) const;
    /*!
     * Read xml attribute by tagName and attribute name.
     */
    QString readXmlAttributeByTagName(const QString &tagName, const QString &attrName) const;
    /*!
     * Read xml attribute's text by tagName.
     */
    QString readXmlTextByTagName(const QString &tagName) const;
    /*!
     * Read xml attributes by tagName and default name "value".
     */
    TTKVariantMap readXmlAttributesByTagName(const QString &tagName) const;
    /*!
     * Create processing instruction in header.
     */
    void createProcessingInstruction();
    /*!
     * Create xml node nodes by node name.
     */
    QDomElement createRoot(const QString &node);
    /*!
     * Create xml node nodes by node atrr.
     */
    QDomElement createRoot(const QString &node, const TTKXmlAttribute &attr);
    /*!
     * Create xml node nodes by node atrrs.
     */
    QDomElement createRoot(const QString &node, const TTKXmlAttributeList &attrs);
    /*!
     * Append xml element nodes by node name.
     */
    QDomElement writeDomNode(QDomElement &element, const QString &node);
    /*!
     * Append xml element nodes by node name\ key name and value.
     */
    QDomElement writeDomElement(QDomElement &element, const QString &node, const TTKXmlAttribute &attr);
    /*!
     * Append xml elements nodes by node name\ keys name and values.
     */
    QDomElement writeDomMutilElement(QDomElement &element, const QString &node, const TTKXmlAttributeList &attrs);
    /*!
     * Append xml element nodes by node name\ key name \ value and attribute's text.
     */
    QDomElement writeDomElementText(QDomElement &element, const QString &node, const TTKXmlAttribute &attr, const QString &text);
    /*!
     * Append xml elements nodes by node name\ keys name \ values and attribute's text.
     */
    QDomElement writeDomMutilElementText(QDomElement &element, const QString &node, const TTKXmlAttributeList &attrs, const QString &text);
    /*!
     * Append xml element nodes by node name and attribute's text.
     */
    QDomElement writeDomText(QDomElement &element, const QString &node, const QString &text);
    /*!
     * Append xml element nodes key name and value.
     */
    void writeAttribute(QDomElement &element, const TTKXmlAttribute &attr);

protected:
    QFile *m_file;
    QDomDocument *m_document;

};

#endif // TTKABSTRACTXML_H
