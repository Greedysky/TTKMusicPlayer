#ifndef TTKABSTRACTXML_H
#define TTKABSTRACTXML_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2025 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QFile>
#include <QTextStream>
#include <QtXml/QDomDocument>

#include "ttktime.h"
#include "ttkabstractbufferinterface.h"

/*! @brief The class of the ttk xml attribute.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT TTKXmlAttr
{
    QString m_key;
    QVariant m_value;

    TTKXmlAttr() = default;
    TTKXmlAttr(const QString &key, const QVariant &value) noexcept
        : m_key(key),
          m_value(value)
    {

    }
};
TTK_DECLARE_LIST(TTKXmlAttr);


/*! @brief The class of the ttk xml node.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT TTKXmlNode
{
    TTKXmlAttrList m_attrs;
    QString m_text;

    TTKXmlNode() noexcept
        : m_attrs(),
          m_text()
    {

    }
};
TTK_DECLARE_LIST(TTKXmlNode);


/*! @brief The class of the ttk xml hepler.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKXmlHelper
{
public:
    /*!
     * Object constructor.
     */
    explicit TTKXmlHelper(const QDomNode &root) noexcept;

    /*!
     * Load xml helper.
     */
    void load();
    /*!
     * Check xml has next node.
     */
    bool hasNext();

    /*!
     * Get xml next node.
     */
    QDomNode next() const noexcept;
    /*!
     * Get match node name case insensitive.
     */
    QString nodeName(const QString &name) const noexcept;

private:
    QDomNode m_root, m_current;
    QSet<QString> m_nodeNames;

};


/*! @brief The class of the ttk xml interface.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKAbstractXml
{
    TTK_DECLARE_MODULE(TTKAbstractXml)
public:
    /*!
     * Object constructor.
     */
    TTKAbstractXml();
    /*!
     * Object destructor.
     */
    virtual ~TTKAbstractXml();

    /*!
     * Init document by given name.
     */
    bool load(const QString &name);
    /*!
     * Save xml stream data to local.
     */
    void save() const;
    /*!
     * Reset xml stream data.
     */
    bool reset();

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
     * Xml stream data to string.
     */
    QString toString() const;
    /*!
     * Xml stream data to byteArray.
     */
    QByteArray toByteArray() const;

public:
    /*!
     * Read xml attribute by tagName and attribute name.
     */
    QString readAttributeByTagName(const QString &node, const QString &attrName = "value") const;
    /*!
     * Read xml attribute's text by tagName.
     */
    QString readTextByTagName(const QString &node) const;
    /*!
     * Read xml node by tagName.
     */
    TTKXmlNode readNodeByTagName(const QString &node) const;

    /*!
     * Read xml multi attribute by tagName and attribute name.
     */
    QStringList readMultiAttributeByTagName(const QString &node, const QString &attrName = "value") const;
    /*!
     * Read xml multi attribute's text by tagName.
     */
    QStringList readMultiTextByTagName(const QString &node) const;
    /*!
     * Read xml multi node by tagName.
     */
    TTKXmlNodeList readMultiNodeByTagName(const QString &node) const;

public:
    /*!
     * Create xml node nodes by node name.
     */
    QDomElement createRoot(const QString &node) const;
    /*!
     * Create xml node nodes by node atrr.
     */
    QDomElement createRoot(const QString &node, const TTKXmlAttr &attr) const;
    /*!
     * Create xml node nodes by node atrrs.
     */
    QDomElement createRoot(const QString &node, const TTKXmlAttrList &attrs) const;

    /*!
     * Write xml element node by node name.
     */
    QDomElement writeDomElement(QDomElement &element, const QString &node) const;
    /*!
     * Write xml element node by node name and attribute's text.
     */
    QDomElement writeDomElement(QDomElement &element, const QString &node, const QString &text) const;
    /*!
     * Write xml element node by node name key name value and attribute's text.
     */
    QDomElement writeDomElement(QDomElement &element, const QString &node, const TTKXmlNode &attr) const;
    /*!
     * Write xml element node by node name key name and value.
     */
    QDomElement writeDomElement(QDomElement &element, const QString &node, const TTKXmlAttr &attr) const;
    /*!
     * Write xml element node by node name key name value and attribute's text.
     */
    QDomElement writeDomElement(QDomElement &element, const QString &node, const TTKXmlAttr &attr, const QString &text) const;

    /*!
     * Write xml elements nodes by node name keys name and values.
     */
    QDomElement writeDomMultiElement(QDomElement &element, const QString &node, const TTKXmlNode &attrs) const;
    /*!
     * Write xml elements nodes by node name keys name and values.
     */
    QDomElement writeDomMultiElement(QDomElement &element, const QString &node, const TTKXmlAttrList &attrs) const;
    /*!
     * Write xml elements nodes by node name keys name values and attribute's text.
     */
    QDomElement writeDomMultiElement(QDomElement &element, const QString &node, const TTKXmlAttrList &attrs, const QString &text) const;

    /*!
     * Write xml element node by node name.
     */
    QDomNodeList findDomNodes(const QString &node) const;

    /*!
     * Create processing instruction in header.
     */
    void createProcessingInstruction() const;

    /*!
     * Write xml element node key name and value.
     */
    void writeAttribute(QDomElement &element, const TTKXmlAttr &attr) const;
    /*!
     * Write xml element nodes key name and value.
     */
    void writeAttribute(QDomElement &element, const TTKXmlAttrList &attr) const;

protected:
    QFile *m_file;
    QDomDocument *m_document;

};

#endif // TTKABSTRACTXML_H
