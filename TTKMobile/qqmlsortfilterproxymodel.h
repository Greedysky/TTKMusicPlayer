/****************************************************************************
 * This file is part of Fluid.
 *
 * Copyright (C) 2016 Pierre-Yves Siret
 *
 * $BEGIN_LICENSE:MIT$
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef QQMLSORTFILTERPROXYMODEL_H
#define QQMLSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QQmlExpression>

class QQmlSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QString filterRoleName READ filterRoleName WRITE setFilterRoleName NOTIFY
                   filterRoleNameChanged)
    Q_PROPERTY(
        QString filterPattern READ filterPattern WRITE setFilterPattern NOTIFY filterPatternChanged)
    Q_PROPERTY(PatternSyntax filterPatternSyntax READ filterPatternSyntax WRITE
                   setFilterPatternSyntax NOTIFY filterPatternSyntaxChanged)
    Q_PROPERTY(QVariant filterValue READ filterValue WRITE setFilterValue NOTIFY filterValueChanged)
    Q_PROPERTY(QQmlScriptString filterExpression READ filterExpression WRITE setFilterExpression
                   NOTIFY filterExpressionChanged)

    Q_PROPERTY(
        QString sortRoleName READ sortRoleName WRITE setSortRoleName NOTIFY sortRoleNameChanged)
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder)
    Q_PROPERTY(QQmlScriptString sortExpression READ sortExpression WRITE setSortExpression NOTIFY
                   sortExpressionChanged)

public:
    enum PatternSyntax {
        RegExp = QRegExp::RegExp,
        Wildcard = QRegExp::Wildcard,
        FixedString = QRegExp::FixedString,
        RegExp2 = QRegExp::RegExp2,
        WildcardUnix = QRegExp::WildcardUnix,
        W3CXmlSchema11 = QRegExp::W3CXmlSchema11
    };
    Q_ENUM(PatternSyntax)

    QQmlSortFilterProxyModel(QObject *parent = 0);

    int count() const;

    const QString &filterRoleName() const;
    void setFilterRoleName(const QString &filterRoleName);

    QString filterPattern() const;
    void setFilterPattern(const QString &filterPattern);

    PatternSyntax filterPatternSyntax() const;
    void setFilterPatternSyntax(PatternSyntax patternSyntax);

    const QVariant &filterValue() const;
    void setFilterValue(const QVariant &filterValue);

    const QQmlScriptString &filterExpression() const;
    void setFilterExpression(const QQmlScriptString &filterScriptString);

    const QString &sortRoleName() const;
    void setSortRoleName(const QString &sortRoleName);

    void setSortOrder(Qt::SortOrder sortOrder);

    const QQmlScriptString &sortExpression() const;
    void setSortExpression(const QQmlScriptString &compareScriptString);

signals:
    void countChanged();

    void filterRoleNameChanged();
    void filterPatternSyntaxChanged();
    void filterPatternChanged();
    void filterValueChanged();
    void filterExpressionChanged();

    void sortRoleNameChanged();
    void sortExpressionChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;

private slots:
    void invalidateFilter();
    void updateFilterRole();
    void updateSortRole();
    void updateRoles();

private:
    QVariantMap modelDataMap(const QModelIndex &modelIndex) const;

    QString m_filterRoleName;
    QString m_sortRoleName;

    QQmlScriptString m_filterScriptString;
    QQmlExpression *m_filterExpression;

    QQmlScriptString m_compareScriptString;
    QQmlExpression *m_compareExpression;
    QVariant m_filterValue;
};

#endif // QQMLSORTFILTERPROXYMODEL_H
