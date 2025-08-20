#include "regularexpression.h"

RegularExpression::RegularExpression(const QString &pattern)
    : m_regular(pattern)
{

}

bool RegularExpression::hasMatch(const QString &str)
{
#if QT_VERSION >= QT_VERSION_CHECK(5,1,0)
    m_match = m_regular.match(str);
    return m_match.hasMatch();
#elif QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    m_match = str;
    return m_regular.match(str).hasMatch();
#else
    return str.indexOf(m_regular) != -1;
#endif
}

QString RegularExpression::value(int index) const
{
#if QT_VERSION >= QT_VERSION_CHECK(5,1,0)
    return m_match.captured(index);
#elif QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    return m_regular.match(m_match).captured(index);
#else
    return m_regular.cap(index);
#endif
}

QString RegularExpression::escape(const QString &str)
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    return QRegularExpression::escape(str);
#else
    return QRegExp::escape(str);
#endif
}

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
RegularExpression::operator QRegularExpression () const
#else
RegularExpression::operator QRegExp () const
#endif
{
    return m_regular;
}
